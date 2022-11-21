#include "main.h"
#include "potentiometer.h"
#include "lock.h"

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	if(hadc->Instance == ADC1){
        Potentiometer_ADC_Interrupt();
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == LOCK_IN_Pin){
		Lock_Interrupt();
	}
}