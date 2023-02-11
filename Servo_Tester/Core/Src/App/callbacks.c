#include <stdbool.h>
#include <stdint.h>
#include "System/Drivers/led.h"
#include "System/Drivers/lock.h"
#include "System/Drivers/potentiometer.h"
#include "System/Drivers/pwm.h"

//Variable used only in this file
static bool lock_enabled;

//Potentiometer
void Potentiometer_Ready(float position){
	if(!lock_enabled){
		Pwm_Set_Ms(1.0f + ((1.0f / 4095.0f) * position));
	}
}

//Lock switch
void Lock_Updated(bool enabled){
	lock_enabled = enabled;
	Led_Set_On(enabled);
}
