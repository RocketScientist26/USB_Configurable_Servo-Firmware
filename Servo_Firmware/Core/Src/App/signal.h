#define SIGNAL_DEBOUNCE_SAMPLES 5 
#define SIGNAL_HIGH GPIO_PIN_SET

void Signal_SysTick_Interrupt();
void Signal_Interrupt();
uint32_t Signal_Read_Timer();
void Signal_Timer_Reset();
uint8_t Signal_Read();
void Signal_Init();