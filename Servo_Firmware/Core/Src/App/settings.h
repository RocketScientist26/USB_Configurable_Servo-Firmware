//LED position change accuracy
#define SETTINGS_LED_POSITION_CHANGE_ACCURACY_PRCNT 0.035f
//Address in flash memory where we store configuration
#define SETTINGS_FLASH_ADDRESS 0x0800FC00

//Functions
void Settings_Read();
void Settings_Apply();
void Settings_Write();

//Global variable for storing settings
extern usb_config_t settings_data;
