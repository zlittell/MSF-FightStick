/*
    Mechanical Squid Factory presents to you:
    Open Source Fight Stick of Awesomesauce
    Compatible w/ PC
    
    Developer: Zack "Reaper" Littell
    Email: zlittell@gmail.com
    www.zlittell.com
    
    Cheap and awesome were the goals so hopefully that works out
    Uses the Teensy-LC
    
    This tricks the computer into loading the xbox 360 controller driver.
    Then it sends button reports in the same way as the xbox 360 controller.
    Which means this is an example of making the teensy into a XINPUT device :).
    
    This is used in a box with a joystick, 8 buttons on top, and a start select on the front.
    Also used in a similar setup but with a "hit box" layout.
    Press start and select together for xbox logo.
    This should work with steam big picture and stuff.
    
    Attempted to save PWM capable pins for LEDs if needed in the future.
    But some were sacrificed for ease of layout
    
    Would love to eventually move to the FreeScale KDS and off the arduino IDE
    
    Credit where credit is due:
    Paul Stoffregen - for the awesome teensy and all the awesome examples he has included
    Hamaluik.com - for allowing me to not murder the arduino "IDE" out of frustration of hidden "magic"
    BeyondLogic.org - for a great resource on all things USB protocol related
    BrandonW - I contacted him a long time ago for a different project to get log files from his
             - beagle usb 12 between the 360 and controller.  I used them again for verification
             - and understanding during this project. (brandonw.net)
    free60.org - for their page on the x360 gamepad and its lusb output plus the explanations of the descriptors
    Microsoft - Windows Message Analyzer.  It wouldn't have been possible at times without this awesome message
              - analyzer capturing USB packets.  Debugged many issues with enumerating the device using this.
              
    Also one final shoutout to Microsoft... basically **** you for creating xinput and not using HID to do so.
    XINPUT makes signing drivers necessary again, which means paying you.  Also you have ZERO openly available
    documentation on the XUSB device standard and I hate you for that.
*/

//Includes
#include <Bounce.h>
#include "FightStick.h"

//LED STYLE
//NO_LED = Do not use LED
//ONBOARD_LED = use on board LED
//EXTERNAL_LED = in the future add option to use outputs for 4 LEDS
const int LEDSTYLE = ONBOARD_LED;

//Global Variables
byte buttonStatus[NUMBUTTONS];  //array Holds a "Snapshot" of the button status to parse and manipulate
uint8_t TXData[20] = {0x00, 0x14, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  //Holds USB transmit packet data
uint8_t RXData[3] = {0x00, 0x00, 0x00};  //Holds USB receive packet data

//LED Toggle Tracking Global Variables
uint8_t LEDState = LOW;	//used to set the pin for the LED
uint32_t previousMS = 0; //used to store the last time LED was updated
uint8_t LEDtracker = 0;	//used as an index to step through a pattern on interval

//LED Patterns
uint8_t patternAllOff[10] = {0,0,0,0,0,0,0,0,0,0};
uint8_t patternBlinkRotate[10] = {1,0,1,0,1,0,1,0,1,0};
uint8_t patternPlayer1[10] = {1,0,0,0,0,0,0,0,0,0};
uint8_t patternPlayer2[10] = {1,0,1,0,0,0,0,0,0,0};
uint8_t patternPlayer3[10] = {1,0,1,0,1,0,0,0,0,0};
uint8_t patternPlayer4[10] = {1,0,1,0,1,0,1,0,0,0};

//Variable to hold the current pattern selected by the host
uint8_t patternCurrent[10] = {0,0,0,0,0,0,0,0,0,0};

//Setup Button Debouncing
Bounce joystickUP = Bounce(pinUP, MILLIDEBOUNCE);
Bounce joystickDOWN = Bounce(pinDN, MILLIDEBOUNCE);
Bounce joystickLEFT = Bounce(pinLT, MILLIDEBOUNCE);
Bounce joystickRIGHT = Bounce(pinRT, MILLIDEBOUNCE);
Bounce button1 = Bounce(pinB1, MILLIDEBOUNCE);
Bounce button2 = Bounce(pinB2, MILLIDEBOUNCE);
Bounce button3 = Bounce(pinB3, MILLIDEBOUNCE);
Bounce button4 = Bounce(pinB4, MILLIDEBOUNCE);
Bounce button5 = Bounce(pinB5, MILLIDEBOUNCE);
Bounce button6 = Bounce(pinB6, MILLIDEBOUNCE);
Bounce button7 = Bounce(pinB7, MILLIDEBOUNCE);
Bounce button8 = Bounce(pinB8, MILLIDEBOUNCE);
Bounce buttonSTART = Bounce(pinST, MILLIDEBOUNCE);
Bounce buttonSELECT = Bounce(pinSL, MILLIDEBOUNCE);

//void Configure Inputs and Outputs
void setupPins()
{
    //Configure the direction of the pins
    //All inputs with internal pullups enabled
    pinMode(pinUP, INPUT_PULLUP);
    pinMode(pinDN, INPUT_PULLUP);
    pinMode(pinLT, INPUT_PULLUP);
    pinMode(pinRT, INPUT_PULLUP);
    pinMode(pinB1, INPUT_PULLUP);
    pinMode(pinB2, INPUT_PULLUP);
    pinMode(pinB3, INPUT_PULLUP);
    pinMode(pinB4, INPUT_PULLUP);
    pinMode(pinB5, INPUT_PULLUP);
    pinMode(pinB6, INPUT_PULLUP);
    pinMode(pinB7, INPUT_PULLUP);
    pinMode(pinB8, INPUT_PULLUP);
    pinMode(pinST, INPUT_PULLUP);
    pinMode(pinSL, INPUT_PULLUP);
    pinMode(pinOBLED, OUTPUT);  
    //Set the LED to low to make sure it is off
    digitalWrite(pinOBLED, LOW);
}

//Update the debounced button statuses
//We are looking for falling edges since the boards are built
//for common ground sticks
void buttonUpdate()
{
  if (joystickUP.update()) {buttonStatus[POSUP] = joystickUP.fallingEdge();}
  if (joystickDOWN.update()) {buttonStatus[POSDN] = joystickDOWN.fallingEdge();}
  if (joystickLEFT.update()) {buttonStatus[POSLT] = joystickLEFT.fallingEdge();}
  if (joystickRIGHT.update()) {buttonStatus[POSRT] = joystickRIGHT.fallingEdge();}
  if (button1.update()) {buttonStatus[POSB1] = button1.fallingEdge();}
  if (button2.update()) {buttonStatus[POSB2] = button2.fallingEdge();}
  if (button3.update()) {buttonStatus[POSB3] = button3.fallingEdge();}
  if (button4.update()) {buttonStatus[POSB4] = button4.fallingEdge();}
  if (button5.update()) {buttonStatus[POSB5] = button5.fallingEdge();}
  if (button6.update()) {buttonStatus[POSB6] = button6.fallingEdge();}
  if (button7.update()) {buttonStatus[POSB7] = button7.fallingEdge();}
  if (button8.update()) {buttonStatus[POSB8] = button8.fallingEdge();}
  if (buttonSTART.update()) {buttonStatus[POSST] = buttonSTART.fallingEdge();}
  if (buttonSELECT.update()) {buttonStatus[POSSL] = buttonSELECT.fallingEdge();}
}

//ProcessInputs
//Button layout on fight stick
//      SL ST
//1  2  3  4
//5  6  7  8
//X360 Verson
//      BK  ST
//X  Y  RB  LB
//A  B  RT  LT
void processInputs()
{
  //Zero out button values
  //Start at 2 so that you can keep the message type and packet size
  //Then fill the rest with 0x00's
  for (int i=2; i<13; i++) {TXData[i] = 0x00;}
  
  //Button Packet 1 (usb data array position 2)
  //DPAD Up
  if (buttonStatus[POSUP]) {TXData[BUTTON_PACKET_1] |= DPAD_UP_MASK;}
  //DPAD Down
  if (buttonStatus[POSDN]) {TXData[BUTTON_PACKET_1] |= DPAD_DOWN_MASK;}
  //DPAD Left
  if (buttonStatus[POSLT]) {TXData[BUTTON_PACKET_1] |= DPAD_LEFT_MASK;}
  //DPAD Right
  if (buttonStatus[POSRT]) {TXData[BUTTON_PACKET_1] |= DPAD_RIGHT_MASK;}
  
  //Button Start OR Select OR Both (XBOX Logo)
  if (buttonStatus[POSST]&&buttonStatus[POSSL]) {TXData[BUTTON_PACKET_2] |= LOGO_MASK;}
  else if (buttonStatus[POSST]) {TXData[BUTTON_PACKET_1] |= START_MASK;}
  else if (buttonStatus[POSSL]) {TXData[BUTTON_PACKET_1] |= BACK_MASK;}
  
  //Button Packet 2 (usb data array position 3)
  //Button 1
  if (buttonStatus[POSB1]) {TXData[BUTTON_PACKET_2] |= X_MASK;}
  //Button 2
  if (buttonStatus[POSB2]) {TXData[BUTTON_PACKET_2] |= Y_MASK;}
  //Button 3
  if (buttonStatus[POSB3]) {TXData[BUTTON_PACKET_2] |= RB_MASK;}
  //Button 4
  if (buttonStatus[POSB4]) {TXData[BUTTON_PACKET_2] |= LB_MASK;}
  //Button 5
  if (buttonStatus[POSB5]) {TXData[BUTTON_PACKET_2] |= A_MASK;}
  //Button 6
  if (buttonStatus[POSB6]) {TXData[BUTTON_PACKET_2] |= B_MASK;}
  
  //Triggers (usb data array position 4 and 5)
  //0xFF is full scale
  //Button 7
  if (buttonStatus[POSB7]) {TXData[LEFT_TRIGGER_PACKET] = 0xFF;}
  //Button 8
  if (buttonStatus[POSB8]) {TXData[RIGHT_TRIGGER_PACKET] = 0xFF;}
}

//Select pattern
//Examines USB packet and sets the correct pattern
//according to the 3rd byte
/*
Process the LED Pattern
0x00 OFF
0x01 All Blinking
0x02 1 Flashes, then on
0x03 2 Flashes, then on
0x04 3 Flashes, then on
0x05 4 Flashes, then on
0x06 1 on
0x07 2 on
0x08 3 on
0x09 4 on
0x0A Rotating (1-2-4-3)
0x0B Blinking*
0x0C Slow Blinking*
0x0D Alternating (1+4-2+3)*
*Does Pattern and then goes back to previous
*/
void LEDPatternSelect()
{
	//All blinking or rotating
	if((RXData[2]==ALLBLINKING)||(RXData[2]==ROTATING))
	{
		//Copy the pattern array into the current pattern
		memcpy(patternCurrent, patternBlinkRotate, 10);
		//Reset the index to beginning of pattern
		LEDtracker = 0;	
	}
	//Device is player 1
	else if ((RXData[2]==FLASHON1)||(RXData[2]==ON1))
	{
		//Copy the pattern array into the current pattern
		memcpy(patternCurrent, patternPlayer1, 10);
		//Reset the index to beginning of pattern
		LEDtracker = 0;
	}
	//Device is player 2
	else if ((RXData[2]==FLASHON2)||(RXData[2]==ON2))
	{
		//Copy the pattern array into the current pattern
		memcpy(patternCurrent, patternPlayer2, 10);
		//Reset the index to beginning of pattern
		LEDtracker = 0;
	}
	//Device is player 3
	else if ((RXData[2]==FLASHON3)||(RXData[2]==ON3))
	{
		//Copy the pattern array into the current pattern
		memcpy(patternCurrent, patternPlayer3, 10);
		//Reset the index to beginning of pattern
		LEDtracker = 0;
	}
	//Device is player 4
	else if ((RXData[2]==FLASHON4)||(RXData[2]==ON4))
	{
		//Copy the pattern array into the current pattern
		memcpy(patternCurrent, patternPlayer4, 10);
		//Reset the index to beginning of pattern
		LEDtracker = 0;
	}
	//If pattern is not specified perform no pattern
	else
	{
		//Copy the pattern array into the current pattern
		memcpy(patternCurrent, patternAllOff, 10);
		//Pattern is all 0's so we don't care where LEDtracker is at
	}
}

//Cycle through the LED pattern
void LEDPatternDisplay(void)
{
	//Grab the current time in mS that the program has been running
	uint32_t currentMS = millis();
	
	//subtract the previous update time from the current time and see if interval has passed
	if ((currentMS - previousMS)>interval)
	{
		//Set the led state correctly according to next part of pattern
		LEDState = patternCurrent[LEDtracker];
		//update the previous time
		previousMS = currentMS;
		//increment the pattern tracker
		LEDtracker++;
		//write the state to the led
		digitalWrite(pinOBLED, LEDState);
	}
	
	//if we increased ledtracker to 10, it needs to rollover
	if (LEDtracker==10) {LEDtracker=0;}
}

//Setup
void setup() 
{
  setupPins();
}

void loop() 
{
  //Poll Buttons
  buttonUpdate();
  
  //Process all inputs and load up the usbData registers correctly
  processInputs();
  
  //Check for bootloader jump
  if (buttonStatus[POSUP] & buttonStatus[POSB1] & buttonStatus[POSB5] & buttonStatus[POSST] & buttonStatus[POSSL])
  {
    _reboot_Teensyduino_();
  }
  
  //Update Buttons
  XInput.send(TXData, USB_TIMEOUT);
  
  //Check if packet available and parse to see if its an LED pattern packet
  if (XInput.available() > 0)
  {
    //Grab packet and store it in RXData array
    XInput.recv(RXData, USB_TIMEOUT);
    
    //If the data is an LED pattern command parse it
    if(RXData[0] == 1)
    {
      LEDPatternSelect();
    }
  }
  
  //Process the LED pattern if the style is onboard LED
  if (LEDSTYLE == ONBOARD_LED)
  {
    LEDPatternDisplay();
  }
}
