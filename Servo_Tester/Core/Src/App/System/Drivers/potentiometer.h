//Sample count for averaging measurements for single result
#define POTENTIOMETER_SAMPLE_CNT 100

//Functions
void Potentiometer_ADC_Interrupt();
void Potentiometer_Init();

//This functions should be implemented in external file
void Potentiometer_Ready(float position);