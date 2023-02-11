#include "main.h"
#include "led.h"

//Turns LED on or off
void Led_Set_On(bool on){
	if(on){
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, LED_ON);
	}else{
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, LED_OFF);
	}
}