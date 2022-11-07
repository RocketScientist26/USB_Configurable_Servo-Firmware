#include "main.h"
#include "app.h"
#include "usb.h"
#include "motor.h"
#include "potentiometer.h"
#include "pid.h"
#include "led.h"
#include "signal.h"
#include "settings.h"

extern float pid_input;
extern uint8_t pid_running;

extern uint8_t motor_systick;
extern uint8_t motor_running;

extern float potentiometer_position;
extern uint16_t potentiometer_min;
extern uint16_t potentiometer_max;

extern uint8_t signal_ignore;

extern uint8_t usb_present;
extern uint8_t usb_rq;
extern uint8_t usb_rq_sett_revert_flash;
extern uint8_t usb_rq_stat_motor;
extern uint8_t usb_rq_stat_phold;

void Potentiometer_Ready(){
	pid_input = potentiometer_position;
	if(!pid_running){
		if(motor_running){
			if((potentiometer_position <= potentiometer_min) && (Motor_Get() < 0)){
				Motor_Set(0);
			}else if((potentiometer_position >= potentiometer_max) && (Motor_Get() > 0)){
				Motor_Set(0);
			}
		}
	}
}
void PID_Ready(float output){
	Motor_Set(output);
}

void App_Init(){
	Settings_Read();
	USB_Det();
	Signal_Init();
	Potentiometer_Init();
	Motor_Init();
}
void App_Loop(){
	switch(usb_rq){
		case USB_RQ_CONF:
			if(usb_rq_sett_revert_flash == USB_SETTINGS_REVERT){
				usb_rq_sett_revert_flash = USB_SETTINGS_KEEP;
				//Read from flash
				Settings_Read();
			}else if(usb_rq_sett_revert_flash == USB_SETTINGS_FLASH){
				usb_rq_sett_revert_flash = USB_SETTINGS_KEEP;
				//Write to flash
				Settings_Write();
			}
			USB_Send_Config();
			usb_rq = USB_RQ_NONE;
		break;
		case USB_RQ_STAT:
			if(usb_rq_stat_motor != 0){
				//Rotate motor with MOTOR_USB_TEST_TIMEOUT_MS timeout
				if(usb_rq_stat_motor == 1){//Backward
					if(potentiometer_position > potentiometer_min){
						Motor_Set(-1000);
					}
				}else if(usb_rq_stat_motor == 2){//Forward
					if(potentiometer_position < potentiometer_max){
						Motor_Set(1000);
					}
				}
				motor_systick = 0;
				usb_rq_stat_motor = 0;
			}
			if(usb_rq_stat_phold == 1){
				usb_rq_stat_phold = 0;
				if(!pid_running){//Run pid with signal timeout
					PID_Start();
				}
			}else if(signal_ignore && pid_running){
				PID_Stop();
			}
			USB_Send_Status();
			usb_rq = USB_RQ_NONE;
		break;
	}
	USB_Det();
	LED_Update();
}
