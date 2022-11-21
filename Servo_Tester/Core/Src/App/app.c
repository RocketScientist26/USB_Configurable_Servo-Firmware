#include "main.h"
#include "app.h"
#include "lock.h"
#include "potentiometer.h"
#include "pwm.h"
#include "led.h"

void Potentiometer_Ready(float position){
	if(Lock_State() == LOCK_DISABLED){
		Pwm_Set_Ms(1.0f + ((1.0f / 4095.0f) * position));
	}
}
void Lock_Updated(uint8_t state){
	if(state == LOCK_ENABLED){
		Led_Set(LED_ON);
	}else{
		Led_Set(LED_OFF);
	}
}

void App_Init(){
	Lock_Init();
	Potentiometer_Init();
	Pwm_Init();
	
	if(Lock_State() == LOCK_ENABLED){
		Led_Set(LED_ON);
	}else{
		Led_Set(LED_OFF);
	}
}