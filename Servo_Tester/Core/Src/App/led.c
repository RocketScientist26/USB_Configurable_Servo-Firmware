#include "main.h"
#include "led.h"

void Led_Set(uint8_t state){
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, state);
}