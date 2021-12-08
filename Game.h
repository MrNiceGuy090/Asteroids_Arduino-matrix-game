#pragma once
#include "EEPROMUtils.h"
#include "Settings.h"
#include "Joystick.h"
#include "GameObject.h"
#include "Consts.h"
#include <LinkedList.h>

extern LiquidCrystal lcd;

class Game{
  int lives;
  long time;
  int currentLevel;
  bool gameEnded;
  bool isPlayingLevel;
  long levelStartingTime;
  long lastTimerChangeTime;
  int score;
  Joystick* js;
  
  byte playerPos[2];
  byte lastPlayerPos[2]; 
  String playerName;
  
  unsigned int long long lastMoved = 0;
  unsigned int long long lastDeployedGameObjectTime = 0;
  bool matrixChanged;
  
  bool displayMatrix[matrixSize][matrixSize]{
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
    };;
  LinkedList<GameObject*> gameObjectsList;

  public:
    Game();
    static Game *getInstance();
    void playGame();
    void startLevel(int level);
    void playLevel();
    void endLevel(int leve);
    void endGame();
    void updatePlayerPosition();
    void updateGameObjectsPosition();
    void updateDisplay();
    void updateLCD();
    void updateScoreAndTimer();
    void updateLives();
    void placeNewGameObject();
    int checkAndSaveHighscore();
};
