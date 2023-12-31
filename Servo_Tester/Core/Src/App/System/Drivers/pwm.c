#include "main.h"
#include "pwm.h"

extern TIM_HandleTypeDef htim4;

//Sets PWM output signal length in milliseconds
void Pwm_Set_Ms(float ms){
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, (uint16_t)(ms * (47999.0f / 2.0f)));
}

//Starts PWM output
void Pwm_Init(){
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
}