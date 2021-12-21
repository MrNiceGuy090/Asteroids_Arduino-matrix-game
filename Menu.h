#pragma once
#include<LiquidCrystal.h>
#include "Settings.h"
#include "Consts.h"
#include "Sound.h"

extern LiquidCrystal lcd;
extern char* state;

class Menu {
  static Menu *instance;
  int selectingOption;
  Settings *settings;
  Sound *sound;
  
  int textStartingPos;
  
  Menu();
  void displayMenuWindow(int selectingOption, int optionsSize, const char** options);
  void displayMenuOptions();
  void displayHighscore();
  void displaySettings();
  void displayAboutInfo();
  
  public:
    static Menu *getInstance();
    Settings* getSettings();
    
    void show();
    void resetMenu();
    
    void increaseSelectingOption();
    void decreaseSelectingOption();
    void scrollTextLeft();
    void scrollTextRight();
    
    char* getState();
    void setState(char* value);
    int getSelectingOption();
    int getMaxSelectingOption();
};
