#include <math.h>
#include "main.h"
#include "System/Drivers/led.h"
#include "System/Drivers/motor.h"
#include "System/Drivers/pid.h"
#include "System/Drivers/potentiometer.h"
#include "System/Drivers/signal.h"
#include "System/Drivers/usb.h"
#include "settings.h"

//PID
void PID_Ready(float output){
	Motor_Set(output);
}

//Potentiometer
void Potentiometer_Ready(){
	pid_i.input = potentiometer_o.position;
	if(!pid_o.running){
		if(motor_o.running){
			if((potentiometer_o.position <= settings_data.potentiometer_min) && (Motor_Get() < 0)){
				Motor_Set(0);
			}else if((potentiometer_o.position >= settings_data.potentiometer_max) && (Motor_Get() > 0)){
				Motor_Set(0);
			}
		}
	}
}

//Signal
void Signal_Gone(){
	if(!settings_data.signal_ignore || (settings_data.signal_ignore && (!usb_o.usb_present))){
		if(pid_o.running){
			PID_Stop();
		}
		pid_i.setpoint = 0;
	}
}
void Signal_Received(float length_ms){
	if(!usb_o.usb_present || (!settings_data.signal_ignore && usb_o.usb_present)){
		if((length_ms <= settings_data.signal_max) && (length_ms >= settings_data.signal_min)){
			//Calculate new PID setpoint
			float new_pid_setpoint =
					(
						((float)settings_data.potentiometer_max - (float)settings_data.potentiometer_min) * //Total enabled potentiometer scale
						((length_ms - settings_data.signal_min) / (settings_data.signal_max - settings_data.signal_min)) //Received signal to total signal scale ratio
					) + (float)settings_data.potentiometer_min;

			//Detect if position change is enough for LED to be fired, if it is in this mode
			if(
				fabs((new_pid_setpoint - (float)settings_data.potentiometer_min) - (pid_i.setpoint - (float)settings_data.potentiometer_min)) /
				(
					((float)settings_data.potentiometer_max - (float)settings_data.potentiometer_min) / 100.0f
				)
				>=
				SETTINGS_LED_POSITION_CHANGE_ACCURACY_PRCNT
			){
				led_i.position_changed = true;
			}

			pid_i.setpoint = new_pid_setpoint;
			if(!pid_o.running){
				PID_Start();
			}
		}
	}
}
