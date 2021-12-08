#pragma once
#include<LiquidCrystal.h>
#include "Settings.h"
#include "Consts.h"

extern LiquidCrystal lcd;
extern String state;

class Menu {
  static Menu *instance;
  int selectingOption;
  Settings *settings;
  int textStartingPos;
  
  Menu();
  void displayMenuWindow(int selectingOption, int optionsSize, String* options);
  void displayMenuOptions();
  void displayHighscore();
  void displaySettings();
  void displayAboutInfo();
  
  public:
    static Menu *getInstance();
    void show();
    void resetMenu();
    void increaseSelectingOption();
    void decreaseSelectingOption();
    void scrollTextLeft();
    void scrollTextRight();
    
    String getState();
    void setState(String value);
    int getSelectingOption();
    Settings* getSettings();
};
