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

//LED STYLE
//0 = Do not use LED
//1 = use on board LED
//2 = in the future add option to use outputs for 4 LEDS
const int LEDSTYLE = 1;

/*
//BUTTON MASK DEFINES
const int R3_MASK 0x80;
const int L3_MASK 0x40;
const int BACK_MASK 0x20;
const int START_MASK 0x10;
const int DPAD_RIGHT_MASK 0x08;
const int DPAD_LEFT_MASK 0x04;
const int DPAD_DOWN_MASK 0x02;
const int DPAD_UP_MASK 0x01;
const int Y_MASK 0x80;
const int X_MASK 0x40;
const int B_MASK 0x20;
const int A_MASK 0x10;
const int LOGO_MASK 0x04;
const int RB_MASK 0x02;
const int LB_MASK 0x01;
//Byte location Definitions
const int BUTTON_PACKET_1 2;
const int BUTTON_PACKET_2 3;
const int LEFT_TRIGGER_PACKET 4;
const int RIGHT_TRIGGER_PACKET 5;
const int LEFT_STICK_X_PACKET_LSB 6;
const int LEFT_STICK_X_PACKET_MSB 7;
const int LEFT_STICK_Y_PACKET_LSB 8;
const int LEFT_STICK_Y_PACKET_MSB 9;
const int RIGHT_STICK_X_PACKET_LSB 10;
const int RIGHT_STICK_X_PACKET_MSB 11;
const int RIGHT_STICK_Y_PACKET_LSB 12;
const int RIGHT_STICK_Y_PACKET_MSB 13;*/

//Declarations
const int MILLIDEBOUNCE= 20;  //Debounce time in milliseconds
const int NUMBUTTONS = 14;  //Number of all buttons
const int NUMBUTTONSONLY = 10; //Number of just buttons
//Pin Declarations
const int pinUP = 5;  //Up on stick is pin 6
const int pinDN = 6;  //Down on stick is pin 7
const int pinLT = 7;  //Left on stick is pin 8
const int pinRT = 8;  //Right on stick is pin 9
const int pinB1 = 9;  //Button 1 is pin 10 (Start of top row and across)
const int pinB2 = 10;  //Button 2 is pin 11
const int pinB3 = 11;  //Button 3 is pin 12
const int pinB4 = 12;  //Button 4 is pin 13
const int pinB5 = 14;  //Button 5 is pin 14 (Start of second row and across)
const int pinB6 = 15;  //Button 6 is pin 15
const int pinB7 = 16;  //Button 7 is pin 16
const int pinB8 = 17;  //Button 8 is pin 17
const int pinST = 18;  //Start Button is pin 18
const int pinSL = 19;  //Select Button is pin 19
const int pinOBLED = 13;  //Onboard LED pin
//Position of a button in the button status array
const int POSUP = 0;
const int POSDN = 1;
const int POSLT = 2;
const int POSRT = 3;
const int POSB1 = 4;
const int POSB2 = 5;
const int POSB3 = 6;
const int POSB4 = 7;
const int POSB5 = 8;
const int POSB6 = 9;
const int POSB7 = 10;
const int POSB8 = 11;
const int POSST = 12;
const int POSSL = 13;

//Global Variables
byte buttonStatus[NUMBUTTONS];  //array Holds a "Snapshot" of the button status to parse and manipulate
uint8_t flashStyle = 0x00;
uint16_t LEDtimer = 0;
uint8_t TXData[20] = {0x00, 0x14, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  //Test Right 0x10 3rd byte
uint8_t RXData[3] = {0x00, 0x00, 0x00};

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
  for (int i=2; i<13; i++) {TXData[i] = 0x00;}
  
  //Button Packet 1 (usb data array position 2)
  //DPAD Up
  if (buttonStatus[POSUP]) {TXData[2] |= 0x01;}
  //DPAD Down
  if (buttonStatus[POSDN]) {TXData[2] |= 0x02;}
  //DPAD Left
  if (buttonStatus[POSLT]) {TXData[2] |= 0x04;}
  //DPAD Right
  if (buttonStatus[POSRT]) {TXData[2] |= 0x08;}
  
  //Button Start OR Select OR Both (XBOX Logo)
  if (buttonStatus[POSST]&&buttonStatus[POSSL]) {TXData[3] |= 0x04;}
  else if (buttonStatus[POSST]) {TXData[2] |= 0x10;}
  else if (buttonStatus[POSSL]) {TXData[2] |= 0x20;}
  
  //Button Packet 2 (usb data array position 3)
  //Button 1
  if (buttonStatus[POSB1]) {TXData[3] |= 0x40;}
  //Button 2
  if (buttonStatus[POSB2]) {TXData[3] |= 0x80;}
  //Button 3
  if (buttonStatus[POSB3]) {TXData[3] |= 0x02;}
  //Button 4
  if (buttonStatus[POSB4]) {TXData[3] |= 0x01;}
  //Button 5
  if (buttonStatus[POSB5]) {TXData[3] |= 0x10;}
  //Button 6
  if (buttonStatus[POSB6]) {TXData[3] |= 0x20;}
  
  //Triggers (usb data array position 4 and 5)
  //Button 7
  if (buttonStatus[POSB7]) {TXData[4] = 0xFF;}
  //Button 8
  if (buttonStatus[POSB8]) {TXData[5] = 0xFF;}
}

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
/*
Remap for single led
Combine 1 flash and on with 1 on etc
All the rest do a rapid blink and pause
*/
void LEDPattern()
{
  //OFF
  if (flashStyle == 0x00) {digitalWrite(pinOBLED, LOW);}
  //All Blinking, Rotating (1-2-4-3), Blinking*, Slow Blinking*, Alternating (1+4-2+3)*
  if ((flashStyle==0x01)||(flashStyle==0x0A)||(flashStyle==0x0B)||(flashStyle==0x0C)||(flashStyle==0x0D))
  {
    switch (LEDtimer)
    {
      case 0:
        digitalWrite(pinOBLED, HIGH);
      case 500:
        digitalWrite(pinOBLED, LOW);
      case 1000:
        digitalWrite(pinOBLED, HIGH);
      case 1500:
        digitalWrite(pinOBLED, LOW);
      case 2000:
        digitalWrite(pinOBLED, HIGH);
      case 2500:
        digitalWrite(pinOBLED, LOW);
      case 3000:
        LEDtimer = 0;
    }
    LEDtimer++;
  }
  
  //1 Flash and then On and just 1 ON
  if ((flashStyle==0x02)||(flashStyle==0x06))
  {
    switch (LEDtimer)
    {
      case 0:
        digitalWrite(pinOBLED, HIGH);
      case 2000:
        digitalWrite(pinOBLED, LOW);
      case 4000:
        digitalWrite(pinOBLED, HIGH);
      case 6000:
        digitalWrite(pinOBLED, LOW);
      case 8000:
        LEDtimer = 0;
    }
    LEDtimer++;
  }
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
  
  //Update dat joystick SONNNNNN
  FightStick.send(TXData, 12840);
  
  if (FightStick.available() > 0)
  {
    FightStick.recv(RXData, 12840);
    if (RXData[1] == 0x03) {flashStyle = RXData[3]; LEDtimer = 0;}
  }
  
  //Process LED Pattern
  LEDPattern();
}
