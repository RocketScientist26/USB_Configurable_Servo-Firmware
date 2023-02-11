#include <stdbool.h>
#include <stdint.h>
#include "app.h"
#include "System/Drivers/lock.h"
#include "System/Drivers/potentiometer.h"
#include "System/Drivers/pwm.h"

//Runs only once at start
void App_Init(){
	//Detect lock switch state and turn on LED if enabled
	Lock_Interrupt();

	//Start potentiometer measurements (also waits until first position measurement completes)
	Potentiometer_Init();

	//Start PWM output
	Pwm_Init();
}