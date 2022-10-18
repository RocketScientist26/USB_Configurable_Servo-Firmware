#define SETTINGS_FLASH_ADDRESS 0x0800FC00
#define SETTINGS_TOTAL_LENGTH (59 + 1 + 4)//Data, CRC padding, CRC
#define SETTINGS_CRC_PADDING 0

#define SETTINGS_DEF_POTENTIOMETER_MIN_MIN 0
#define SETTINGS_DEF_POTENTIOMETER_MIN_MAX 3967
#define SETTINGS_DEF_POTENTIOMETER_MIN_DEF 128
#define SETTINGS_DEF_POTENTIOMETER_MAX_MIN 128
#define SETTINGS_DEF_POTENTIOMETER_MAX_MAX 4095
#define SETTINGS_DEF_POTENTIOMETER_MAX_DEF 3967
#define SETTINGS_DEF_MOTOR_MAX_POWER_MIN 0
#define SETTINGS_DEF_MOTOR_MAX_POWER_MAX 1000
#define SETTINGS_DEF_MOTOR_MAX_POWER_DEF 1000
#define SETTINGS_DEF_MOTOR_INVERT_DEF MOTOR_NORMAL
#define SETTINGS_DEF_SIGNAL_IGNORE_DEF GENERAL_TRUE
#define SETTINGS_DEF_SIGNAL_LENGTH_MIN 2.0
#define SETTINGS_DEF_SIGNAL_LENGTH_MAX 5.0f
#define SETTINGS_DEF_SIGNAL_LENGTH_DEF 2.0f
#define SETTINGS_DEF_SIGNAL_TIMEOUT_MIN 6
#define SETTINGS_DEF_SIGNAL_TIMEOUT_MAX 200
#define SETTINGS_DEF_SIGNAL_TIMEOUT_DEF 25
#define SETTINGS_DEF_LED_MODE_DEF LED_MODE_POWER
#define SETTINGS_DEF_PID_ON_DEF PID_ON_E
#define SETTINGS_DEF_PID_KP_MIN 0.0f
#define SETTINGS_DEF_PID_KP_MAX 100.0f
#define SETTINGS_DEF_PID_KP_DEF 1.0f
#define SETTINGS_DEF_PID_KI_MIN 0.0f
#define SETTINGS_DEF_PID_KI_MAX 100.0f
#define SETTINGS_DEF_PID_KI_DEF 0.0f
#define SETTINGS_DEF_PID_KD_MIN 0.0f
#define SETTINGS_DEF_PID_KD_MAX 100.0f
#define SETTINGS_DEF_PID_KD_DEF 3.0f
#define SETTINGS_DEF_PID_SPLIT_1_MIN 0//TBD
#define SETTINGS_DEF_PID_SPLIT_1_MAX 4095
#define SETTINGS_DEF_PID_SPLIT_1_DEF 2047
#define SETTINGS_DEF_PID_SPLIT_2_MIN 0//TBD
#define SETTINGS_DEF_PID_SPLIT_2_MAX 4095
#define SETTINGS_DEF_PID_SPLIT_2_DEF 3068
#define SETTINGS_DEF_PID_SAMPLING_TIME_MIN 5.0f
#define SETTINGS_DEF_PID_SAMPLING_TIME_MAX 100.0f
#define SETTINGS_DEF_PID_SAMPLING_TIME_DEF 5.0f

void Settings_Read();
void Settings_Write();
