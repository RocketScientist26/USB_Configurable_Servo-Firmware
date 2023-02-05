//Data types
typedef struct{
	bool invert; //Sets motor output pins A/B or B/A, inverting rotation direction
	uint16_t max_power; //Indicates/controls maximum output power on motor
}motor_ctr_t;

typedef struct{
	bool pid_running; //Indicates PID is running or not, if PID is not running we should turn off motor after MOTOR_TEST_TIMEOUT_MS milliseconds
}motor_i_t;

typedef struct{
	bool running; //Indicates motor is currently running or not
}motor_o_t;

//Operation modes
enum{
	MOTOR_NORMAL,
	MOTOR_INVERT
};

//Timeout for motor test mode
#define MOTOR_TEST_TIMEOUT_MS 100

//Functions
void Motor_SysTick_Interrupt();
void Motor_Set(int16_t strength);
int16_t Motor_Get();
void Motor_Init();

//Control/status variables
extern motor_ctr_t motor_ctr;
//Input data
extern motor_i_t motor_i;
//Input data
extern motor_o_t motor_o;
