#pragma once
#include "Consts.h"

class Asteroid{
  int x;
  int y;
  int movementDirection[2];
  static const int directions[8][2];
  static const int directionsSize;
  
  public:
    Asteroid();
    void advance();
    void generateFirstAxis(int axis);
    void generateSecondAxis(int axis);
    int getX();
    int getY();
};
