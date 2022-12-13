#include "main.h"
#include "pid.h"
#include "motor.h"

extern TIM_HandleTypeDef htim3;

//Configuration
uint8_t pid_sampling_time;
float pid_kp_1;
float pid_ki_1;
float pid_kd_1;
float pid_kp_2;
float pid_ki_2;
float pid_kd_2;
float pid_kp_3;
float pid_ki_3;
float pid_kd_3;
uint16_t pid_split_1;
uint16_t pid_split_2;
uint8_t pid_on;
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
	__HAL_TIM_SET_COUNTER(&htim3, 0);
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

	//Select parameters for range
	float kp, ki, kd;
	if(error >= pid_split_1){
		kp = pid_kp_1;
		ki = pid_ki_1;
		kd = pid_kd_1;
	}else if((error < pid_split_1) && (error >= pid_split_2)){
		kp = pid_kp_2;
		ki = pid_ki_2;
		kd = pid_kd_2;
	}else{
		kp = pid_kp_3;
		ki = pid_ki_3;
		kd = pid_kd_3;
	}

	//Calculate output
	pid_output_sum += ki * error;
	if(pid_on == PID_ON_M){
		pid_output_sum -= kp * input_d;
	}

	if(pid_output_sum > 1000.0f){
		pid_output_sum = 1000.0f;
	}else if(pid_output_sum < -1000.0f){
		pid_output_sum = -1000.0f;
	}

	if(pid_on == PID_ON_E){
		output = kp * error;
	}

	output += pid_output_sum - kd * input_d;
	if(output > 1000.0f){
		output = 1000.0f;
	}
	else if(output < -1000.0f){
		output = -1000.0f;
	}

	//Store
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
