#include "main.h" 
#include "motor.h"

extern TIM_HandleTypeDef htim2;

extern uint8_t pid_running;

uint8_t motor_invert = MOTOR_NORMAL;
uint16_t motor_max_power = 1000;
uint8_t motor_running = 0;
uint8_t motor_systick = 0;

void Motor_SysTick_Interrupt(){
	if(!pid_running){
		if(motor_running){
			motor_systick++;
			if(motor_systick == MOTOR_USB_TEST_TIMEOUT_MS){
				Motor_Set(0);
				motor_systick = 0;
			}
		}
	}
}

void Motor_Set(int16_t strength){//-1000 0 1000 Strength
	if(motor_invert == MOTOR_INVERT){
		strength = strength - (2 * strength);
	}

	if(strength == 0){
		TIM2 -> CCR1 = 1000;
		TIM2 -> CCR2 = 1000;
		motor_running = 0;
		motor_systick = 0;
	}else if(strength < 0){
		TIM2 -> CCR1 = 1000;
		TIM2 -> CCR2 = 1000 + (strength / (1000 / motor_max_power));
		motor_running = 1;
	}else if(strength > 0){
		TIM2 -> CCR1 = 1000 - (strength / (1000 / motor_max_power));
		TIM2 -> CCR2 = 1000;
		motor_running = 1;
	}
}
int16_t Motor_Get(){
	if(TIM2 -> CCR1 != 1000){
		return (1000 - (int16_t)(TIM2 -> CCR1)) * (1000 / motor_max_power);
	}else if(TIM2 -> CCR2 != 1000){
		return ((int16_t)(TIM2 -> CCR2) - 1000) * (1000 / motor_max_power);
	}
	return 0;
} 
void Motor_Init(){
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
}