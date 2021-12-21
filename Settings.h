#pragma once
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "EEPROMUtils.h"
#include "Joystick.h"
#include "LCDUtils.h"
#include "LedControl.h" 
#include "Consts.h"
#include "Sound.h"

extern LiquidCrystal lcd;
extern LedControl lc;

class Settings{
  static Settings *instance;
  Joystick *js;
  Sound *sound;
  
  String name;
  int value;
  
  int currentPosition;
  char toWriteCharacter;
  
  const int blinkingInterval = 300;
  unsigned int long long  blinkingTime = 0;
  
  bool changingName;
  bool changingLevel;
  bool changingPlayerSpeed;
  bool changingContrast;
  bool changingLCDBrightness;
  bool changingMatrixBrightness;
  bool changingSound;
  bool resetingHighscore;
  
  Settings();
  public:
    static Settings *getInstance();
    void changeName();
    void changeLevel();
    void changePlayerSpeed();
    void changeContrast();
    void changeLCDBrightness();
    void changeMatrixBrightness();
    void changeSound();
    void resetHighscore();
    
    bool isChangingName();
    bool isChangingLevel();
    bool isChangingPlayerSpeed();
    bool isChangingContrast();
    bool isChangingLCDBrightness();
    bool isChangingMatrixBrightness();
    bool isChangingSound();
    bool isResetingHighscore();
};
