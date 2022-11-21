#define LOCK_DEBOUNCE_SAMPLES 10
#define LOCK_ENABLED GPIO_PIN_RESET
#define LOCK_DISABLED GPIO_PIN_SET

void Lock_Updated(uint8_t state);
void Lock_Interrupt();
uint8_t Lock_State();
uint8_t Lock_Read();
void Lock_Init();