#include "main.h"
#include "usb.h"
#include "motor.h"
#include "pid.h"
#include "general.h"
#include "../../../USB_DEVICE/App/usbd_cdc_if.h"

extern CRC_HandleTypeDef hcrc;

extern uint16_t potentiometer_min;
extern uint16_t potentiometer_max;
extern uint16_t motor_max_power;
extern uint8_t motor_invert;
extern uint8_t signal_ignore;
extern float signal_length;
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
extern uint8_t pid_running;
extern float pid_setpoint;
extern float potentiometer_position;
extern uint8_t led_position_changed;

uint8_t usb_rq = USB_RQ_NONE;
uint8_t usb_rq_sett_revert_flash = 0;
uint8_t usb_rq_stat_motor = 0;
uint8_t usb_rq_stat_phold = 0;

uint8_t usb_present = 0;
uint8_t usb_tx_buffer[64];
uint8_t usb_long_buffer[256];

uint8_t USB_Det_Read(){
	uint8_t level;
	uint8_t d = 0;
	uint8_t i = 0;
	while(d != 1){
		i = 0;
		level = HAL_GPIO_ReadPin(USB_DET_GPIO_Port, USB_DET_Pin);
		while(i != USB_DET_DEBOUNCE_SAMPLES){
			i++;
			if(HAL_GPIO_ReadPin(USB_DET_GPIO_Port, USB_DET_Pin) != level){
				break;
			}
			if(i == USB_DET_DEBOUNCE_SAMPLES){
				d = 1;
			}
		}
	}
	return level;
}
void USB_Det_Interrupt(){
	if(USB_Det_Read() == USB_DET_HIGH){
		usb_present = 1;
	}else{
		usb_present = 0;
	}
	if(pid_running){
		PID_Stop();
	}
}
void USB_Init(){
	if(USB_Det_Read() == USB_DET_HIGH){
		usb_present = 1;
	}else{
		usb_present = 0;
	}
}
void USB_Packet_Received(uint8_t *data, uint32_t length){
	if((data[1] == USB_CMD_CONFIG) && (uint32_t)data[0] == length){
		if(HAL_CRC_Calculate(&hcrc, (uint32_t *)data, (length / 4) - 1) == *(uint32_t *)&data[length - 4]){
			USB_Parse_Config(data);
			usb_rq = USB_RQ_CONF;
		}
	}else if((data[1] == USB_CMD_STATUS) && (length == 64)){
		memcpy(usb_long_buffer, data, 64);
	}
	else if((usb_long_buffer[1] == USB_CMD_STATUS) && (usb_long_buffer[0] == (length + 64))){
		memcpy(&usb_long_buffer[64], data, length);
		if(HAL_CRC_Calculate(&hcrc, (uint32_t *)usb_long_buffer, ((length + 64) / 4) - 1) == *(uint32_t *)&usb_long_buffer[length + 64- 4]){
			USB_Parse_Status(usb_long_buffer);
			usb_rq = USB_RQ_STAT;
		}
		usb_long_buffer[1] = 0xff;
	}else{
		usb_long_buffer[1] = 0xff;
	}
}
void USB_Parse_Config(uint8_t *data){
	usb_rq_sett_revert_flash = data[2];
}
void USB_Send_Config(){
	usb_tx_buffer[0] = USB_TX_CONFIG_LENGTH;
	usb_tx_buffer[1] = USB_CMD_CONFIG;
	*(uint16_t *)&usb_tx_buffer[2] = potentiometer_min;
	*(uint16_t *)&usb_tx_buffer[4] = potentiometer_max;
	usb_tx_buffer[6] = (uint8_t)((uint16_t)motor_max_power / 10);
	usb_tx_buffer[7] = motor_invert;
	usb_tx_buffer[8] = signal_ignore;
	General_Copy_32_Bit((uint32_t)&usb_tx_buffer[9], (uint32_t)&signal_length);
	General_Copy_32_Bit((uint32_t)&usb_tx_buffer[13], (uint32_t)&signal_timeout);
	usb_tx_buffer[17] = led_mode;
	usb_tx_buffer[18] = pid_on;
	General_Copy_32_Bit((uint32_t)&usb_tx_buffer[19], (uint32_t)&pid_kp_1);
	General_Copy_32_Bit((uint32_t)&usb_tx_buffer[23], (uint32_t)&pid_ki_1);
	General_Copy_32_Bit((uint32_t)&usb_tx_buffer[27], (uint32_t)&pid_kd_1);
	General_Copy_32_Bit((uint32_t)&usb_tx_buffer[31], (uint32_t)&pid_kp_1);
	General_Copy_32_Bit((uint32_t)&usb_tx_buffer[35], (uint32_t)&pid_ki_1);
	General_Copy_32_Bit((uint32_t)&usb_tx_buffer[39], (uint32_t)&pid_kd_1);
	General_Copy_32_Bit((uint32_t)&usb_tx_buffer[43], (uint32_t)&pid_kp_1);
	General_Copy_32_Bit((uint32_t)&usb_tx_buffer[47], (uint32_t)&pid_ki_1);
	General_Copy_32_Bit((uint32_t)&usb_tx_buffer[51], (uint32_t)&pid_kd_1);
	General_Copy_16_Bit((uint32_t)&usb_tx_buffer[55], (uint32_t)&pid_split_1);
	General_Copy_16_Bit((uint32_t)&usb_tx_buffer[57], (uint32_t)&pid_split_2);
	usb_tx_buffer[59] = pid_sampling_time;
	uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)&usb_tx_buffer, (USB_TX_CONFIG_LENGTH / 4) - 1);
	General_Copy_32_Bit((uint32_t)&usb_tx_buffer[60], (uint32_t)&crc);
	CDC_Transmit_FS((uint8_t *)&usb_tx_buffer, USB_TX_CONFIG_LENGTH);
}
void USB_Parse_Status(uint8_t *data){
	potentiometer_min = *(uint16_t *)&data[2];
	potentiometer_max = *(uint16_t *)&data[4];
	motor_max_power = (uint16_t)data[6] * 10;
	motor_invert = data[7];
	signal_ignore = data[8];
	General_Copy_32_Bit((uint32_t)&signal_length, (uint32_t)&data[9]);
	General_Copy_32_Bit((uint32_t)&signal_timeout, (uint32_t)&data[13]);
	led_mode = data[17];
	pid_on = data[18];
	General_Copy_32_Bit((uint32_t)&pid_kp_1, (uint32_t)&data[19]);
	General_Copy_32_Bit((uint32_t)&pid_ki_1, (uint32_t)&data[23]);
	General_Copy_32_Bit((uint32_t)&pid_kd_1, (uint32_t)&data[27]);
	General_Copy_32_Bit((uint32_t)&pid_kp_2, (uint32_t)&data[31]);
	General_Copy_32_Bit((uint32_t)&pid_ki_2, (uint32_t)&data[35]);
	General_Copy_32_Bit((uint32_t)&pid_kd_2, (uint32_t)&data[39]);
	General_Copy_32_Bit((uint32_t)&pid_kp_3, (uint32_t)&data[43]);
	General_Copy_32_Bit((uint32_t)&pid_ki_3, (uint32_t)&data[47]);
	General_Copy_32_Bit((uint32_t)&pid_kd_3, (uint32_t)&data[51]);
	General_Copy_16_Bit((uint32_t)&pid_split_1, (uint32_t)&data[55]);
	General_Copy_16_Bit((uint32_t)&pid_split_2, (uint32_t)&data[57]);
	pid_sampling_time = data[59];
	if(PID_Get_Sampling_Time() != pid_sampling_time){
		PID_Set_Sampling_Time(pid_sampling_time);
	}
	float new_pid_setpoint = 0;
	General_Copy_32_Bit((uint32_t)&new_pid_setpoint, (uint32_t)&data[60]);
	if(signal_ignore){
		if(pid_setpoint != new_pid_setpoint){
			led_position_changed = 1;
		}
		pid_setpoint = new_pid_setpoint;
	}
	usb_rq_stat_phold = data[64];
	usb_rq_stat_motor = data[65];
}
void USB_Send_Status(){
	usb_tx_buffer[0] = USB_TX_STATUS_LENGTH;
	usb_tx_buffer[1] = USB_CMD_STATUS;
	usb_tx_buffer[2] = pid_running;
	General_Copy_32_Bit((uint32_t)&usb_tx_buffer[3], (uint32_t)&pid_setpoint);
	General_Copy_32_Bit((uint32_t)&usb_tx_buffer[7], (uint32_t)&potentiometer_position);
	int16_t motor_val = Motor_Get();
	General_Copy_16_Bit((uint32_t)&usb_tx_buffer[11], (uint32_t)&motor_val);
	usb_tx_buffer[13] = USB_CRC_PADDING;
	usb_tx_buffer[14] = USB_CRC_PADDING;
	usb_tx_buffer[15] = USB_CRC_PADDING;
	uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)&usb_tx_buffer, (USB_TX_STATUS_LENGTH / 4) - 1);
	General_Copy_32_Bit((uint32_t)&usb_tx_buffer[16], (uint32_t)&crc);
	CDC_Transmit_FS((uint8_t *)&usb_tx_buffer, USB_TX_STATUS_LENGTH);
}
