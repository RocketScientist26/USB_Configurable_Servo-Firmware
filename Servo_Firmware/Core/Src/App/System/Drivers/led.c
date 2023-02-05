#include "main.h"
#include "led.h"

//Control/status variables
led_ctr_t led_ctr;
//Input data
led_i_t led_i;
//Output data
led_o_t led_o;

//Working variable, counter for turning off LED in "LED_MODE_POSITION_CHANGE" mode
static uint32_t systick;

//Function used only in this file
static void LED_On(bool on){
	if(on){
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	}
	led_o.on = on;
}

//SysTick interrupt for LED, called in every millisecond, with lowest interrupt priority (highest number)
void LED_SysTick_Interrupt(){
	if(led_ctr.mode == LED_MODE_POSITION_CHANGE){
		systick++;
	}
}

//Should be called from app main loop function to make LED work
void LED_Update(){
	switch(led_ctr.mode){
		case LED_MODE_OFF:
			LED_On(false);
		break;
		case LED_MODE_POWER:
			LED_On(true);
		break;
		case LED_MODE_PID:
			if(led_i.pid_running){
				LED_On(true);
			}else{
				LED_On(false);
			}
		break;
		case LED_MODE_POSITION_CHANGE:
			if(led_i.position_changed){
				led_i.position_changed = false;
				systick = 0;
				LED_On(true);
			}
			if(systick >= LED_POSITION_CHANGE_MODE_TIMEOUT_MS){
				LED_On(false);
				systick = 0;
			}
		break;
	}
}
