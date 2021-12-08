#pragma once

class Joystick {
  static Joystick* instance;
  
  int xValue;
  int yValue;
  bool joyMoved;
  bool swState;
  bool swReading;
  bool prevSwReading;
  const int minThreshold = 400;
  const int maxThreshold = 600;
  const int debounceInterval = 100;
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
