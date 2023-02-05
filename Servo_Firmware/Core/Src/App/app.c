#include "main.h"
#include "System/Drivers/led.h"
#include "System/Drivers/motor.h"
#include "System/Drivers/pid.h"
#include "System/Drivers/potentiometer.h"
#include "System/Drivers/signal.h"
#include "System/Drivers/usb.h"
#include "app.h"
#include "settings.h"

void App_Init(){
	//Read configuration from flash into "settings_data" global struct
	Settings_Read();
	//Apply configuration to functions
	Settings_Apply();

	//Start motor PWM
	Motor_Init();
	//Start potentiometer measurements
	Potentiometer_Init();
}
void App_Loop(){
	//Reply and handle USB requests
	switch(usb_o.usb_rq){
		//Configuration request from host
		case USB_RQ_CONF:
			//Flash update or revert if "set_revert_flash_rq" byte is set so in received packet
			switch(usb_o.set_revert_flash_rq){
				case USB_SETTINGS_REVERT:
					Settings_Read();
					Settings_Apply();
				break;
				case USB_SETTINGS_FLASH:
					Settings_Write();
				break;
			}

			//Reply with configuration
			USB_Send_Config(&settings_data);
		break;

		//Status request from host (incoming packet also contains configuration which we should set)
		case USB_RQ_STAT:
			//Copy packed struct to unpacked struct and apply configuration to functions
			settings_data = usb_rx_config;
			Settings_Apply();

			//Rotate motor with MOTOR_USB_TEST_TIMEOUT_MS timeout if "usb_rq_stat_motor" is set in received packet
			switch(usb_rx_status.usb_rq_stat_motor){
				case USB_MOTOR_BACKWARD:
					if(potentiometer_o.position > settings_data.potentiometer_min){
						Motor_Set(-1000);
					}
				break;
				case USB_MOTOR_FORWARD:
					if(potentiometer_o.position < settings_data.potentiometer_max){
						Motor_Set(1000);
					}
				break;
			}

			//Hold position with PID request if "usb_rq_stat_phold" is set in received packet
			if(usb_rx_status.usb_rq_stat_phold){
				pid_i.setpoint = usb_rx_status.pid_setpoint;
				if(!pid_o.running){
					//In this mode PID and motor operation automatically stops after signal_ctr.timeout_ms milliseconds unless renewed
					PID_Start();
				}
			}else if(settings_data.signal_ignore && pid_o.running){
				PID_Stop();
			}

			//Reply with status packet
			usb_tx_status_t status = {
				.pid_running = pid_o.running,
				.pid_setpoint = pid_i.setpoint,
				.potentiometer_position = potentiometer_o.position,
				.motor_power = Motor_Get()
			};
			USB_Send_Status(&status);
		break;
	}

	//Update USB is connected or not status variable
	USB_Det();
	//Operate LED
	led_i.pid_running = pid_o.running;
	LED_Update();
}
