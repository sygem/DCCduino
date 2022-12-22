/**********************************************************************

Config.h
COPYRIGHT (c) 2013-2016 Gregg E. Berman

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/

/////////////////////////////////////////////////////////////////////////////////////
//
// DEFINE MOTOR_SHIELD_TYPE ACCORDING TO THE FOLLOWING TABLE:
//
//  0 = ARDUINO MOTOR SHIELD          (MAX 18V/2A PER CHANNEL)
//  1 = POLOLU MC33926 MOTOR SHIELD   (MAX 28V/3A PER CHANNEL)
//  2 = BTS7960B                      (MAX 27V/43A PER CHANNEL)

#define MOTOR_SHIELD_TYPE   0

/////////////////////////////////////////////////////////////////////////////////////
//
// DEFINE NUMBER OF MAIN TRACK REGISTER

#define MAX_MAIN_REGISTERS 12

/////////////////////////////////////////////////////////////////////////////////////
//
// DEFINE COMMUNICATIONS INTERFACE
//
//  0 = Built-in Serial Port
//  1 = Arduino.cc Ethernet/SD-Card Shield
//  2 = Arduino.org Ethernet/SD-Card Shield
//  3 = Seeed Studio Ethernet/SD-Card Shield W5200
//  4 = ESP8266 WiFi module

#define COMM_INTERFACE   0

#if COMM_INTERFACE == 4
	/////////////////////////////////////////////////////////////////////////////////////
	//
	// DEFINE WiFi Parameters
	//
	#define WIFI_SSID ""
	#define WIFI_PASSWORD ""

	// The WIFI_SERIAL_RX and WIFI_SERIAL_TX config values are not used on the Mega2650 which
	// has multiple Serial interfaces.  For Mega2560 the default is Serial1.
	//#define WIFI_SERIAL_RX
	//#define WIFI_SERIAL_TX

	// For the Uno you can also use Serial instead of SoftwareSerial by defining the following
	// option.  Enabling this option will set the IO device as Serial instead of
	// SoftwareSerial using the above WIFI_SERIAL_RX and WIFI_SERIAL_TX pin assignments. This
	// can also be used on the Mega2560 to allow Serial1 to be used for other purposes.
	// #define USE_SERIAL_FOR_WIFI

	// This defines the speed at which the Arduino will communicate with the ESP8266 module.
	// When using the ESP8266 on an Uno it is recommended to use 9600, for Mega2560 the
	// higher speed can be used.  Set this based on your ESP8266 module configuration.
	// Common defaults are 9600 or 115200.
	#define WIFI_SERIAL_LINK_SPEED 115200
#endif

/////////////////////////////////////////////////////////////////////////////////////
//
// DEFINE STATIC IP ADDRESS *OR* COMMENT OUT TO USE DHCP
//

//#define IP_ADDRESS { 192, 168, 1, 200 }

/////////////////////////////////////////////////////////////////////////////////////
//
// DEFINE PORT TO USE FOR ETHERNET COMMUNICATIONS INTERFACE
//

#define ETHERNET_PORT 2560

/////////////////////////////////////////////////////////////////////////////////////
//
// DEFINE MAC ADDRESS ARRAY FOR ETHERNET COMMUNICATIONS INTERFACE
//
// Note: This is not used with ESP8266 WiFi modules.

#define MAC_ADDRESS {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF }

/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
//
// DEFINE LCD SCREEN USAGE BY THE BASE STATION
//
// Note: This feature requires an I2C enabled LCD screen using a PCF8574 based chipset.
//
//#define ENABLE_LCD

#ifdef ENABLE_LCD
	// This defines the I2C address for the LCD device
	#define LCD_ADDRESS 0x27

	// This defines the number of columns the LCD device has
	#define LCD_COLUMNS 20

	// This defines the number of lines the LCD device has
	#define LCD_LINES 4
#endif
