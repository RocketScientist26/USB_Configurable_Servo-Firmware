#include "main.h" 
#include "motor.h"

extern TIM_HandleTypeDef htim2;

//Control/status variables
motor_ctr_t motor_ctr;
//Input data
motor_i_t motor_i;
//Input data
motor_o_t motor_o;

//Working variable, counter for motor turn off in test mode
static uint32_t systick;

//SysTick interrupt for motor, called in every millisecond, with lowest interrupt priority (highest number)
void Motor_SysTick_Interrupt(){
	if(!motor_i.pid_running){
		if(motor_o.running){
			systick++;
			if(systick == MOTOR_TEST_TIMEOUT_MS){
				Motor_Set(0);
				systick = 0;
			}
		}
	}
}

//Sets motor rotation direction and power, value from -1000 to 1000. Zero means off
void Motor_Set(int16_t strength){
	if(motor_ctr.invert == MOTOR_INVERT){
		strength = strength - (2 * strength);
	}

	systick = 0;
	if(strength == 0){
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 1000);
		motor_o.running = false;
	}else if(strength < 0){
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 1000 + (strength / (1000 / motor_ctr.max_power)));
		motor_o.running = true;
	}else if(strength > 0){
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000 - (strength / (1000 / motor_ctr.max_power)));
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 1000);
		motor_o.running = true;
	}
}

//Returns current motor rotation direction and power, value from -1000 to 1000
int16_t Motor_Get(){
	if(__HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_1) != 1000){
		return (1000 - (int16_t)__HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_1)) * (1000 / motor_ctr.max_power);
	}else if(TIM2 -> CCR2 != 1000){
		return ((int16_t)__HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_2) - 1000) * (1000 / motor_ctr.max_power);
	}
	return 0;
}

//Init function, should be called once at start
void Motor_Init(){
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
}
