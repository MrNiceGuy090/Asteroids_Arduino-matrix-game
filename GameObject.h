#pragma once
#include "Consts.h"

class GameObject{
  int x;
  int y;
  int movementDirection[2];
  static const int directions[8][2];
  static const int directionsSize;
  
  public:
    GameObject();
    void advance();
    void generatePosition(int axis);
    int getX();
    int getY();
};
