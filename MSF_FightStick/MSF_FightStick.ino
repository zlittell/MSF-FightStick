/*
    Mechanical Squid Factory presents to you:
    Open Source Fight Stick of Awesomesauce
    Compatible w/ PC
    
    Developer: Zack "Reaper" Littell
    Email: zlittell@gmail.com
    
    Cheap and awesome were the goals so hopefully that works out
    Uses the Teensy-LC
    
    This is used in a box with a joystick, 8 buttons on top, and a start select on the front.
    Also used in a similar setup but with a "hit box" layout.
    Press start and select together for PS home.
    
    Attempted to save PWM capable pins for LEDs if needed in the future.
    But some were sacrificed for ease of layout
   
    Obviously if you play on more than a PC this is not an ideal solution for you
    We happen to only play PC as of right now and a PS360+ was just not in the build budget
    however ~$10 teensyLC made the cut
    
    Would love to eventual move to the FreeScale KDS and off the arduino
    Also would love to eventually add in PS3 support
    
    Shout outs:
    Teensy Examples - for being awesome
    Hamaluik.com - for allowing me to not murder the arduino "IDE" out of frustration of hidden "magic"
*/

//Includes
#include <Bounce.h>

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

//Array holds locations of button status to convert to correct order working with MKX on PC
//Uses a mix of PS3 and XBOX360 Button mapping lol.  Don't ask why.
//1)Triangle, 2)Circle, 3)Cross, 4)Square, 5)L1, 6)R1, 7)L2, 8)R2, 9)Select, 10)Start
//Our current order has buttons wired as 1234, 5678
//We need that to match Square Triangle R1 L1, Cross Circle R2 L2
//Has been swapped to use the XBOX360 Layout because PC be cray cray
//1 4 6 5
//2 3 8 7
//10 9
const int CORRECTORDER[NUMBUTTONSONLY] = {1,4,6,5,2,3,8,7,10,9};

//Global Variables
byte buttonStatus[NUMBUTTONS];  //array Holds a "Snapshot" of the button status to parse and manipulate
int stickPosition = 15;  //Create a variable to hold the position of the stick
uint8_t usbData[8] = {0,0,0,127,127,127,127,127};  //Array to hold correctly formatted USB data that needs to be sent fill with 127 to center extra "fake" inputs

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

//Process all the inputs and load them into the correct bytes to be loaded to usb
//Byte1 = 8 buttons, Byte2 5 buttons and 3 fillers, Byte3 DPAD and 4 fillers
//(Byte1) B8,B7,B6,B5,B4,B3,B2,B1
//(Byte2) x,x,x,B13,B12,B11,B10,B9
//(Byte3) x,x,x,x,D,D,D,D
//The last bytes are all for X Y Z Rx Ry and should be set to 0 always
//These were included because MK X was not picking up on a controller being plugged in
void processInputs()
{
  //Clear USB Data Holder Bytes
  usbData[0]=0x00;
  usbData[1]=0x00;
  usbData[2]=0x00;
  
  //Set up a variable to hold the correct button values
  int rearrangedButtons[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  
  //Move button statuses into the correct order
  for (int i=0; i<NUMBUTTONSONLY; i++)
  {
    //look at button number from index i of correctorder array
    //then set that button in the array for rearranged buttons
    //to its value from button status
    //sorry this is so crazy
    int tempIndex = CORRECTORDER[i] - 1;
    rearrangedButtons[tempIndex] = buttonStatus[i+4];
  }
  
  //Oh bitmasking here we go
  //First byte (B8-B1)
  for (int i=0; i<8; i++)
  {
    rearrangedButtons[i] <<= i;
    usbData[0] |= rearrangedButtons[i];
  }
  
  //Second byte
  for (int i=0; i<8; i++)
  {
    rearrangedButtons[i+8] <<= i;
    usbData[1] |= rearrangedButtons[i+8];
  }
  
  
  //Process the angle of the joystick
  //Process the primary directions first (up and down)
  //Nest processing secondaries
  //Then process secondaries individually
  //Variable to hold Position of Hat
  //Up + Right = 1
  if (buttonStatus[POSUP] & buttonStatus[POSRT]) {stickPosition = 1;}
  //Up + Left = 7
  else if (buttonStatus[POSUP] & buttonStatus[POSLT]) {stickPosition = 7;}
  //Up = 0
  else if (buttonStatus[POSUP]) {stickPosition = 0;}
  //Down + Right = 3
  else if (buttonStatus[POSDN] & buttonStatus[POSRT]) {stickPosition = 3;}
  //Down + Left = 5
  else if (buttonStatus[POSDN] & buttonStatus[POSLT]) {stickPosition = 5;}
  //Down = 4
  else if (buttonStatus[POSDN]) {stickPosition = 4;}
  //Left = 6
  else if (buttonStatus[POSLT]) {stickPosition = 6;}
  //Right = 2
  else if (buttonStatus[POSRT]) {stickPosition = 2;}
  //nothing
  else {stickPosition = 15;}
  
  //Man I hope they can just be equals
  usbData[2] = stickPosition;
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
  
  //Update dat joystick SONNNNNN
  FightStick.send(usbData);
}
