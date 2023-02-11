#include "main.h"
#include "potentiometer.h"

extern ADC_HandleTypeDef hadc1;

//Init, collect and calculate variables, used only in this file
static bool init_cplt;
static uint32_t adctmp;
static uint16_t adccnt;

//ADC complete interrupt
void Potentiometer_ADC_Interrupt(){
	adctmp += HAL_ADC_GetValue(&hadc1);
	adccnt++;
	if(adccnt == POTENTIOMETER_SAMPLE_CNT){
		Potentiometer_Ready((float)adctmp / (float)POTENTIOMETER_SAMPLE_CNT);
		adccnt = 0;
		adctmp = 0;
	}
	init_cplt = 1;
}

//Starts measurement and waits for first position measurement to complete
void Potentiometer_Init(){
	while(HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK);
	HAL_ADC_Start_IT(&hadc1);
	while(!init_cplt);
}

