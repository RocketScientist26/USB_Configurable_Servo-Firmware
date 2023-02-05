#include "main.h"
#include "potentiometer.h"

extern ADC_HandleTypeDef hadc1;

//Output data
potentiometer_o_t potentiometer_o;

//Working variables
static uint32_t adctmp; //Buffer variable for collecting ADC measurements sum
static uint16_t adccnt; //Counter for how many measurements were performed
static bool init_cplt; //Set to "true" once ADC initialization is complete and measurement is working

//ADC Interrupt
void Potentiometer_ADC_Interrupt(){
	adctmp += HAL_ADC_GetValue(&hadc1);
	adccnt++;
	if(adccnt == POTENTIOMETER_SAMPLE_CNT){
		potentiometer_o.position = (float)adctmp / (float)POTENTIOMETER_SAMPLE_CNT;
		Potentiometer_Ready();
		adccnt = 0;
		adctmp = 0;
	}
	init_cplt = true;
}

//Runs measurement, called once at start
void Potentiometer_Init(){
	while(HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK);
	HAL_ADC_Start_IT(&hadc1);
	while(!init_cplt);
}
