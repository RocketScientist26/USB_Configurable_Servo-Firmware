#define LED_MODE_OFF 0
#define LED_MODE_POWER 1
#define LED_MODE_SIGNAL 2
#define LED_MODE_POSITION_CHANGE 3

#define LED_POSITION_CHANGE_MODE_TIMEOUT_MS 20
#define LED_POSITION_CHANGE_ACCURACY_PRCNT 0.1f

#define ON 1
#define OFF 0

void LED_SysTick_Interrupt();
void LED_Set(uint8_t state);
void LED_Update();
