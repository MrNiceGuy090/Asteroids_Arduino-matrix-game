#include "GameObject.h"

const int GameObject::directions[8][2] = {{1,1},{1,0},{1,-1},{0,-1}, {-1,-1},{-1,0},{-1,1}, {0,1}};
const int GameObject::directionsSize = 8;

GameObject::GameObject(){
  int randomDirection = random(0,directionsSize);
  movementDirection[0] = directions[randomDirection][0];
  movementDirection[1] = directions[randomDirection][1];
  int randomAxis = random(0,1);
  generatePosition(randomAxis);
  int* otherAxisPtr;
  
  if(randomAxis == 0){
    otherAxisPtr = &y;
  }
  else if(randomAxis == 1){
    otherAxisPtr = &y;
  }
  if(movementDirection[randomAxis] == 1){
   *otherAxisPtr = matrixSize-1;
  }
  else{
    *otherAxisPtr = 0;
  }
};

void GameObject::generatePosition(int axis){
  int* axisPtr;
  if(axis == 0){
    axisPtr = &x;
  }
  else if(axis == 1){
    axisPtr = &y;
  }
  if(movementDirection[axis] > 0){
    *axisPtr = random(0,matrixSize-2);
  }
  else if(movementDirection[axis] < 0 ){
    *axisPtr = random(1,matrixSize-1);
  }
  else
    *axisPtr = random(0, matrixSize-1);
  
}

void GameObject::advance(){
  x += movementDirection[0];
  y += movementDirection[1];
};

int GameObject::getX(){
  return x;
}
int GameObject::getY(){
  return y;
}
