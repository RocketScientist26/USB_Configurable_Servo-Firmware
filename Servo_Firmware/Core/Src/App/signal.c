#include "main.h" 
#include "signal.h"
#include "pid.h"

extern TIM_HandleTypeDef htim4;

extern uint8_t pid_running;
extern float pid_setpoint;
extern uint16_t potentiometer_min;
extern uint16_t potentiometer_max;
extern uint8_t usb_present;
extern uint8_t led_position_changed;

uint8_t signal_ignore;
float signal_length;
float signal_timeout;

uint8_t signal_present = 0;

void Signal_SysTick_Interrupt(){
	if(((float)(Signal_Read_Timer() + 1) / 500.0f) > signal_timeout){
		signal_present = 0;
		Signal_Timer_Reset();
		if(!signal_ignore || (signal_ignore && (!usb_present))){
			if(pid_running){
				PID_Stop();
			}
			pid_setpoint = 0;
		}
	}
}
void Signal_Interrupt(){
	uint32_t cnt = Signal_Read_Timer();
	if(!signal_ignore || (signal_ignore && (!usb_present))){
		if(Signal_Read() == SIGNAL_HIGH){
			Signal_Timer_Reset();
			signal_present = 1;
		}else if(signal_present){
			float received_length_ms  = (float)((uint32_t)cnt + (uint32_t)1) / 500.0f;
			if((received_length_ms <= signal_length) && (received_length_ms >= 1.0f)){
				float new_pid_setpoint = (((float)potentiometer_max - (float)potentiometer_min) * ((received_length_ms - 1.0f) / (signal_length - 1.0f))) + (float)potentiometer_min;
				if(pid_setpoint != new_pid_setpoint){
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
