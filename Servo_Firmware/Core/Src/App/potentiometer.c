#include "main.h"
#include "potentiometer.h"

extern ADC_HandleTypeDef hadc1;

//Configuration
uint16_t potentiometer_min;
uint16_t potentiometer_max;
//Runtime
uint32_t potentiometer_adctmp = 0;
uint16_t potentiometer_adccnt = 0;
uint8_t potentiometer_init_cplt = 0;
float potentiometer_position = 0;

void Potentiometer_ADC_Interrupt(){
	potentiometer_adctmp += HAL_ADC_GetValue(&hadc1);
	potentiometer_adccnt++;
	if(potentiometer_adccnt == POTENTIOMETER_SAMPLE_CNT){
		potentiometer_position = (float)potentiometer_adctmp/POTENTIOMETER_SAMPLE_CNT;
		Potentiometer_Ready();
		potentiometer_adccnt = 0;
		potentiometer_adctmp = 0;
	}
	potentiometer_init_cplt = 1;
}

void Potentiometer_Init(){
	while(HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK);
	HAL_ADC_Start_IT(&hadc1);
	while(!potentiometer_init_cplt);
}

