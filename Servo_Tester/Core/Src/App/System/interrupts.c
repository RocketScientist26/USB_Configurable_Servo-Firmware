#include "main.h"
#include "Drivers/potentiometer.h"
#include "Drivers/lock.h"

//ADC Conversation complete interrupt
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	if(hadc->Instance == ADC1){
        Potentiometer_ADC_Interrupt();
    }
}

//GPIO interrupts
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == LOCK_IN_Pin){
		Lock_Interrupt();
	}
}