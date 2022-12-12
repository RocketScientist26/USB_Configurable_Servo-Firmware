#include "main.h"
#include "led.h"

extern uint8_t pid_running;

uint8_t led_mode;
uint8_t led_position_changed = 0;
uint8_t led_systick = 0;

void LED_SysTick_Interrupt(){
	if(led_mode == LED_MODE_POSITION_CHANGE){
		led_systick++;
		if(led_systick >= LED_POSITION_CHANGE_MODE_TIMEOUT_MS){
			LED_Set(OFF);
			led_systick = 0;
		}
	}
}

void LED_Set(uint8_t state){
	if(state == ON){
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	}
}
void LED_Update(){
	switch(led_mode){
		case LED_MODE_OFF:
			LED_Set(OFF);
		break;
		case LED_MODE_POWER:
			LED_Set(ON);
		break;
		case LED_MODE_SIGNAL:
			if(pid_running){
				LED_Set(ON);
			}else{
				LED_Set(OFF);
			}
		break;
		case LED_MODE_POSITION_CHANGE:
			if(led_position_changed){
				led_systick = 0;
				LED_Set(ON);
			}
		break;
	}
	led_position_changed = 0;
}
