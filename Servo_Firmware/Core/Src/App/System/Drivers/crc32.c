#include "main.h"
#include "crc32.h"

extern CRC_HandleTypeDef hcrc;

//Returns 32bit hardware computed CRC of array
uint32_t CRC32_Get(uint32_t *data, uint32_t length_words){
	return HAL_CRC_Calculate(&hcrc, data, length_words);
}