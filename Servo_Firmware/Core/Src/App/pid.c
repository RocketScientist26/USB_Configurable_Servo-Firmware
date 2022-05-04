#include "main.h"
#include "pid.h"
#include "motor.h"

extern TIM_HandleTypeDef htim3;

//Configuration
uint8_t pid_sampling_time = 20;
float pid_kp_1 = 2;
float pid_ki_1 = 0;
float pid_kd_1 = 12;
float pid_kp_2 = 2;
float pid_ki_2 = 0;
float pid_kd_2 = 12;
float pid_kp_3 = 2;
float pid_ki_3 = 0;
float pid_kd_3 = 12;
uint16_t pid_split_1 = 2048;
uint16_t pid_split_2 = 3068;
uint8_t pid_on = PID_ON_E;
//Data input/output
uint8_t pid_running = 0;
float pid_input = 0;
float pid_output = 0;
float pid_setpoint = 2047;
//Runtime variables
float pid_last_input = 0;
float pid_output_sum = 0;

void PID_Timer_Stop(){
	HAL_TIM_Base_Stop_IT(&htim3);
}
void PID_Timer_Restart(){
	HAL_TIM_Base_Stop_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim3);
}
void PID_Timer_Interrupt(){
	PID_Compute();
	PID_Ready(pid_output);
	PID_Timer_Restart();
}

uint16_t PID_Get_Sampling_Time(){
	return (__HAL_TIM_GET_AUTORELOAD(&htim3) / 100);
}
void PID_Set_Sampling_Time(uint16_t ms){
	__HAL_TIM_SET_AUTORELOAD(&htim3, ms * 100);
}
void PID_Compute(){
	float input = pid_input;
	float error = pid_setpoint - input;
	float input_d = (input - pid_last_input);
	float output = 0;

	if(error >= pid_split_1){
		pid_output_sum += pid_ki_1 * error;
		if(pid_on == PID_ON_M){
			pid_output_sum -= pid_kp_1 * input_d;
		}

		if(pid_output_sum > 1000.0f){
			pid_output_sum = 1000.0f;
		}else if(pid_output_sum < -1000.0f){
			pid_output_sum = -1000.0f;
		}

		if(pid_on == PID_ON_E){
			output = pid_kp_1 * error;
		}

		output += pid_output_sum - pid_kd_1 * input_d;
		if(output > 1000.0f){
			output = 1000.0f;
		}
		else if(output < -1000.0f){
			output = -1000.0f;
		}
	}else if((error < pid_split_1) && (error >= pid_split_2)){
		pid_output_sum += pid_ki_2 * error;
		if(pid_on == PID_ON_M){
			pid_output_sum -= pid_kp_2 * input_d;
		}

		if(pid_output_sum > 1000.0f){
			pid_output_sum = 1000.0f;
		}else if(pid_output_sum < -1000.0f){
			pid_output_sum = -1000.0f;
		}

		if(pid_on == PID_ON_E){
			output = pid_kp_2 * error;
		}

		output += pid_output_sum - pid_kd_2 * input_d;
		if(output > 1000.0f){
			output = 1000.0f;
		}
		else if(output < -1000.0f){
			output = -1000.0f;
		}
	}else{
		pid_output_sum += pid_ki_3 * error;
		if(pid_on == PID_ON_M){
			pid_output_sum -= pid_kp_3 * input_d;
		}

		if(pid_output_sum > 1000.0f){
			pid_output_sum = 1000.0f;
		}else if(pid_output_sum < -1000.0f){
			pid_output_sum = -1000.0f;
		}

		if(pid_on == PID_ON_E){
			output = pid_kp_3 * error;
		}

		output += pid_output_sum - pid_kd_3 * input_d;
		if(output > 1000.0f){
			output = 1000.0f;
		}
		else if(output < -1000.0f){
			output = -1000.0f;
		}
	}

	pid_output = output;
	pid_last_input = input;
}
void PID_Start(){
	pid_last_input = pid_input;
	pid_output_sum = 0;
    PID_Set_Sampling_Time(pid_sampling_time);
	PID_Timer_Restart();
	pid_running = 1;
}
void PID_Stop(){
	PID_Timer_Stop();
	PID_Ready(0);
	pid_running = 0;
}
