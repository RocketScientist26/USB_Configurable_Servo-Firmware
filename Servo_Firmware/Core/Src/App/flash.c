#include "main.h"
#include "flash.h"

void Flash_Read(uint8_t *data, uint32_t address, uint32_t length){
	uint32_t i = 0;
	while(i != length){
		data[i] = *(uint8_t *)(address + i);
		i++;
	}
}
void Flash_Write(uint8_t *data, uint32_t address, uint32_t length){
	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = address;
	EraseInitStruct.NbPages = 1;
	uint32_t ErrorCode = 0;
	HAL_FLASH_Unlock();
	HAL_FLASHEx_Erase(&EraseInitStruct, &ErrorCode);

	uint16_t fpages = length/4;
	uint16_t lbytes = length-(fpages*4);
	uint16_t i = 0;
	while(i != (fpages*4)){
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + i, ((uint32_t)data[i]<<24)|((uint32_t)data[i+1]<<16)|((uint32_t)data[i+2]<<8)|(uint32_t)data[i+3]);
		i+=4;
	}
	if(lbytes != 0){
		uint32_t wdata = 0;
		if(lbytes == 3){
			wdata = ((uint32_t)data[i]<<24)|((uint32_t)data[i+1]<<16)|((uint32_t)data[i+2]<<8);
		}
		else if(lbytes == 2){
			wdata = ((uint32_t)data[i]<<24)|((uint32_t)data[i+1]<<16);
		}
		else if(lbytes == 1){
			wdata = ((uint32_t)data[i]<<24);
		}
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + i, wdata);
	}

	HAL_FLASH_Lock();
}
