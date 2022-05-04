#include "main.h"
#include "interrupts.h"
#include "potentiometer.h"
#include "pid.h"
#include "motor.h"
#include "signal.h"
#include "usb.h"
#include "led.h"

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	if(hadc->Instance == ADC1){
        Potentiometer_ADC_Interrupt();
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM3){
		PID_Timer_Interrupt();
	}
}

void Interrupts_SysTick(){
	Motor_SysTick_Interrupt();
	Signal_SysTick_Interrupt();
	LED_SysTick_Interrupt();
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == SIGNAL_Pin){
		Signal_Interrupt();
	}else if(GPIO_Pin == USB_DET_Pin){
		USB_Det_Interrupt();
	}
}
