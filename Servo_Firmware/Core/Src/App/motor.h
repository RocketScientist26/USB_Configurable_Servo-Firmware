#define MOTOR_USB_TEST_TIMEOUT_MS 50
#define MOTOR_NORMAL 0
#define MOTOR_INVERT 1

void Motor_SysTick_Interrupt();
void Motor_Set(int16_t strength);
int16_t Motor_Get();
void Motor_Init();
