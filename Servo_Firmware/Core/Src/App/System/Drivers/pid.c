#include "main.h"
#include "pid.h"

extern TIM_HandleTypeDef htim3;

//Control/status variables
pid_ctr_t pid_ctr;
//Input data
pid_i_t pid_i;
//Output data
pid_o_t pid_o;

//Working variables
static float last_input;
static float output_sum;

//Functions used only in this file
static void PID_Timer_Stop(){
	HAL_TIM_Base_Stop_IT(&htim3);
}
static void PID_Timer_Restart(){
	HAL_TIM_Base_Stop_IT(&htim3);
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	HAL_TIM_Base_Start_IT(&htim3);
}
static void PID_Compute(){
	float input = pid_i.input;
	float error = pid_i.setpoint - input;
	float input_d = (input - last_input);
	float output = 0;

	//Select parameters for range
	float kp, ki, kd;
	if(error >= pid_ctr.split_1){
		kp = pid_ctr.kp_1;
		ki = pid_ctr.ki_1;
		kd = pid_ctr.kd_1;
	}else if((error < pid_ctr.split_1) && (error >= pid_ctr.split_2)){
		kp = pid_ctr.kp_2;
		ki = pid_ctr.ki_2;
		kd = pid_ctr.kd_2;
	}else{
		kp = pid_ctr.kp_3;
		ki = pid_ctr.ki_3;
		kd = pid_ctr.kd_3;
	}

	//Calculate output
	output_sum += ki * error;
	if(pid_ctr.on_e_m == PID_ON_M){
		output_sum -= kp * input_d;
	}

	if(output_sum > 1000.0f){
		output_sum = 1000.0f;
	}else if(output_sum < -1000.0f){
		output_sum = -1000.0f;
	}

	if(pid_ctr.on_e_m == PID_ON_E){
		output = kp * error;
	}

	output += output_sum - kd * input_d;
	if(output > 1000.0f){
		output = 1000.0f;
	}
	else if(output < -1000.0f){
		output = -1000.0f;
	}

	//Store
	pid_o.output = output;
	last_input = input;
}

//Timer interrupt
void PID_Timer_Interrupt(){
	PID_Compute();
	PID_Ready(pid_o.output);
}

//Returns sampling time in milliseconds
uint16_t PID_Get_Sampling_Time(){
	return (__HAL_TIM_GET_AUTORELOAD(&htim3) / 100);
}
//Sets PID sampling time
void PID_Set_Sampling_Time(uint16_t ms){
	__HAL_TIM_SET_AUTORELOAD(&htim3, ms * 100);
}
//Starts PID. Before calling this, correct sampling time should be set
void PID_Start(){
	if(!pid_o.running){
		last_input = pid_i.input;
		output_sum = 0;
		PID_Timer_Restart();
		pid_o.running = true;
	}
}
//Stops PID
void PID_Stop(){
	if(pid_o.running){
		PID_Timer_Stop();
		PID_Ready(0);
		pid_o.running = false;
	}
}
