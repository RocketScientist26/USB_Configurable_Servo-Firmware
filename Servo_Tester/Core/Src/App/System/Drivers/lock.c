#include "main.h"
#include "lock.h"

//Function used only in this file
static uint8_t Lock_Read(){
	/*
		Determine pin state, depounce until get consistent result, or
		give up if retry limit reached and return that lock is disabled
	*/
	uint8_t retry = 0;
	uint8_t level;
	uint8_t i;

	while(retry != LOCK_DEBOUNCE_RETRIES){
		retry++;
		level = HAL_GPIO_ReadPin(LOCK_IN_GPIO_Port, LOCK_IN_Pin);
		i = 0;
		while(i != LOCK_DEBOUNCE_SAMPLES){
			if(HAL_GPIO_ReadPin(LOCK_IN_GPIO_Port, LOCK_IN_Pin) != level){
				if(retry == LOCK_DEBOUNCE_RETRIES){
					//If this is a last retry and still can't determine signal level correctly, just return low
					return LOCK_DISABLED;
				}
				break;
			}
			i++;
		}
		if(i == LOCK_DEBOUNCE_SAMPLES){
			break;
		}
	}

	return level;
}

//Called on pin interrupt and also at start once
void Lock_Interrupt(){
	//Determine lock switch pin is high or low
	uint8_t lock_pin_state = Lock_Read();

	/*
		Determine this lock switch pin state means that position
		lock is enabled or disabled. Depending on which pin state
		(high or low) means which lock state(enabled or disabled)
	*/
	bool lock_state = false;
	if(LOCK_ENABLED){
		lock_state = (bool)lock_pin_state;
	}else{
		if(lock_pin_state){
			lock_state = false;
		}else{
			lock_state = true;
		}
	}

	//Finally run app function
	Lock_Updated(lock_state);
}