#include "main.h"
#include "signal.h"

extern TIM_HandleTypeDef htim4;

//Control/status variables
signal_ctr_t signal_ctr;
//Output variables
signal_o_t signal_o;

//Working variables
static uint32_t timeout_passed_ms = 0; //Counter for measuring time since getting first edge of signal
static bool signal_reading;

//Functions used only in this file
static void Signal_Timer_Stop(){
	HAL_TIM_Base_Stop_IT(&htim4);
}
static void Signal_Timer_Restart(){
	HAL_TIM_Base_Stop_IT(&htim4);
	__HAL_TIM_SET_COUNTER(&htim4, 0);
	HAL_TIM_Base_Start_IT(&htim4);
}
static uint32_t Signal_Read_Timer(){
	return __HAL_TIM_GET_COUNTER(&htim4);
}
static uint8_t Signal_Read_Pin(){
	uint8_t retry = 0;
	uint8_t level;
	uint8_t i;

	while(retry != SIGNAL_DEBOUNCE_RETRIES){
		retry++;
		level = HAL_GPIO_ReadPin(SIGNAL_GPIO_Port, SIGNAL_Pin);
		i = 0;
		while(i != SIGNAL_DEBOUNCE_SAMPLES){
			if(HAL_GPIO_ReadPin(SIGNAL_GPIO_Port, SIGNAL_Pin) != level){
				if(retry == SIGNAL_DEBOUNCE_RETRIES){
					//If this is a last retry and still can't determine signal level correctly, just return low
					return SIGNAL_LOW;
				}
				break;
			}
			i++;
		}
		if(i == SIGNAL_DEBOUNCE_SAMPLES){
			break;
		}
	}
	return level;
}

//SysTick interrupt, used for detecting signal timeout
void Signal_SysTick_Interrupt(){
	if(signal_reading || signal_o.signal_available){
		if(timeout_passed_ms >= signal_ctr.timeout_ms){
			signal_o.signal_available = false;
			Signal_Gone();
		}else{
			timeout_passed_ms++;
		}
	}
}
void Signal_Timer_Interrupt(){
	signal_reading = false;
}
//Signal pin interrupt
void Signal_Pin_Interrupt(){
	if(Signal_Read_Pin() == SIGNAL_HIGH){
		//Start measuring signal length
		Signal_Timer_Restart();
		timeout_passed_ms = 0;
		signal_reading = true;
	}else if(signal_reading){
		float length_ms  = (float)((uint32_t)Signal_Read_Timer() + (uint32_t)1) / 24000.0f;
		Signal_Timer_Stop();
		signal_reading = false;
		signal_o.signal_available = true;
		signal_o.length_ms = length_ms;
		Signal_Received();
	}
}
