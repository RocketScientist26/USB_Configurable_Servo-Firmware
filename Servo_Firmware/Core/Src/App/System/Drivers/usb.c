#include "main.h"
#include "../../../../../USB_DEVICE/App/usbd_cdc_if.h"
#include "usb.h"

extern CRC_HandleTypeDef hcrc;
extern USBD_HandleTypeDef hUsbDeviceFS;

//Buffers
static uint8_t tx_buffer[255];
static uint8_t rx_buffer[255];

//Output status data from this c/h pair
usb_o_t usb_o;
//Received status and configuration data pointers from host
usb_config_t usb_rx_config;
usb_rx_status_t usb_rx_status;

//Function used only in this file, simple data copy loop for unaligned memory access
static void _memcpy(uint32_t dst_addr, uint32_t src_addr, uint32_t length){
	uint32_t i = 0;
	while(i != length){
		*(uint8_t *)(dst_addr + i) = *(uint8_t *)(src_addr + i);
		i++;
	}
}

//Called from USB packet received interrupt
void USB_Packet_Received(uint8_t *data, uint32_t length){
	/*
		If size of expecting packet is more than 64, this function will be called multiply times
		each time "*data" will contain 64 or less bytes, "length" will indicate how many bytes
		can be read from "*data".
	*/

	//Ignore any packet if previous packet request is still pending or size could not be correct
	if((usb_o.usb_rq != USB_RQ_NONE) || (length % sizeof(uint32_t))){
		return;
	}

	//Variables for CRCs
	uint32_t stored_crc = 0;
	uint32_t actual_crc = 0;
	
	//Calculate expecting configuration request packet length without CRC padding symbols
	uint32_t cmd_rx_config_rq_length = 2 + sizeof(uint8_t) + sizeof(uint32_t);
	//Include CRC padding symbols in length
	while(cmd_rx_config_rq_length % sizeof(uint32_t)) cmd_rx_config_rq_length++;

	//Configuration request from host, length is less than 64
	if(
		(data[1] == USB_CMD_CONFIG) &&
		((uint32_t)data[0] == cmd_rx_config_rq_length) &&
		(length == cmd_rx_config_rq_length)
	){
		_memcpy((uint32_t)&stored_crc, (uint32_t)&data[length - sizeof(uint32_t)], sizeof(uint32_t));
		actual_crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)data, (length / sizeof(uint32_t)) - 1);
		if(stored_crc == actual_crc){
			//Parse received request
			usb_o.set_revert_flash_rq = data[2];
			usb_o.usb_rq = USB_RQ_CONF;
		}
		return;
	}

	//Calculate expecting status request packet length without CRC padding symbols
	//Length and command bytes
	uint32_t cmd_rx_status_rq_length = 2;
	//Append "usb_config_t" length
	cmd_rx_status_rq_length +=
		sizeof(uint16_t) +
		sizeof(uint16_t) +
		sizeof(uint8_t) +
		sizeof(bool) +
		sizeof(bool) +
		sizeof(float) +
		sizeof(float) +
		sizeof(uint32_t) +
		sizeof(uint8_t) +
		sizeof(bool) +
		sizeof(float) +
		sizeof(float) +
		sizeof(float) +
		sizeof(float) +
		sizeof(float) +
		sizeof(float) +
		sizeof(float) +
		sizeof(float) +
		sizeof(float) +
		sizeof(uint16_t) +
		sizeof(uint16_t) +
		sizeof(uint8_t);
	//Append "usb_rx_status_t" length
	cmd_rx_status_rq_length +=
		sizeof(float) +
		sizeof(bool) +
		sizeof(uint8_t);
	//Append CRC length
	cmd_rx_status_rq_length +=
		sizeof(uint32_t);
	
	//Include CRC padding symbols in length
	while(cmd_rx_status_rq_length % sizeof(uint32_t)) cmd_rx_status_rq_length++;

	//Status request from host, length is more than 64, so here we store first 64 bytes
	if(
		(data[1] == USB_CMD_STATUS) &&
		((uint32_t)data[0] == cmd_rx_status_rq_length) &&
		(length == 64)
	){
		_memcpy((uint32_t)&rx_buffer[0], (uint32_t)&data[0], 64);
		return;
	}

	//Status request from host, here we store remaining bytes and parse results
	if(
		(rx_buffer[0] == cmd_rx_status_rq_length) &&
		(rx_buffer[1] == USB_CMD_STATUS) &&
		(64 + length == cmd_rx_status_rq_length)
	){
		//Copy remaining data
		_memcpy((uint32_t)&rx_buffer[64], (uint32_t)&data[0], length);
		//Check CRC
		_memcpy((uint32_t)&stored_crc, (uint32_t)&rx_buffer[cmd_rx_status_rq_length - sizeof(uint32_t)], sizeof(uint32_t));
		actual_crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)rx_buffer, (cmd_rx_status_rq_length / sizeof(uint32_t)) - 1);
		if(stored_crc == actual_crc){
			//We don't copy size and command bytes here
			length = 2;

			//Copy config to global struct variables
			_memcpy((uint32_t)&usb_rx_config.potentiometer_min, (uint32_t)&rx_buffer[length], sizeof(uint16_t));
			length += sizeof(uint16_t);
			_memcpy((uint32_t)&usb_rx_config.potentiometer_max, (uint32_t)&rx_buffer[length], sizeof(uint16_t));
			length += sizeof(uint16_t);
			_memcpy((uint32_t)&usb_rx_config.motor_max_power, (uint32_t)&rx_buffer[length], sizeof(uint8_t));
			length += sizeof(uint8_t);
			_memcpy((uint32_t)&usb_rx_config.motor_invert, (uint32_t)&rx_buffer[length], sizeof(bool));
			length += sizeof(bool);
			_memcpy((uint32_t)&usb_rx_config.signal_ignore, (uint32_t)&rx_buffer[length], sizeof(bool));
			length += sizeof(bool);
			_memcpy((uint32_t)&usb_rx_config.signal_min, (uint32_t)&rx_buffer[length], sizeof(float));
			length += sizeof(float);
			_memcpy((uint32_t)&usb_rx_config.signal_max, (uint32_t)&rx_buffer[length], sizeof(float));
			length += sizeof(float);
			_memcpy((uint32_t)&usb_rx_config.signal_timeout, (uint32_t)&rx_buffer[length], sizeof(uint32_t));
			length += sizeof(uint32_t);
			_memcpy((uint32_t)&usb_rx_config.led_mode, (uint32_t)&rx_buffer[length], sizeof(uint8_t));
			length += sizeof(uint8_t);
			_memcpy((uint32_t)&usb_rx_config.pid_on_e_m, (uint32_t)&rx_buffer[length], sizeof(bool));
			length += sizeof(bool);
			_memcpy((uint32_t)&usb_rx_config.pid_kp_1, (uint32_t)&rx_buffer[length], sizeof(float));
			length += sizeof(float);
			_memcpy((uint32_t)&usb_rx_config.pid_ki_1, (uint32_t)&rx_buffer[length], sizeof(float));
			length += sizeof(float);
			_memcpy((uint32_t)&usb_rx_config.pid_kd_1, (uint32_t)&rx_buffer[length], sizeof(float));
			length += sizeof(float);
			_memcpy((uint32_t)&usb_rx_config.pid_kp_2, (uint32_t)&rx_buffer[length], sizeof(float));
			length += sizeof(float);
			_memcpy((uint32_t)&usb_rx_config.pid_ki_2, (uint32_t)&rx_buffer[length], sizeof(float));
			length += sizeof(float);
			_memcpy((uint32_t)&usb_rx_config.pid_kd_2, (uint32_t)&rx_buffer[length], sizeof(float));
			length += sizeof(float);
			_memcpy((uint32_t)&usb_rx_config.pid_kp_3, (uint32_t)&rx_buffer[length], sizeof(float));
			length += sizeof(float);
			_memcpy((uint32_t)&usb_rx_config.pid_ki_3, (uint32_t)&rx_buffer[length], sizeof(float));
			length += sizeof(float);
			_memcpy((uint32_t)&usb_rx_config.pid_kd_3, (uint32_t)&rx_buffer[length], sizeof(float));
			length += sizeof(float);
			_memcpy((uint32_t)&usb_rx_config.pid_split_steps_1, (uint32_t)&rx_buffer[length], sizeof(uint16_t));
			length += sizeof(uint16_t);
			_memcpy((uint32_t)&usb_rx_config.pid_split_steps_2, (uint32_t)&rx_buffer[length], sizeof(uint16_t));
			length += sizeof(uint16_t);
			_memcpy((uint32_t)&usb_rx_config.pid_sampling_ms, (uint32_t)&rx_buffer[length], sizeof(uint8_t));
			length += sizeof(uint8_t);

			//Copy status
			_memcpy((uint32_t)&usb_rx_status.pid_setpoint, (uint32_t)&rx_buffer[length], sizeof(float));
			length += sizeof(float);
			_memcpy((uint32_t)&usb_rx_status.usb_rq_stat_phold, (uint32_t)&rx_buffer[length], sizeof(bool));
			length += sizeof(bool);
			_memcpy((uint32_t)&usb_rx_status.usb_rq_stat_motor, (uint32_t)&rx_buffer[length], sizeof(uint8_t));
			length += sizeof(uint8_t);

			//Set status request flag
			usb_o.usb_rq = USB_RQ_STAT;
		}
	}

	//Clear length and command bytes in buffer
	rx_buffer[0] = 0x00;
	rx_buffer[1] = 0xff;
}

//Sends status, clears pending request flag
void USB_Send_Status(usb_tx_status_t *status){
	//Keep track of written data
	uint8_t length = 0; //Total length should not be more than 255 bytes

	//First byte of tx array contains total data length, we write it last
	length++;
	
	//Write command byte
	tx_buffer[length] = USB_CMD_STATUS;
	length++;

	//Write data
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&status->pid_running, sizeof(bool));
	length += sizeof(bool);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&status->pid_setpoint, sizeof(float));
	length += sizeof(float);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&status->potentiometer_position, sizeof(float));
	length += sizeof(float);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&status->motor_power, sizeof(int16_t));
	length += sizeof(int16_t);

	//Write CRC padding bytes
	while(length % sizeof(uint32_t)){
		tx_buffer[length] = USB_CRC_PADDING_SYMBOL;
		length++;
	}

	//Set length at start now
	tx_buffer[0] = length + sizeof(uint32_t);

	//Write CRC
	uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)&tx_buffer, (length / sizeof(uint32_t)));
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&crc, sizeof(uint32_t));
	length += sizeof(uint32_t);
	
	//Send
	CDC_Transmit_FS((uint8_t *)&tx_buffer, length);

	//Clear flags
	usb_o.set_revert_flash_rq = USB_SETTINGS_KEEP;
	usb_o.usb_rq = USB_RQ_NONE;
}
//Sends config, clears pending request flag
void USB_Send_Config(usb_config_t *config){
	//Keep track of written data
	uint8_t length = 0; //Total length should not be more than 255 bytes

	//First byte of tx array contains total data length, we write it last
	length++;
	
	//Write command byte
	tx_buffer[length] = USB_CMD_CONFIG;
	length++;

	//Write data
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->potentiometer_min, sizeof(uint16_t));
	length += sizeof(uint16_t);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->potentiometer_max, sizeof(uint16_t));
	length += sizeof(uint16_t);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->motor_max_power, sizeof(uint8_t));
	length += sizeof(uint8_t);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->motor_invert, sizeof(bool));
	length += sizeof(bool);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->signal_ignore, sizeof(bool));
	length += sizeof(bool);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->signal_min, sizeof(float));
	length += sizeof(float);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->signal_max, sizeof(float));
	length += sizeof(float);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->signal_timeout, sizeof(uint32_t));
	length += sizeof(uint32_t);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->led_mode, sizeof(uint8_t));
	length += sizeof(uint8_t);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->pid_on_e_m, sizeof(bool));
	length += sizeof(bool);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->pid_kp_1, sizeof(float));
	length += sizeof(float);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->pid_ki_1, sizeof(float));
	length += sizeof(float);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->pid_kd_1, sizeof(float));
	length += sizeof(float);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->pid_kp_2, sizeof(float));
	length += sizeof(float);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->pid_ki_2, sizeof(float));
	length += sizeof(float);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->pid_kd_2, sizeof(float));
	length += sizeof(float);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->pid_kp_3, sizeof(float));
	length += sizeof(float);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->pid_ki_3, sizeof(float));
	length += sizeof(float);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->pid_kd_3, sizeof(float));
	length += sizeof(float);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->pid_split_steps_1, sizeof(uint16_t));
	length += sizeof(uint16_t);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->pid_split_steps_2, sizeof(uint16_t));
	length += sizeof(uint16_t);
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&config->pid_sampling_ms, sizeof(uint8_t));
	length += sizeof(uint8_t);

	//Write CRC padding bytes
	while(length % sizeof(uint32_t)){
		tx_buffer[length] = USB_CRC_PADDING_SYMBOL;
		length++;
	}

	//Set length at start now
	tx_buffer[0] = length + sizeof(uint32_t);

	//Write CRC
	uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)&tx_buffer, (length / sizeof(uint32_t)));
	_memcpy((uint32_t)&tx_buffer[length], (uint32_t)&crc, sizeof(uint32_t));
	length += sizeof(uint32_t);

	//Send
	CDC_Transmit_FS((uint8_t *)&tx_buffer, length);

	//Clear flags
	usb_o.set_revert_flash_rq = USB_SETTINGS_KEEP;
	usb_o.usb_rq = USB_RQ_NONE;
}
//Checks if USB is connected and updates "usb_o.usb_present" variable
void USB_Det(){
	usb_o.usb_present = (hUsbDeviceFS.dev_state == USBD_STATE_ADDRESSED) || (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED);
}
