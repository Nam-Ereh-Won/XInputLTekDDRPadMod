#include <XInput.h>
#include <EEPROM.h>

const boolean UseTriggerButtons = true;

enum DirectionType : uint8_t {
  LEFT_STICK = 1,
  RIGHT_STICK = 2,
  DPAD = 4,
  FACE_BUTTONS = 8,
};

// Button Pins
const int pinDown =        4;  //DDR Down Arrow
const int pinRight =        5;  //DDR Right Arrow
const int pinLeft =        6;  //DDR Left Arrow
const int pinUp =        7;  //DDR Up Arrow
const int pinStart =    8;  //For Pads with Start/Back
const int pinBack =     9;  //For Pads with Start/Back

// LED Pins
const int pinLEDDown =     21;
const int pinLEDRight =     20;
const int pinLEDLeft =     19;
const int pinLEDUp =     18;

// SNES/NES Pins
const int snesData =    15;
const int snesLatch =   14;
const int snesClock =   16;

DirectionType dPadMode, faceMode, ddrMode;

boolean ddrDown, ddrRight, ddrLeft, ddrUp, xDown, xRight, xLeft, xUp, dDown, dRight, dLeft, dUp;

void setup(){
  
  pinMode(pinDown, INPUT_PULLUP);
  pinMode(pinRight, INPUT_PULLUP);
  pinMode(pinLeft, INPUT_PULLUP);
  pinMode(pinUp, INPUT_PULLUP);
  pinMode(pinStart, INPUT_PULLUP);
  pinMode(pinBack, INPUT_PULLUP);
  
  pinMode(pinLEDDown, OUTPUT);
  pinMode(pinLEDRight, OUTPUT);
  pinMode(pinLEDLeft, OUTPUT);
  pinMode(pinLEDUp, OUTPUT);
  
  
  pinMode(snesData,INPUT_PULLUP);
  pinMode(snesLatch,OUTPUT);
  pinMode(snesClock,OUTPUT);
  
  // Set Clock and Latch Low to start
  digitalWrite(snesClock,LOW);
  digitalWrite(snesLatch,LOW);

  byte dPadEEP = EEPROM.read(0);
  if (isModeValid(dPadEEP)){
    dPadMode = dPadEEP;
  } else {
    dPadMode = DPAD;
  }
  byte faceEEP = EEPROM.read(1);
  if (isModeValid(faceEEP)){
    faceMode = faceEEP;
  } else {
    faceMode = RIGHT_STICK;
  }
  byte ddrEEP = EEPROM.read(2);
  if (isModeValid(ddrEEP)){
    ddrMode = ddrEEP;
  } else {
    ddrMode = FACE_BUTTONS;
  }
  
  XInput.setAutoSend(false); 
  
  XInput.begin();
  
}

boolean isModeValid(int modeVal){
  switch(modeVal){
    case LEFT_STICK:
    case RIGHT_STICK:
    case DPAD:
    case FACE_BUTTONS:
      return true;
      break;
    default:
      return false;
      break;
  }
}

void loop(){

  // Gotta hold onto these for the LEDs
  ddrDown = !digitalRead(pinDown);
  ddrRight = !digitalRead(pinRight);
  ddrLeft = !digitalRead(pinLeft);
  ddrUp = !digitalRead(pinUp);
  
  // Set LEDs
  digitalWrite(pinLEDDown,ddrDown);
  digitalWrite(pinLEDRight,ddrRight);
  digitalWrite(pinLEDLeft,ddrLeft);
  digitalWrite(pinLEDUp,ddrUp);
  
  // Set DDR Arrows
  
  
  snesBS();
  
  if(faceMode==ddrMode && dPadMode==ddrMode){//all three set to same mode
    DirectionSet(dUp||ddrUp||xUp,dDown||ddrDown||xDown,dLeft||ddrLeft||xLeft,dRight||ddrRight||xRight,dPadMode);
  } else if (faceMode==ddrMode){//face and ddr set to same mode
    DirectionSet(xUp||ddrUp,xDown||ddrDown,xLeft||ddrLeft,xRight||ddrRight,faceMode);
    DirectionSet(dUp,dDown,dLeft,dRight,dPadMode);
  } else if (dPadMode==ddrMode){//dpad and ddr set to same mode
    DirectionSet(dUp||ddrUp,dDown||ddrDown,dLeft||ddrLeft,dRight||ddrRight,dPadMode);
    DirectionSet(xUp,xDown,xLeft,xRight,faceMode);
  } else if (dPadMode==faceMode){//dpad and face set to same mode
    DirectionSet(xUp||dUp,xDown||dDown,xLeft||dLeft,xRight||dRight,dPadMode);
    DirectionSet(ddrUp,ddrDown,ddrLeft,ddrRight,ddrMode);
  } else {//all modes different
    DirectionSet(xUp,xDown,xLeft,xRight,faceMode);
    DirectionSet(dUp,dDown,dLeft,dRight,dPadMode);
    DirectionSet(ddrUp,ddrDown,ddrLeft,ddrRight,ddrMode);
  }
  // Allows for either Pad or Controller Start/Back to work properly
  XInput.setButton(BUTTON_START, XInput.getButton(BUTTON_START)||!digitalRead(pinStart));
  XInput.setButton(BUTTON_BACK, XInput.getButton(BUTTON_BACK)||!digitalRead(pinBack));

  // If L+R+Select Pressed, start mode change
  if (XInput.getButton(BUTTON_LB) && XInput.getButton(BUTTON_RB) && XInput.getButton(BUTTON_BACK) ) {
    DirectionModeChange();
  }
  
  XInput.send();
  
}

/* Changes mode of directional inputs as follows
 *  Left - Left Stick
 *  Right - Right Stick
 *  Down - D-pad
 *  Up - Face buttons
 */
void DirectionModeChange(){
  XInput.releaseAll();
  int ddr = ddrLeft+(ddrRight<<1)+(ddrDown<<2)+(ddrUp<<3);
  int face = xLeft+(xRight<<1)+(xDown<<2)+(xUp<<3);
  int dpad = dLeft+(dRight<<1)+(dDown<<2)+(dUp<<3);
  
  if (isModeValid(dpad) && dPadMode != dpad){
    dPadMode = dpad;
    EEPROM.update(0,dpad);
  }

  if(isModeValid(face) && faceMode != face){
    faceMode = face;
    EEPROM.update(1,face);
  }

  if(isModeValid(ddr) && ddrMode != ddr){
    ddrMode = ddr;
    EEPROM.update(2,ddr);
  }


}

void DirectionSet(boolean up, boolean down, boolean left, boolean right, DirectionType mode ){
  switch(mode){
    case LEFT_STICK:
      XInput.setJoystick(JOY_LEFT, up, down, left, right);
      break;
    case RIGHT_STICK:
      XInput.setJoystick(JOY_RIGHT, up, down, left, right);
      break;
    case DPAD:
      XInput.setDpad(up, down, left, right);
      break;
    case FACE_BUTTONS:
        XInput.setButton(BUTTON_Y, up);
        XInput.setButton(BUTTON_A, down);
        XInput.setButton(BUTTON_X, left);
        XInput.setButton(BUTTON_B, right);
      break;
  }
}

void snesBS(){
  
  // Toggle latch pin to hold input
  digitalWrite(snesLatch,HIGH);
  digitalWrite(snesLatch,LOW);
  
  // Cycle through the SNES/NES buttons. 
  xDown = !digitalRead(snesData);  // SNES B Button | X-Down
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  xLeft = !digitalRead(snesData);  // SNES Y Button | X-Left
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  XInput.setButton(BUTTON_BACK, !digitalRead(snesData));  // SNES Select Button
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  XInput.setButton(BUTTON_START, !digitalRead(snesData));  // SNES Start Button
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  dUp = !digitalRead(snesData); // SNES DPad Up
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  dDown = !digitalRead(snesData); // SNES DPad Down
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  dLeft = !digitalRead(snesData); // SNES DPad Left
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  dRight = !digitalRead(snesData); // SNES DPad Right
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  xRight = !digitalRead(snesData);  // SNES A Button | X-Right
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  xUp = !digitalRead(snesData);  // SNES X Button | X-Up
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  XInput.setButton(BUTTON_LB, !digitalRead(snesData));  // SNES L Button
  digitalWrite(snesClock,HIGH);
  digitalWrite(snesClock,LOW);
  XInput.setButton(BUTTON_RB, !digitalRead(snesData));  // SNES R Button
  
}
