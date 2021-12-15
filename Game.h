#pragma once
#include "EEPROMUtils.h"
#include "Settings.h"
#include "Joystick.h"
#include "Asteroid.h"
#include "Shield.h"
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
  
  long lastPowerUpCheckTime;
  bool isPowerUpDisplayed;
  byte powerUp[2];
  long powerUpDisplayTime;
  long powerUpInitTime;
  long playeBlnkingTime;
  bool isPlayerBlinking = false;
  
  int score;
  Joystick* js;
  int asteroidDeploymentInterval;
  int asteroidsSpeed;
  
  byte playerPos[2];
  byte lastPlayerPos[2];
  String playerName;
  
  unsigned int long long lastMoved = 0;
  unsigned int long long lastDeployedAsteroidTime = 0;
  unsigned int long long lastAsteroidMovedTime = 0;

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
  LinkedList<Asteroid*> asteroidsList;

  public:
    Game();
    static Game *getInstance();
    void playGame();
    void startLevel(int level);
    void playLevel();
    void endLevel(int leve);
    void endGame();
    void updatePlayerPosition();
    void updateAsteroidsPosition();
    void updateDisplay();
    void updateLCD();
    void updateScoreAndTimer();
    void updateLives();
    void placeNewAsteroid();
    int checkAndSaveHighscore();
    void checkAndProvidePowerUps();
    void providePowerUp();
    void resetPowerUp();
    void activatePowerUp();
    int maxAsteroids();
};
