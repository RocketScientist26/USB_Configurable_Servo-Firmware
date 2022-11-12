#include <math.h>
#include "main.h"
#include "signal.h"
#include "pid.h"
#include "led.h"

extern TIM_HandleTypeDef htim4;

extern uint8_t pid_running;
extern float pid_setpoint;
extern uint16_t potentiometer_min;
extern uint16_t potentiometer_max;
extern uint8_t usb_present;
extern uint8_t led_position_changed;

uint8_t signal_ignore;
float signal_min;
float signal_max;
uint32_t signal_timeout;

uint8_t signal_present = 0;
uint32_t signal_timeout_passed_ms = 0;

void Signal_SysTick_Interrupt(){
	if(signal_present){
		if(signal_timeout_passed_ms >= signal_timeout){
			signal_present = 0;
			if(!signal_ignore || (signal_ignore && (!usb_present))){
				if(pid_running){
					PID_Stop();
				}
				pid_setpoint = 0;
			}
		}else{
			signal_timeout_passed_ms++;
		}
	}
}
void Signal_Timer_Interrupt(){
	signal_present = 0;
}
void Signal_Interrupt(){
	uint32_t cnt = Signal_Read_Timer();
	if(Signal_Read() == SIGNAL_HIGH){
		Signal_Timer_Reset();
		signal_present = 1;
		signal_timeout_passed_ms = 0;
	}else if(signal_present){
		if(!usb_present || (!signal_ignore && usb_present)){
			float received_length_ms  = (float)((uint32_t)cnt + (uint32_t)1) / 24000.0f;
			if((received_length_ms <= signal_max) && (received_length_ms >= signal_min)){
				float new_pid_setpoint = (((float)potentiometer_max - (float)potentiometer_min) * ((received_length_ms - signal_min) / (signal_max - signal_min))) + (float)potentiometer_min;

				if(
					fabs((new_pid_setpoint - (float)potentiometer_min) - (pid_setpoint - (float)potentiometer_min)) / (((float)potentiometer_max - (float)potentiometer_min) / 100.0f)
					>=
					LED_POSITION_CHANGE_ACCURACY_PRCNT
				){
					led_position_changed = 1;
				}

				pid_setpoint = new_pid_setpoint;
				if(!pid_running){
					PID_Start();
				}
			}else{
				signal_present = 0;
				if(pid_running){
					PID_Stop();
				}
			}
		}
	}
}

uint32_t Signal_Read_Timer(){
	return __HAL_TIM_GET_COUNTER(&htim4);
}
void Signal_Timer_Reset(){
	__HAL_TIM_SET_COUNTER(&htim4, 0);
}
uint8_t Signal_Read(){
	uint8_t level;
	uint8_t d = 0;
	uint8_t i = 0;
	while(d != 1){
		i = 0;
		level = HAL_GPIO_ReadPin(SIGNAL_GPIO_Port, SIGNAL_Pin);
		while(i != SIGNAL_DEBOUNCE_SAMPLES){
			i++;
			if(HAL_GPIO_ReadPin(SIGNAL_GPIO_Port, SIGNAL_Pin) != level){
				break;
			}
			if(i == SIGNAL_DEBOUNCE_SAMPLES){
				d = 1;
			}
		}
	}
	return level;
}

void Signal_Init(){
	HAL_TIM_Base_Start(&htim4);
}
