#define PID_ON_E 0//Proportional on measurement
#define PID_ON_M 1//Proportional on error

void PID_SysTick_Interrupt();
void PID_Timer_Stop();
void PID_Timer_Restart();
void PID_Timer_Interrupt();
uint16_t PID_Get_Sampling_Time();
void PID_Set_Sampling_Time(uint16_t ms);
void PID_Ready(float output);
void PID_Compute();
void PID_Start();
void PID_Stop();
