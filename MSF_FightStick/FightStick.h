//General Declarations
#define MILLIDEBOUNCE 20  //Debounce time in milliseconds
#define NUMBUTTONS 14  //Number of all buttons
#define NUMBUTTONSONLY 10 //Number of just buttons
#define interval 150	//interval in milliseconds to update LED
#define USB_TIMEOUT 12840	//packet timeout for USB

//LED STYLE DEFINES
#define NO_LED 0
#define ONBOARD_LED 1
#define EXTERNAL_LED 2

//LED Pattern Defines
#define ALLOFF 0x00
#define ALLBLINKING 0x01
#define FLASHON1 0x02
#define FLASHON2 0x03
#define FLASHON3 0x04
#define FLASHON4 0x05
#define ON1  0x06
#define ON2  0x07
#define ON3  0x08
#define ON4  0x09
#define ROTATING 0x0A
#define BLINK	 0x0B
#define SLOWBLINK 0x0C
#define ALTERNATE 0x0D

//BUTTON MASK DEFINES
#define R3_MASK 0x80
#define L3_MASK 0x40
#define BACK_MASK 0x20
#define START_MASK 0x10
#define DPAD_RIGHT_MASK 0x08
#define DPAD_LEFT_MASK 0x04
#define DPAD_DOWN_MASK 0x02
#define DPAD_UP_MASK 0x01
#define Y_MASK 0x80
#define X_MASK 0x40
#define B_MASK 0x20
#define A_MASK 0x10
#define LOGO_MASK 0x04
#define RB_MASK 0x02
#define LB_MASK 0x01

//Byte location Definitions
#define BUTTON_PACKET_1 2
#define BUTTON_PACKET_2 3
#define LEFT_TRIGGER_PACKET 4
#define RIGHT_TRIGGER_PACKET 5
#define LEFT_STICK_X_PACKET_LSB 6
#define LEFT_STICK_X_PACKET_MSB 7
#define LEFT_STICK_Y_PACKET_LSB 8
#define LEFT_STICK_Y_PACKET_MSB 9
#define RIGHT_STICK_X_PACKET_LSB 10
#define RIGHT_STICK_X_PACKET_MSB 11
#define RIGHT_STICK_Y_PACKET_LSB 12
#define RIGHT_STICK_Y_PACKET_MSB 13

//Pin Declarations
#define pinUP 5  //Up on stick is pin 5
#define pinDN 6  //Down on stick is pin 6
#define pinLT 7  //Left on stick is pin 7
#define pinRT 8  //Right on stick is pin 8
#define pinB1 9  //Button 1 is pin 9 (Start of top row and across)
#define pinB2 10  //Button 2 is pin 10
#define pinB3 11  //Button 3 is pin 11
#define pinB4 12  //Button 4 is pin 12
#define pinB5 14  //Button 5 is pin 13 (Start of second row and across)
#define pinB6 15  //Button 6 is pin 14
#define pinB7 16  //Button 7 is pin 15
#define pinB8 17  //Button 8 is pin 16
#define pinST 18  //Start Button is pin 17
#define pinSL 19  //Select Button is pin 18
#define pinOBLED 13  //Onboard LED pin

//Position of a button in the button status array
#define POSUP 0
#define POSDN 1
#define POSLT 2
#define POSRT 3
#define POSB1 4
#define POSB2 5
#define POSB3 6
#define POSB4 7
#define POSB5 8
#define POSB6 9
#define POSB7 10
#define POSB8 11
#define POSST 12
#define POSSL 13

