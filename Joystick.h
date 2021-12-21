#pragma once
#include "Consts.h"
#include "Sound.h"

class Joystick {
  static Joystick* instance;

  Sound* sound;
  
  int xValue;
  int yValue;
  bool joyMoved;
  bool swState;
  bool swReading;
  bool prevSwReading;
  unsigned int long long debounceTime = 0;
  
  Joystick();
  public:
    static Joystick *getInstance();
    
    void read();
    bool hasButtonBeenPressed();
    
    bool isLeft();
    bool isRight();
    bool isDown();
    bool isUp();
    
    bool turnedLeftOnce();
    bool turnedRightOnce();
    bool turnedDownOnce();
    bool turnedUpOnce();
    
    int getX();
    int getY();
};
