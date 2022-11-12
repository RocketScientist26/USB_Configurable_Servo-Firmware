/*
	Packets structures

	Configuraiton request
	Incoming:	[LENGTH 1][CONF_CMD 1][SETTINGS_REVERT_FLASH 1][USB_CRC_PADDING 1][CRC32 4]
	Outgoing:	[LENGTH 1][CONF_CMD 1]
				[POTENTIOMETER_START 2][POTENTIOMETER_END 2]
				[MOTOR_MAX_PWR 1][MOTOR_INVERT 1]
				[SIGNAL_IGNORE_ON_USB 1][SIGNAL_LEN 4][SIGNAL_TIMEOUT 4]
				[LED 1]
				[PID_ON 1]
				[PID_P 4]
				[PID_I 4]
				[PID_D 4]
				[PID_SAMPLE_TIME 1]
				[CRC32 4]
	
	Status request (incoming configuration)
	Incoming:	[LENGTH 1][STAT_CMD 1]
				[POTENTIOMETER_START 2][POTENTIOMETER_END 2]
				[MOTOR_MAX_PWR 1][MOTOR_INVERT 1]
				[SIGNAL_IGNORE_ON_USB 1][SIGNAL_LEN 4][SIGNAL_TIMEOUT 4]
				[LED 1]
				[PID_ON 1]
				[PID_P 4]
				[PID_I 4]
				[PID_D 4]
				[PID_SAMPLE_TIME 1]
				[RQ_POSITION 4][RQ_SIGNAL_HOLD 1][RQ_MOTOR_RUN 1]
				[CRC_PADDING 2][CRC32 4]
	Outgoing:	[LENGTH 1][STAT_CMD 1]
				[PID_RUNNING 1][PID_SET_POS 4][POTENTIOMETER_POSITION 4][MOTOR_POWER 2]
				[USB_CRC_PADDING 3][CRC32 4]
*/

//Lengths
#define USB_RX_CONFIG_LENGTH 8
#define USB_TX_CONFIG_LENGTH 68
#define USB_RX_STATUS_LENGTH 76
#define USB_TX_STATUS_LENGTH 20

//Commands
#define USB_CMD_CONFIG 0
#define USB_CMD_STATUS 1

//Flags
#define USB_SETTINGS_KEEP 0
#define USB_SETTINGS_REVERT 1
#define USB_SETTINGS_FLASH 2

#define USB_RQ_NONE 0
#define USB_RQ_CONF 1
#define USB_RQ_STAT 2

//CRC Padding
#define USB_CRC_PADDING 0

void USB_Det();
void USB_Packet_Received(uint8_t *data, uint32_t lenght);
void USB_Parse_Config(uint8_t *data);
void USB_Send_Config();
void USB_Parse_Status(uint8_t *data);
void USB_Send_Status();
