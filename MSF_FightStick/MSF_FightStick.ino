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

//Declarations
//Joystick Pin Declarations
const int UP = 5;  //Up on stick is pin 6
const int DN = 6;  //Down on stick is pin 7
const int LT = 7;  //Left on stick is pin 8
const int RT = 8;  //Right on stick is pin 9
//Top Row of Buttons Pin Declarations
const int B1 = 9;  //Button 1 is pin 10
const int B2 = 10;  //Button 2 is pin 11
const int B3 = 11;  //Button 3 is pin 12
const int B4 = 12;  //Button 4 is pin 13
//Bottom Row of Buttons Pin Declarations
const int B5 = 14;  //Button 5 is pin 14
const int B6 = 15;  //Button 6 is pin 15
const int B7 = 16;  //Button 7 is pin 16
const int B8 = 17;  //Button 8 is pin 17
//Front Panel Pin Declarations
const int ST = 18;  //Start Button is pin 18
const int SL = 19;  //Select Button is pin 19


//Setup
void setup() 
{
    //Configure joystick for manual send more.  Allows precise control over when updates are sent
    Joystick.useManualSend(true);
    
    //Configure the direction of the pins
    //All inputs with internal pullups enabled
    pinMode(UP, INPUT_PULLUP);
    pinMode(DN, INPUT_PULLUP);
    pinMode(LT, INPUT_PULLUP);
    pinMode(RT, INPUT_PULLUP);
    pinMode(B1, INPUT_PULLUP);
    pinMode(B2, INPUT_PULLUP);
    pinMode(B3, INPUT_PULLUP);
    pinMode(B4, INPUT_PULLUP);
    pinMode(B5, INPUT_PULLUP);
    pinMode(B6, INPUT_PULLUP);
    pinMode(B7, INPUT_PULLUP);
    pinMode(B8, INPUT_PULLUP);
    pinMode(ST, INPUT_PULLUP);
    pinMode(SL, INPUT_PULLUP);
    
    
}

void loop() {
  // put your main code here, to run repeatedly:

}
