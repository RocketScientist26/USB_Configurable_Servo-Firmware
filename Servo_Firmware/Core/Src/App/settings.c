#include "main.h"
#include "System/Drivers/flash.h"
#include "System/Drivers/led.h"
#include "System/Drivers/pid.h"
#include "System/Drivers/motor.h"
#include "System/Drivers/signal.h"
#include "System/Drivers/usb.h"
#include "settings.h"

extern CRC_HandleTypeDef hcrc;

//Global configuration variable
usb_config_t settings_data;

//Default, minimum and maximum variables
static const usb_config_t settings_data_defaults = {
	.potentiometer_min = 128,
	.potentiometer_max = 3967,
	.motor_max_power = 100,
	.motor_invert = false,
	.signal_ignore = true,
	.signal_min = 1.0f,
	.signal_max = 2.0f,
	.signal_timeout = 25,
	.led_mode = LED_MODE_POWER,
	.pid_on_e_m = PID_ON_E,
	.pid_kp_1 = 1.0f,
	.pid_ki_1 = 0,
	.pid_kd_1 = 3.0f,
	.pid_kp_2 = 1.0f,
	.pid_ki_2 = 0,
	.pid_kd_2 = 3.0f,
	.pid_kp_3 = 1.0f,
	.pid_ki_3 = 0,
	.pid_kd_3 = 3.0f,
	.pid_split_steps_1 = 2047,
	.pid_split_steps_2 = 3068,
	.pid_sampling_ms = 5
};
static const usb_config_t settings_data_min = {
	.potentiometer_min = 0,
	.potentiometer_max = 128,
	.motor_max_power = 0,
	.motor_invert = false,
	.signal_ignore = false,
	.signal_min = 0.1f,
	.signal_max = 0.5f,
	.signal_timeout = 6,
	.led_mode = LED_MODE_OFF,
	.pid_on_e_m = PID_ON_E,
	.pid_kp_1 = 0,
	.pid_ki_1 = 0,
	.pid_kd_1 = 0,
	.pid_kp_2 = 0,
	.pid_ki_2 = 0,
	.pid_kd_2 = 0,
	.pid_kp_3 = 0,
	.pid_ki_3 = 0,
	.pid_kd_3 = 0,
	.pid_split_steps_1 = 0,
	.pid_split_steps_2 = 0,
	.pid_sampling_ms = 5
};
static const usb_config_t settings_data_max = {
	.potentiometer_min = 3967,
	.potentiometer_max = 4095,
	.motor_max_power = 100,
	.motor_invert = true,
	.signal_ignore = true,
	.signal_min = 2.1f, //.signal_max - settings_data_min.signal_max - settings_data_min.signal_min
	.signal_max = 2.7f,
	.signal_timeout = 200,
	.led_mode = LED_MODE_POSITION_CHANGE,
	.pid_on_e_m = PID_ON_M,
	.pid_kp_1 = 100.0f,
	.pid_ki_1 = 100.0f,
	.pid_kd_1 = 100.0f,
	.pid_kp_2 = 100.0f,
	.pid_ki_2 = 100.0f,
	.pid_kd_2 = 100.0f,
	.pid_kp_3 = 100.0f,
	.pid_ki_3 = 100.0f,
	.pid_kd_3 = 100.0f,
	.pid_split_steps_1 = 4095,
	.pid_split_steps_2 = 4095,
	.pid_sampling_ms = 100
};

//Buffer for data + CRC padding bytes + CRC for writing to/reading from flash
static uint8_t settings_data_buff[sizeof(usb_config_t) + (sizeof(uint32_t) - 1) + sizeof(uint32_t)]; //Data size + maximum amount of CRC padding bytes + CRC size

//Function used only in this file, simple data copy loop for unaligned memory access
static void _memcpy(uint32_t dst_addr, uint32_t src_addr, uint32_t length){
	uint32_t i = 0;
	while(i != length){
		*(uint8_t *)(dst_addr + i) = *(uint8_t *)(src_addr + i);
		i++;
	}
}

//Reads settings to "settings_data" global variable from flash. If does not exist in flash or wrong data, writes default values
void Settings_Read(){
	//Calculate CRC padding bytes count
	uint8_t i = 0;
	while((sizeof(usb_config_t) + i) % sizeof(uint32_t)){
		i++;
	}

	//Read from flash
	Flash_Read(&settings_data_buff[0], SETTINGS_FLASH_ADDRESS, sizeof(usb_config_t) + i + sizeof(uint32_t));

	//Get CRCs
	uint32_t actual_crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)&settings_data_buff[0], (sizeof(usb_config_t) + i) / sizeof(uint32_t));
	uint32_t stored_crc = 0;
	_memcpy((uint32_t)&stored_crc, (uint32_t)&settings_data_buff[sizeof(usb_config_t) + i], sizeof(uint32_t));

	//Check if CRC is correct
	if(actual_crc != stored_crc){
		settings_data = settings_data_defaults;
		return;
	}

	//Copy to global variable from flash buffer
	_memcpy((uint32_t)&settings_data, (uint32_t)&settings_data_buff[0], sizeof(usb_config_t));

	//Verify that all configuration values are in range
	if(
		(settings_data.potentiometer_min < settings_data_min.potentiometer_min) || (settings_data.potentiometer_min > settings_data_max.potentiometer_min) ||
		(settings_data.potentiometer_max < settings_data_min.potentiometer_max) || (settings_data.potentiometer_max > settings_data_max.potentiometer_max) ||
		(settings_data.motor_max_power < settings_data_min.motor_max_power) || (settings_data.motor_max_power > settings_data_max.motor_max_power) ||
		(settings_data.motor_invert < settings_data_min.motor_invert) || (settings_data.motor_invert > settings_data_max.motor_invert) ||
		(settings_data.signal_ignore < settings_data_min.signal_ignore) || (settings_data.motor_invert > settings_data_max.signal_ignore) ||
		(settings_data.signal_min < settings_data_min.signal_min) || (settings_data.signal_min > settings_data_max.signal_min) ||
		(settings_data.signal_max < settings_data_min.signal_max) || (settings_data.signal_max > settings_data_max.signal_max) ||
		(settings_data.signal_timeout < settings_data_min.signal_timeout) || (settings_data.signal_timeout > settings_data_max.signal_timeout) ||
		(settings_data.led_mode < settings_data_min.led_mode) || (settings_data.led_mode > settings_data_max.led_mode) ||
		(settings_data.pid_on_e_m < settings_data_min.pid_on_e_m) || (settings_data.pid_on_e_m > settings_data_max.pid_on_e_m) ||
		(settings_data.pid_kp_1 < settings_data_min.pid_kp_1) || (settings_data.pid_kp_1 > settings_data_max.pid_kp_1) ||
		(settings_data.pid_ki_1 < settings_data_min.pid_ki_1) || (settings_data.pid_ki_1 > settings_data_max.pid_ki_1) ||
		(settings_data.pid_kd_1 < settings_data_min.pid_kd_1) || (settings_data.pid_kd_1 > settings_data_max.pid_kd_1) ||
		(settings_data.pid_kp_2 < settings_data_min.pid_kp_2) || (settings_data.pid_kp_2 > settings_data_max.pid_kp_2) ||
		(settings_data.pid_ki_2 < settings_data_min.pid_ki_2) || (settings_data.pid_ki_2 > settings_data_max.pid_ki_2) ||
		(settings_data.pid_kd_2 < settings_data_min.pid_kd_2) || (settings_data.pid_kd_2 > settings_data_max.pid_kd_2) ||
		(settings_data.pid_kp_3 < settings_data_min.pid_kp_3) || (settings_data.pid_kp_3 > settings_data_max.pid_kp_3) ||
		(settings_data.pid_ki_3 < settings_data_min.pid_ki_3) || (settings_data.pid_ki_3 > settings_data_max.pid_ki_3) ||
		(settings_data.pid_kd_3 < settings_data_min.pid_kd_3) || (settings_data.pid_kd_3 > settings_data_max.pid_kd_3) ||
		(settings_data.pid_split_steps_1 < settings_data_min.pid_split_steps_1) || (settings_data.pid_split_steps_1 > settings_data_max.pid_split_steps_1) ||
		(settings_data.pid_split_steps_2 < settings_data_min.pid_split_steps_2) || (settings_data.pid_split_steps_2 > settings_data_max.pid_split_steps_2) ||
		(settings_data.pid_sampling_ms < settings_data_min.pid_sampling_ms) || (settings_data.pid_sampling_ms > settings_data_max.pid_sampling_ms)
	){
		//If anything is aout of range, copy defaults
		settings_data = settings_data_defaults;
	}
}

//Copies values from "settings_data" to functions configurations variables
void Settings_Apply(){
	motor_ctr.max_power = settings_data.motor_max_power * 10;
	motor_ctr.invert = settings_data.motor_invert;
	signal_ctr.timeout_ms = settings_data.signal_timeout;
	led_ctr.mode = settings_data.led_mode;
	pid_ctr.on_e_m = settings_data.pid_on_e_m;
	pid_ctr.kp_1 = settings_data.pid_kp_1;
	pid_ctr.ki_1 = settings_data.pid_ki_1;
	pid_ctr.kd_1 = settings_data.pid_kd_1;
	pid_ctr.kp_2 = settings_data.pid_kp_2;
	pid_ctr.ki_2 = settings_data.pid_ki_2;
	pid_ctr.kd_2 = settings_data.pid_kd_2;
	pid_ctr.kp_3 = settings_data.pid_kp_3;
	pid_ctr.ki_3 = settings_data.pid_ki_3;
	pid_ctr.kd_3 = settings_data.pid_kd_3;
	pid_ctr.split_1 = settings_data.pid_split_steps_1;
	pid_ctr.split_2 = settings_data.pid_split_steps_2;
	PID_Set_Sampling_Time(settings_data.pid_sampling_ms);
}

//Writes "settings_data" global variable to flash
void Settings_Write(){
	//Copy data to temporary buffer
	_memcpy((uint32_t)&settings_data_buff[0], (uint32_t)&settings_data, sizeof(usb_config_t));
	//Append CRC padding symbols if necessary
	uint8_t i = 0;
	while((sizeof(usb_config_t) + i) % sizeof(uint32_t)){
		settings_data_buff[sizeof(usb_config_t) + i] = USB_CRC_PADDING_SYMBOL;
		i++;
	}

	//Append CRC
	uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)&settings_data_buff[0], (sizeof(usb_config_t) + i) / sizeof(uint32_t));
	_memcpy((uint32_t)&settings_data_buff[sizeof(usb_config_t) + i], (uint32_t)&crc, sizeof(uint32_t));
	
	//Write to flash
	Flash_Write(&settings_data_buff[0], SETTINGS_FLASH_ADDRESS, sizeof(usb_config_t) + i + sizeof(uint32_t));
}
