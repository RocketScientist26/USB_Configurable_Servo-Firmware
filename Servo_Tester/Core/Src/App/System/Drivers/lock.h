//Pin level determination configuration
#define LOCK_DEBOUNCE_RETRIES 5
#define LOCK_DEBOUNCE_SAMPLES 10

//Pin states which means lock is enabled or disabled
#define LOCK_ENABLED GPIO_PIN_RESET
#define LOCK_DISABLED GPIO_PIN_SET

//Functions
void Lock_Interrupt();

//This functions should be implemented in external file
void Lock_Updated(bool enabled);
