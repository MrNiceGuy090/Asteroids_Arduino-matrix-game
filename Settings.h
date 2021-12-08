#pragma once
#include<LiquidCrystal.h>
#include <EEPROM.h>
#include "EEPROMUtils.h"
#include "Joystick.h"
#include "LCDUtils.h"
#include "LedControl.h" 
#include "Consts.h"

extern LiquidCrystal lcd;
extern LedControl lc;

class Settings{
  static Settings *instance;
  Joystick *js;
  String name;
  int value;
  int currentPosition;
  const int blinkingInterval = 300;
  unsigned int long long  blinkingTime = 0;
  char toWriteCharacter;
  
  bool changingName;
  bool changingLevel;
  bool changingContrast;
  bool changingLCDBrightness;
  bool changingMatrixBrightness;
  
  Settings();
  String readNameFromEEPROM(int maxLengthName);
  void saveNameToEEPROM(String name, int maxLengthName);
  public:
    static Settings *getInstance();
    void changeName();
    void changeLevel();
    void changeContrast();
    void changeLCDBrightness();
    void changeMatrixBrightness();
    bool isChangingName();
    bool isChangingLevel();
    bool isChangingContrast();
    bool isChangingLCDBrightness();
    bool isChangingMatrixBrightness();
    int getMaxLevel();
};
