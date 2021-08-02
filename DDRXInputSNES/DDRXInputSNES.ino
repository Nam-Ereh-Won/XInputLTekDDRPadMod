#include <XInput.h>


const boolean UseTriggerButtons = true;

// Button Pins
const int pinA =        4;  //DDR Down Arrow
const int pinB =        5;  //DDR Right Arrow
const int pinX =        6;  //DDR Left Arrow
const int pinY =        7;  //DDR Up Arrow
const int pinStart =    8;  //For Pads with Start/Back
const int pinBack =     9;  //For Pads with Start/Back

// LED Pins
const int pinLEDA =     21;
const int pinLEDB =     20;
const int pinLEDX =     19;
const int pinLEDY =     18;

// SNES/NES Pins
const int snesData =    15;
const int snesLatch =   14;
const int snesClock =   16;

// Joy/DPad Toggle pin
const int dPadToggle =  2;

void setup(){
  
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinX, INPUT_PULLUP);
  pinMode(pinY, INPUT_PULLUP);
  pinMode(pinStart, INPUT_PULLUP);
  pinMode(pinBack, INPUT_PULLUP);
  
  pinMode(pinLEDA, OUTPUT);
  pinMode(pinLEDB, OUTPUT);
  pinMode(pinLEDX, OUTPUT);
  pinMode(pinLEDY, OUTPUT);
  
  
  pinMode(snesData,INPUT_PULLUP);
  pinMode(snesLatch,OUTPUT);
  pinMode(snesClock,OUTPUT);
  
  pinMode(dPadToggle,INPUT_PULLUP);
  
  // Set Clock and Latch Low to start
  digitalWrite(snesClock,LOW);
  digitalWrite(snesLatch,LOW);
  
  XInput.setAutoSend(false); 
  
  XInput.begin();
  
}

void loop(){

  // Gotta hold onto these for the LEDs
  boolean buttonA = !digitalRead(pinA);
  boolean buttonB = !digitalRead(pinB);
  boolean buttonX = !digitalRead(pinX);
  boolean buttonY = !digitalRead(pinY);
  
  // Set LEDs
  digitalWrite(pinLEDA,buttonA);
  digitalWrite(pinLEDB,buttonB);
  digitalWrite(pinLEDX,buttonX);
  digitalWrite(pinLEDY,buttonY);
  
  // Set XInput buttons
  XInput.setButton(BUTTON_A, buttonA);
  XInput.setButton(BUTTON_B, buttonB);
  XInput.setButton(BUTTON_X, buttonX);
  XInput.setButton(BUTTON_Y, buttonY);
  
  snesBS();
  
  // Allows for either Pad or Controller Start/Back to work properly
  XInput.setButton(BUTTON_START, XInput.getButton(BUTTON_START)||!digitalRead(pinStart));
  XInput.setButton(BUTTON_BACK, XInput.getButton(BUTTON_BACK)||!digitalRead(pinBack));
  
  XInput.send();
  
}


void snesBS(){
  
  // Toggle latch pin to hold input
  digitalWrite(snesLatch,HIGH);
  digitalWrite(snesLatch,LOW);
  
  // Cycle through the SNES/NES buttons. Delays added on dpad just in case. may not be needed.
  delayMicroseconds(5);
  boolean xDown = !digitalRead(snesData);  // SNES B Button | X-Down
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  delayMicroseconds(5);
  boolean xLeft = !digitalRead(snesData);  // SNES Y Button | X-Left
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  XInput.setButton(BUTTON_BACK, !digitalRead(snesData));  // SNES Select Button
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  XInput.setButton(BUTTON_START, !digitalRead(snesData));  // SNES Start Button
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  boolean dUp = !digitalRead(snesData); // SNES DPad Up
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  boolean dDown = !digitalRead(snesData); // SNES DPad Down
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  boolean dLeft = !digitalRead(snesData); // SNES DPad Left
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  boolean dRight = !digitalRead(snesData); // SNES DPad Right
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  delayMicroseconds(5);
  boolean xRight = !digitalRead(snesData);  // SNES A Button | X-Right
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  delayMicroseconds(5);
  boolean xUp = !digitalRead(snesData);  // SNES X Button | X-Up
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  XInput.setButton(BUTTON_LB, !digitalRead(snesData));  // SNES L Button
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  XInput.setButton(BUTTON_RB, !digitalRead(snesData));  // SNES R Button
  
  // Switches DPad between Left Stick and DPad based on toggle switch and clears the unused one.
  if(digitalRead(dPadToggle)){
    XInput.setDpad(dUp, dDown, dLeft, dRight);
    XInput.setJoystick(JOY_LEFT, false, false, false, false);
  } else {
    XInput.setJoystick(JOY_LEFT, dUp, dDown, dLeft, dRight);
    XInput.setDpad(false, false, false, false);
  }
  // Uses Controller Face buttons as right stick. May add some additional logic to allow Dpad, Face Buttons, and DDR Arrows to function as any combination of Sticks, DPad, and Face buttons.
  XInput.setJoystick(JOY_RIGHT, xUp, xDown, xLeft, xRight);
  
}
