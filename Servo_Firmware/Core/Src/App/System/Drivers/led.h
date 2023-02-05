//Data types
typedef struct{
	uint8_t mode;//Sets/indicates LED operation mode, can be any of four from enum in this header
}led_ctr_t;

typedef struct{
	bool position_changed; //When set means we should fire led if "led_mode == LED_MODE_POSITION_CHANGE" for "LED_POSITION_CHANGE_MODE_TIMEOUT_MS" duration
	bool pid_running; //When set, this means that PID is running and we should fire led if "led_mode == LED_MODE_PID"
}led_i_t;

typedef struct{
	bool on; //Indicates LED is currently on or off
}led_o_t;

//Operation modes
enum{
	LED_MODE_OFF,
	LED_MODE_POWER,
	LED_MODE_PID,
	LED_MODE_POSITION_CHANGE
};

//Parameter for LED_MODE_POSITION_CHANGE operation mode
#define LED_POSITION_CHANGE_MODE_TIMEOUT_MS 20

//Functions
void LED_SysTick_Interrupt();
void LED_Update();

//Control/status variables
extern led_ctr_t led_ctr;
//Input data
extern led_i_t led_i;
//Output data
extern led_o_t led_o;
