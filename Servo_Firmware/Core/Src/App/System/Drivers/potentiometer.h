//Data types
typedef struct{
	float position;
}potentiometer_o_t;

//Parameter defining how many samples should be collected for calculating median for single ADC measurement
#define POTENTIOMETER_SAMPLE_CNT 100

//Functions
void Potentiometer_ADC_Interrupt();
void Potentiometer_Init();

//Function should be implemented externally
void Potentiometer_Ready();

//Output data
extern potentiometer_o_t potentiometer_o;
