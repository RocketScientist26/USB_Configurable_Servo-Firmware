#include "main.h"
#include "general.h"
#include "settings.h"
#include "led.h"
#include "pid.h"
#include "motor.h"
#include "flash.h"

extern CRC_HandleTypeDef hcrc;

extern uint16_t potentiometer_min;
extern uint16_t potentiometer_max;
extern uint16_t motor_max_power;
extern uint8_t motor_invert;
extern uint8_t signal_ignore;
extern float signal_min;
extern float signal_max;
extern uint32_t signal_timeout;
extern uint8_t led_mode;
extern uint8_t pid_on;
extern float pid_kp_1;
extern float pid_ki_1;
extern float pid_kd_1;
extern float pid_kp_2;
extern float pid_ki_2;
extern float pid_kd_2;
extern float pid_kp_3;
extern float pid_ki_3;
extern float pid_kd_3;
extern uint16_t pid_split_1;
extern uint16_t pid_split_2;
extern uint8_t pid_sampling_time;

uint8_t settings[SETTINGS_TOTAL_LENGTH];

void Settings_Read(){
	//Read
	Flash_Read(settings, SETTINGS_FLASH_ADDRESS, SETTINGS_TOTAL_LENGTH);
	//Get CRC
	uint32_t crc = 0;
	General_Copy_32_Bit((uint32_t)&crc, (uint32_t)&settings[SETTINGS_TOTAL_LENGTH - 4]);
	//If CRC is correct
	if(HAL_CRC_Calculate(&hcrc, (uint32_t *)settings, (SETTINGS_TOTAL_LENGTH / 4) - 1) == crc){
		//Parse
		General_Copy_16_Bit((uint32_t)&potentiometer_min, (uint32_t)&settings[0]);
		General_Copy_16_Bit((uint32_t)&potentiometer_max, (uint32_t)&settings[2]);
		General_Copy_16_Bit((uint32_t)&motor_max_power, (uint32_t)&settings[4]);
		motor_invert = settings[6];
		signal_ignore = settings[7];
		General_Copy_32_Bit((uint32_t)&signal_min, (uint32_t)&settings[8]);
		General_Copy_32_Bit((uint32_t)&signal_max, (uint32_t)&settings[12]);
		General_Copy_32_Bit((uint32_t)&signal_timeout, (uint32_t)&settings[16]);
		led_mode = settings[20];
		pid_on = settings[21];
		General_Copy_32_Bit((uint32_t)&pid_kp_1, (uint32_t)&settings[22]);
		General_Copy_32_Bit((uint32_t)&pid_ki_1, (uint32_t)&settings[26]);
		General_Copy_32_Bit((uint32_t)&pid_kd_1, (uint32_t)&settings[30]);
		General_Copy_32_Bit((uint32_t)&pid_kp_2, (uint32_t)&settings[34]);
		General_Copy_32_Bit((uint32_t)&pid_ki_2, (uint32_t)&settings[38]);
		General_Copy_32_Bit((uint32_t)&pid_kd_2, (uint32_t)&settings[42]);
		General_Copy_32_Bit((uint32_t)&pid_kp_3, (uint32_t)&settings[46]);
		General_Copy_32_Bit((uint32_t)&pid_ki_3, (uint32_t)&settings[50]);
		General_Copy_32_Bit((uint32_t)&pid_kd_3, (uint32_t)&settings[54]);
		General_Copy_16_Bit((uint32_t)&pid_split_1, (uint32_t)&settings[58]);
		General_Copy_16_Bit((uint32_t)&pid_split_2, (uint32_t)&settings[60]);
		pid_sampling_time = settings[62];

		//Verify
		if((potentiometer_min < SETTINGS_DEF_POTENTIOMETER_MIN_MIN)||(potentiometer_min > SETTINGS_DEF_POTENTIOMETER_MIN_MAX)){
			potentiometer_min = SETTINGS_DEF_POTENTIOMETER_MIN_DEF;
		}
		if((potentiometer_max < SETTINGS_DEF_POTENTIOMETER_MAX_MIN)||(potentiometer_max > SETTINGS_DEF_POTENTIOMETER_MAX_MAX)){
			potentiometer_max = SETTINGS_DEF_POTENTIOMETER_MAX_DEF;
		}
		if((motor_max_power < SETTINGS_DEF_MOTOR_MAX_POWER_MIN)||(motor_max_power > SETTINGS_DEF_MOTOR_MAX_POWER_MAX)){
			motor_max_power = SETTINGS_DEF_MOTOR_MAX_POWER_DEF;
		}
		if((motor_invert != MOTOR_NORMAL) && (motor_invert != MOTOR_INVERT)){
			motor_invert = SETTINGS_DEF_MOTOR_INVERT_DEF;
		}
		if((signal_ignore != GENERAL_TRUE) && (signal_ignore != GENERAL_FALSE)){
			signal_ignore = SETTINGS_DEF_SIGNAL_IGNORE_DEF;
		}
		if((signal_min < SETTINGS_DEF_SIGNAL_MIN_MIN)||(signal_min> SETTINGS_DEF_SIGNAL_MIN_MAX)){
			signal_min = SETTINGS_DEF_SIGNAL_MIN_DEF;
		}
		if((signal_max < SETTINGS_DEF_SIGNAL_MAX_MIN)||(signal_max > SETTINGS_DEF_SIGNAL_MAX_MAX)){
			signal_max = SETTINGS_DEF_SIGNAL_MAX_DEF;
		}
		if((signal_max - signal_min) < (SETTINGS_DEF_SIGNAL_MAX_MIN - SETTINGS_DEF_SIGNAL_MIN_MIN)){
			signal_min = SETTINGS_DEF_SIGNAL_MIN_DEF;
			signal_max = SETTINGS_DEF_SIGNAL_MAX_DEF;
		}
		if((signal_timeout < SETTINGS_DEF_SIGNAL_TIMEOUT_MIN)||(signal_timeout > SETTINGS_DEF_SIGNAL_TIMEOUT_MAX)){
			signal_timeout = SETTINGS_DEF_SIGNAL_TIMEOUT_DEF;
		}
		if((led_mode != LED_MODE_OFF) && (led_mode != LED_MODE_POWER) && (led_mode != LED_MODE_SIGNAL) && (led_mode != LED_MODE_POSITION_CHANGE)){
			led_mode = SETTINGS_DEF_LED_MODE_DEF;
		}
		if((pid_on != PID_ON_M) && (pid_on != PID_ON_E)){
			pid_on = SETTINGS_DEF_PID_ON_DEF;
		}
		if((pid_kp_1 < SETTINGS_DEF_PID_KP_MIN)||(pid_kp_1 > SETTINGS_DEF_PID_KP_MAX)){
			pid_kp_1 = SETTINGS_DEF_PID_KP_DEF;
		}
		if((pid_ki_1 < SETTINGS_DEF_PID_KI_MIN)||(pid_ki_1 > SETTINGS_DEF_PID_KI_MAX)){
			pid_ki_1 = SETTINGS_DEF_PID_KI_DEF;
		}
		if((pid_kd_1 < SETTINGS_DEF_PID_KD_MIN)||(pid_kd_1 > SETTINGS_DEF_PID_KD_MAX)){
			pid_kd_1 = SETTINGS_DEF_PID_KD_DEF;
		}
		if((pid_kp_2 < SETTINGS_DEF_PID_KP_MIN)||(pid_kp_2 > SETTINGS_DEF_PID_KP_MAX)){
			pid_kp_2 = SETTINGS_DEF_PID_KP_DEF;
		}
		if((pid_ki_2 < SETTINGS_DEF_PID_KI_MIN)||(pid_ki_2 > SETTINGS_DEF_PID_KI_MAX)){
			pid_ki_2 = SETTINGS_DEF_PID_KI_DEF;
		}
		if((pid_kd_2 < SETTINGS_DEF_PID_KD_MIN)||(pid_kd_2 > SETTINGS_DEF_PID_KD_MAX)){
			pid_kd_2 = SETTINGS_DEF_PID_KD_DEF;
		}
		if((pid_kp_3 < SETTINGS_DEF_PID_KP_MIN)||(pid_kp_3 > SETTINGS_DEF_PID_KP_MAX)){
			pid_kp_3 = SETTINGS_DEF_PID_KP_DEF;
		}
		if((pid_ki_3 < SETTINGS_DEF_PID_KI_MIN)||(pid_ki_3 > SETTINGS_DEF_PID_KI_MAX)){
			pid_ki_3 = SETTINGS_DEF_PID_KI_DEF;
		}
		if((pid_kd_3 < SETTINGS_DEF_PID_KD_MIN)||(pid_kd_3 > SETTINGS_DEF_PID_KD_MAX)){
			pid_kd_3 = SETTINGS_DEF_PID_KD_DEF;
		}
		if((pid_split_1 < SETTINGS_DEF_PID_SPLIT_1_MIN)||(pid_split_1 > SETTINGS_DEF_PID_SPLIT_1_MAX)){
			pid_split_1 = SETTINGS_DEF_PID_SPLIT_1_DEF;
		}
		if((pid_split_2 < SETTINGS_DEF_PID_SPLIT_2_MIN)||(pid_split_2 > SETTINGS_DEF_PID_SPLIT_2_MAX)){
			pid_split_2 = SETTINGS_DEF_PID_SPLIT_2_DEF;
		}
		if((pid_sampling_time < SETTINGS_DEF_PID_SAMPLING_TIME_MIN)||(pid_sampling_time > SETTINGS_DEF_PID_SAMPLING_TIME_MAX)){
			pid_sampling_time = SETTINGS_DEF_PID_SAMPLING_TIME_DEF;
		}
	}else{
		//CRC wrong, fill default values
		potentiometer_min = SETTINGS_DEF_POTENTIOMETER_MIN_DEF;
		potentiometer_max = SETTINGS_DEF_POTENTIOMETER_MAX_DEF;
		motor_max_power = SETTINGS_DEF_MOTOR_MAX_POWER_DEF;
		motor_invert = SETTINGS_DEF_MOTOR_INVERT_DEF;
		signal_ignore = SETTINGS_DEF_SIGNAL_IGNORE_DEF;
		signal_min = SETTINGS_DEF_SIGNAL_MAX_DEF;
		signal_max = SETTINGS_DEF_SIGNAL_MAX_DEF;
		signal_timeout = SETTINGS_DEF_SIGNAL_TIMEOUT_DEF;
		led_mode = SETTINGS_DEF_LED_MODE_DEF;
		pid_on = SETTINGS_DEF_PID_ON_DEF;
		pid_kp_1 = SETTINGS_DEF_PID_KP_DEF;
		pid_ki_1 = SETTINGS_DEF_PID_KI_DEF;
		pid_kd_1 = SETTINGS_DEF_PID_KD_DEF;
		pid_kp_2 = SETTINGS_DEF_PID_KP_DEF;
		pid_ki_2 = SETTINGS_DEF_PID_KI_DEF;
		pid_kd_2 = SETTINGS_DEF_PID_KD_DEF;
		pid_kp_3 = SETTINGS_DEF_PID_KP_DEF;
		pid_ki_3 = SETTINGS_DEF_PID_KI_DEF;
		pid_kd_3 = SETTINGS_DEF_PID_KD_DEF;
		pid_split_1 = SETTINGS_DEF_PID_SPLIT_1_DEF;
		pid_split_2 = SETTINGS_DEF_PID_SPLIT_2_DEF;
		pid_sampling_time = SETTINGS_DEF_PID_SAMPLING_TIME_DEF;
	}
}
void Settings_Write(){
	General_Copy_16_Bit((uint32_t)&settings[0], (uint32_t)&potentiometer_min);
	General_Copy_16_Bit((uint32_t)&settings[2], (uint32_t)&potentiometer_max);
	General_Copy_16_Bit((uint32_t)&settings[4], (uint32_t)&motor_max_power);
	settings[6] = motor_invert;
	settings[7] = signal_ignore;
	General_Copy_32_Bit((uint32_t)&settings[8], (uint32_t)&signal_min);
	General_Copy_32_Bit((uint32_t)&settings[12], (uint32_t)&signal_max);
	General_Copy_32_Bit((uint32_t)&settings[16], (uint32_t)&signal_timeout);
	settings[20] = led_mode;
	settings[21] = pid_on;
	General_Copy_32_Bit((uint32_t)&settings[22], (uint32_t)&pid_kp_1);
	General_Copy_32_Bit((uint32_t)&settings[26], (uint32_t)&pid_ki_1);
	General_Copy_32_Bit((uint32_t)&settings[30], (uint32_t)&pid_kd_1);
	General_Copy_32_Bit((uint32_t)&settings[34], (uint32_t)&pid_kp_2);
	General_Copy_32_Bit((uint32_t)&settings[38], (uint32_t)&pid_ki_2);
	General_Copy_32_Bit((uint32_t)&settings[42], (uint32_t)&pid_kd_2);
	General_Copy_32_Bit((uint32_t)&settings[46], (uint32_t)&pid_kp_3);
	General_Copy_32_Bit((uint32_t)&settings[50], (uint32_t)&pid_ki_3);
	General_Copy_32_Bit((uint32_t)&settings[54], (uint32_t)&pid_kd_3);
	General_Copy_16_Bit((uint32_t)&settings[58], (uint32_t)&pid_split_1);
	General_Copy_16_Bit((uint32_t)&settings[60], (uint32_t)&pid_split_2);
	settings[62] = pid_sampling_time;
	settings[63] = SETTINGS_CRC_PADDING;
	uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)settings, (SETTINGS_TOTAL_LENGTH / 4) - 1);
	General_Copy_32_Bit((uint32_t)&settings[64], (uint32_t)&crc);
	Flash_Write(settings, SETTINGS_FLASH_ADDRESS, SETTINGS_TOTAL_LENGTH);
}
