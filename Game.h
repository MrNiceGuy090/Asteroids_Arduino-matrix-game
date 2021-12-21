#pragma once
#include "EEPROMUtils.h"
#include "Settings.h"
#include "Joystick.h"
#include "Asteroid.h"
#include "Consts.h"
#include "Sound.h"
#include <LinkedList.h>

extern LiquidCrystal lcd;

class Game{
  int lives;
  int currentLevel;
  bool gameEnded;
  bool isPlayingLevel;
  unsigned long long levelStartingTime;
  unsigned long long lastTimerChangeTime;
  
  unsigned long long lastPowerUpCheckTime;
  bool isPowerUpDisplayed;
  byte powerUp[2];
  unsigned long long powerUpDisplayTime;
  unsigned long long powerUpInitTime;

  int playerSpeed;
  unsigned long long playeBlnkingTime;
  bool isPlayerBlinking = false;
  
  Joystick* js;
  Sound* sound;
  
  int score;
  int asteroidDeploymentInterval;
  int asteroidsSpeed;
  
  byte playerPos[2];
  byte lastPlayerPos[2];
  String playerName;
  
  unsigned long long lastMoved = 0;
  unsigned long long lastDeployedAsteroidTime = 0;
  unsigned long long lastAsteroidMovedTime = 0;

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
    int calculatePlayerSpeed();
    
    void updateDisplay();
    void updateLCD();
    void initScoreAndTimer();
    void updateScoreAndTimer();
    void updateLivesDisplay();
    int checkAndSaveHighscore();
    
    void checkAndProvidePowerUps();
    void providePowerUp();
    void resetPowerUp();
    void activatePowerUp();
    
    void placeNewAsteroid();
    void updateAsteroidsPosition();
    void deleteAsteroid(int pos);
    void deleteAllAsteroids();
    int maxAsteroids();
    
};
