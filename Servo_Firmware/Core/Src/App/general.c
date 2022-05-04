#include "main.h"
#include "general.h"

void General_Copy_32_Bit(uint32_t address_to, uint32_t address_from){
	uint8_t i = 0;
	while(i != 4){
		*(uint8_t *)(address_to + i) = *(uint8_t *)(address_from + i);
		i++;
	}
}
void General_Copy_16_Bit(uint32_t address_to, uint32_t address_from){
	uint8_t i = 0;
	while(i != 2){
		*(uint8_t *)(address_to + i) = *(uint8_t *)(address_from + i);
		i++;
	}
}