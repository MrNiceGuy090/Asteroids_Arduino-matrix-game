#include "Asteroid.h"

const int Asteroid::directions[8][2] = {{1,1},{1,0},{1,-1},{0,-1}, {-1,-1},{-1,0},{-1,1}, {0,1}};
const int Asteroid::directionsSize = 8;

Asteroid::Asteroid(){
  //chose randomly a direction of movement for Asteroid
  int randomDirection = random(0,directionsSize);
  movementDirection[0] = directions[randomDirection][0];
  movementDirection[1] = directions[randomDirection][1];

  //generate Asteroid's coordonates
  int randomAxis = random(0,2);
  generateFirstAxis(randomAxis);
  generateSecondAxis(randomAxis^1);

};

// asign a random value to an axis, such that, given the direction,
// the Asteroid can travel at least one square
void Asteroid::generateFirstAxis(int axis){
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

// complement the first axis with generating the second,
// such that the Asteroid spawn on the edges of the matrix
void Asteroid::generateSecondAxis(int axis){  
  int* otherAxisPtr;
  if(axis == 0){
    otherAxisPtr = &x;
  }
  else if(axis == 1){
    otherAxisPtr = &y;
  }
  if(movementDirection[axis] == 1){
    *otherAxisPtr = 0;
  }
  else{
    *otherAxisPtr = matrixSize-1;
  }  
}


void Asteroid::advance(){
  x += movementDirection[0];
  y += movementDirection[1];
};

int Asteroid::getX(){
  return x;
}
int Asteroid::getY(){
  return y;
}
