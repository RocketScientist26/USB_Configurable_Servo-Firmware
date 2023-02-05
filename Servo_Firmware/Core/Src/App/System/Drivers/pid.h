//Data types
typedef struct{
	float kp_1;
	float ki_1;
	float kd_1;
	float kp_2;
	float ki_2;
	float kd_2;
	float kp_3;
	float ki_3;
	float kd_3;
	uint16_t split_1;
	uint16_t split_2;
	bool on_e_m;
}pid_ctr_t;

typedef struct{
	float input;
	float setpoint;
}pid_i_t;

typedef struct{
	bool running;
	float output;
}pid_o_t;

//Operation modes
enum{
	PID_ON_E, //Proportional on error
	PID_ON_M //Proportional on measurement
};

//Functions
void PID_SysTick_Interrupt();
void PID_Timer_Interrupt();
uint16_t PID_Get_Sampling_Time();
void PID_Set_Sampling_Time(uint16_t ms);
void PID_Start();
void PID_Stop();

//Function should be implemented externally
void PID_Ready(float output); //called every time PID computation is complete

//Control/status variables
extern pid_ctr_t pid_ctr;
//Input data
extern pid_i_t pid_i;
//Output data
extern pid_o_t pid_o;
