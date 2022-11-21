#include "main.h"
#include "lock.h"

uint8_t lock_state;

void Lock_Interrupt(){
	lock_state = Lock_Read();
	Lock_Updated(lock_state);
}
uint8_t Lock_State(){
	return lock_state;
}
uint8_t Lock_Read(){
	uint8_t level;
	uint8_t d = 0;
	uint8_t i = 0;
	while(d != 1){
		i = 0;
		level = HAL_GPIO_ReadPin(LOCK_IN_GPIO_Port, LOCK_IN_Pin);
		while(i != LOCK_DEBOUNCE_SAMPLES){
			i++;
			if(HAL_GPIO_ReadPin(LOCK_IN_GPIO_Port, LOCK_IN_Pin) != level){
				break;
			}
			if(i == LOCK_DEBOUNCE_SAMPLES){
				d = 1;
			}
		}
	}
	return level;
}
void Lock_Init(){
	Lock_Interrupt();
}