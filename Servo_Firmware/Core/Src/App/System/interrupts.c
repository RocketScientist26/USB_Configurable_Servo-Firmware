#include "main.h"
#include "interrupts.h"
#include "Drivers/potentiometer.h"
#include "Drivers/pid.h"
#include "Drivers/signal.h"
#include "Drivers/motor.h"
#include "Drivers/led.h"

//ADC Conversation complete interrupt
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	if(hadc->Instance == ADC1){
        Potentiometer_ADC_Interrupt();
    }
}

//Timer period elapsed interrupts
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM3){
		PID_Timer_Interrupt();
	}
	else if(htim->Instance == TIM4){
		Signal_Timer_Interrupt();
	}
}

//GPIO interrupts
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == SIGNAL_Pin){
		Signal_Pin_Interrupt();
	}
}

//Interrupt function called in every millisecond by system timer
void Interrupts_SysTick(){
	Signal_SysTick_Interrupt();
	Motor_SysTick_Interrupt();
	LED_SysTick_Interrupt();
}
