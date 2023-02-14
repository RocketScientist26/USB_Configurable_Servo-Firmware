//Data types
typedef struct{
	uint32_t timeout_ms;
}signal_ctr_t;
typedef struct{
	uint32_t signal_available;
	float length_ms;
}signal_o_t;


//Pin level detecting configuration
#define SIGNAL_DEBOUNCE_SAMPLES 5 
#define SIGNAL_DEBOUNCE_RETRIES 5
#define SIGNAL_HIGH GPIO_PIN_SET
#define SIGNAL_LOW GPIO_PIN_RESET

//Functions
void Signal_SysTick_Interrupt();
void Signal_Timer_Interrupt();
void Signal_Pin_Interrupt();
void Signal_Init();

//Functions which should be implemented externally
void Signal_Gone();
void Signal_Received();

//Control/status variables
extern signal_ctr_t signal_ctr;
extern signal_o_t signal_o;
