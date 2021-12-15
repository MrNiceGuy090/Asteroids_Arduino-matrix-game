#include "Joystick.h"
#include "Arduino.h"

Joystick *Joystick::instance = 0;

extern const byte pinSW;
extern const byte pinX;
extern const byte pinY;

Joystick::Joystick() {
  xValue = 0;
  yValue = 0;
  joyMoved = false;
  swState = HIGH;
  swReading = HIGH;
  prevSwReading = HIGH;
  debounceTime = 0;
}

Joystick* Joystick::getInstance(){
  if(!instance){
    instance = new Joystick;
  }
  return instance;
}

void Joystick::read() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  if(xValue >= minThreshold && xValue <= maxThreshold && yValue >= minThreshold && yValue <= maxThreshold){
    joyMoved = false;
  }
}

bool Joystick::hasButtonBeenPressed(){
  swReading = digitalRead(pinSW);
  if(swReading != prevSwReading){
    debounceTime = millis();
  }
  prevSwReading = swReading;
  if(millis() - debounceTime > debounceInterval ){
    if(swReading != swState){
        swState = swReading;
        // case for falling
        if(swReading == 0) {
          tone(buzzerPin, joystickPressBuzzerFreq,joystickPressBuzzerDuration);
          return true;
        }
    }
  } 
  return false;
}

bool Joystick::isLeft() { 
  read();
  return yValue < minThreshold;
}
bool Joystick::isRight() { 
  read();
  return yValue > maxThreshold; 
}
bool Joystick::isDown() {
  read();
  return xValue < minThreshold;  
}
bool Joystick::isUp() { 
  read();
  return xValue > maxThreshold; 
}

bool Joystick::turnedLeftOnce() { 
  read();
  bool value = Joystick::isLeft() && joyMoved == false;
  if(value) joyMoved = true;
  return value; 
}
bool Joystick::turnedRightOnce() {
  read();
  bool value = Joystick::isRight() && joyMoved == false; 
  if(value) joyMoved = true;
  return value;
}
bool Joystick::turnedDownOnce() { 
  read();
  bool value = Joystick::isDown() && joyMoved == false; 
  if(value) joyMoved = true;
  return value;
}
bool Joystick::turnedUpOnce() { 
  read();
  bool value = Joystick::isUp() && joyMoved == false; 
  if(value) joyMoved = true;
  return value;
}

int Joystick::getX() { return xValue; }
int Joystick::getY() { return yValue; }
