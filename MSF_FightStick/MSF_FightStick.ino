/*
    Mechanical Squid Factory presents to you:
    Open Source Fight Stick of Awesomesauce
    Compatible w/ PC & PS3
    
    Developer: Zack "Reaper" Littell
    Email: zlittell@gmail.com
    
    Cheap and awesome were the goals so hopefully that works out
    Uses the Teensy-LC
    
    This is used in a box with a joystick, 8 buttons on top, and a start select on the front.
    Also used in a similar setup but with a "hit box" layout.
    Press start and select together for PS home.
    
    Attempted to save PWM capable pins for LEDs if needed in the future.
    But some were sacrificed for ease of layout
    
    Some code exists out there.  I am hoping to combine code to make a complete solution specifically for fight sticks.
    I also was hoping to reduce confusing code and clutter.
    
    Shout outs:
    Teensy Examples - for being awesome
    Hamaluik.com - for the refresher course on descriptors
    jiGGaK @ SlashDev - For compiling some badass code that allows use of the homekey
*/

//Includes
#include <Bounce.h>

//Declarations
const int milliDebounce = 20;  //Debounce time in milliseconds
const int numButtons = 14;  //Number of all buttons
const int numButtonsOnly = 10; //Number of just buttons
//Joystick Pin Declarations
const int pinUP = 5;  //Up on stick is pin 6
const int pinDN = 6;  //Down on stick is pin 7
const int pinLT = 7;  //Left on stick is pin 8
const int pinRT = 8;  //Right on stick is pin 9
//Top Row of Buttons Pin Declarations
const int pinB1 = 9;  //Button 1 is pin 10
const int pinB2 = 10;  //Button 2 is pin 11
const int pinB3 = 11;  //Button 3 is pin 12
const int pinB4 = 12;  //Button 4 is pin 13
//Bottom Row of Buttons Pin Declarations
const int pinB5 = 14;  //Button 5 is pin 14
const int pinB6 = 15;  //Button 6 is pin 15
const int pinB7 = 16;  //Button 7 is pin 16
const int pinB8 = 17;  //Button 8 is pin 17
//Front Panel Pin Declarations
const int pinST = 18;  //Start Button is pin 18
const int pinSL = 19;  //Select Button is pin 19
//Button Position in Array
const int posUP = 0;
const int posDN = 1;
const int posLT = 2;
const int posRT = 3;
const int posB1 = 4;
const int posB2 = 5;
const int posB3 = 6;
const int posB4 = 7;
const int posB5 = 8;
const int posB6 = 9;
const int posB7 = 10;
const int posB8 = 11;
const int posST = 12;
const int posSL = 13;

//Global Variables
byte buttonStatus[numButtons];
int angle = -1;
//Setup Button Debouncing
Bounce joystickUP = Bounce(pinUP, milliDebounce);
Bounce joystickDOWN = Bounce(pinDN, milliDebounce);
Bounce joystickLEFT = Bounce(pinLT, milliDebounce);
Bounce joystickRIGHT = Bounce(pinRT, milliDebounce);
Bounce button1 = Bounce(pinB1, milliDebounce);
Bounce button2 = Bounce(pinB2, milliDebounce);
Bounce button3 = Bounce(pinB3, milliDebounce);
Bounce button4 = Bounce(pinB4, milliDebounce);
Bounce button5 = Bounce(pinB5, milliDebounce);
Bounce button6 = Bounce(pinB6, milliDebounce);
Bounce button7 = Bounce(pinB7, milliDebounce);
Bounce button8 = Bounce(pinB8, milliDebounce);
Bounce buttonSTART = Bounce(pinST, milliDebounce);
Bounce buttonSELECT = Bounce(pinSL, milliDebounce);

//Update Buttons
void buttonUpdate()
{
  if (joystickUP.update()) {buttonStatus[posUP] = joystickUP.fallingEdge();}
  if (joystickDOWN.update()) {buttonStatus[posDN] = joystickDOWN.fallingEdge();}
  if (joystickLEFT.update()) {buttonStatus[posLT] = joystickLEFT.fallingEdge();}
  if (joystickRIGHT.update()) {buttonStatus[posRT] = joystickRIGHT.fallingEdge();}
  if (button1.update()) {buttonStatus[posB1] = button1.fallingEdge();}
  if (button2.update()) {buttonStatus[posB2] = button2.fallingEdge();}
  if (button3.update()) {buttonStatus[posB3] = button3.fallingEdge();}
  if (button4.update()) {buttonStatus[posB4] = button4.fallingEdge();}
  if (button5.update()) {buttonStatus[posB5] = button5.fallingEdge();}
  if (button6.update()) {buttonStatus[posB6] = button6.fallingEdge();}
  if (button7.update()) {buttonStatus[posB7] = button7.fallingEdge();}
  if (button8.update()) {buttonStatus[posB8] = button8.fallingEdge();}
  if (buttonSTART.update()) {buttonStatus[posST] = buttonSTART.fallingEdge();}
  if (buttonSELECT.update()) {buttonStatus[posSL] = buttonSELECT.fallingEdge();}
}

//Process the angle of the joystick
//Process the primary directions first (up and down)
//Nest processing secondaries
//Then process secondaries individually
int joystickProcess()
{
  //Process UP
  if (buttonStatus[posUP])
  {
    //UP + RIGHT = 45 degrees
    if (buttonStatus[posRT]) {return 45;}
    //UP + LEFT = 315 degrees
    else if (buttonStatus[posLT]) {return 315;}
    //Just UP = 0 degress
    else {return 0;}
  }
  //Process DOWN
  else if (buttonStatus[posDN])
  {
    //DOWN + RIGHT = 135
    if (buttonStatus[posRT]) {return 135;}
    //DOWN + LEFT = 225
    else if (buttonStatus[posLT]) {return 225;}
    //Just DOWN = 180
    else {return 180;}
  }
  //Process Left = 270 degrees
  else if (buttonStatus[posLT]) {return 270;}
  //Process Right = 90 degrees
  else if (buttonStatus[posRT]) {return 90;}
  //Nothing pressed center the joystick with -1
  else {return -1;}
}

//Setup
void setup() 
{
    //Configure joystick for manual send more.  Allows precise control over when updates are sent
    Joystick.useManualSend(true);
    
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
    
    //Set Extra Shit to 0
    Joystick.X(0);
    Joystick.Y(0);
    Joystick.Z(0);
    Joystick.Zrotate(0);
    Joystick.sliderLeft(0);
    Joystick.sliderRight(0);
}

void loop() 
{
  buttonUpdate();
  
  //For Loop to go through and load all the buttons from the array into the joystick to be sent
  for (int i=0; i<numButtonsOnly; i++)
  {
    //Update Button
    //First # is the button number, second is the value to use
    //Skip 4 places to bypass the directions)
    Joystick.button(i+1, buttonStatus[i+4]);
  }
  
  //Get the angle of the joystick and set that
  angle = joystickProcess();
  Joystick.hat(angle);

  //Update dat joystick SONNNNNN
  Joystick.send_now();
  
  //Delays are lame
}
