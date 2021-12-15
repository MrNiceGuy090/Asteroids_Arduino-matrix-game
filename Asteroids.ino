#include "LedControl.h" //  need the library
#include "Menu.h"
#include "Joystick.h"
#include "LCDUtils.h"
#include "Game.h"
#include "Consts.h"
#include<LiquidCrystal.h>

LiquidCrystal lcd(RS,enable,d4,d5,d6,d7);
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER
const String states[10] = {"MainMenu", "Game", "Highscores", "SettingsMenu", "About", "EditName", "EditLevel", "EditContrast", "EditLCDBrightness", "EditGameBrightness"};

String state;
Menu *menu= Menu::getInstance();
Joystick *js= Joystick::getInstance();
Game *game;

void setup()
{
  Serial.begin(9600);
  pinMode(pinSW,INPUT_PULLUP);
  
  state = states[0];
  LCDSetup();
  delay(2000);
  menu->show();
  matrixSetup();
}
void loop() {
  if(state == "MainMenu"){
    if(js->hasButtonBeenPressed()){
      state = mainMenuStates[menu->getSelectingOption()];
      menu->resetMenu();
      menu->show();
    }
    if(js->turnedDownOnce()){
      menu->increaseSelectingOption();
      menu->show();
    }
    else if(js->turnedUpOnce()){
      menu->decreaseSelectingOption();
      menu->show();
    }
  }
  else if(state == "Game"){ 
    if(game == NULL)
      game = new Game();
    game->playGame();
    if(state == "MainMenu"){
      delete game;
      game = NULL;
      menu->resetMenu();
      menu->show();
    }
  }
  else if(state == "Highscores"){    
    if(js->hasButtonBeenPressed() && menu->getSelectingOption() == 3){
      state = "MainMenu";
      menu->resetMenu();
      menu->show();
    }    
    if(js->turnedDownOnce()){
      menu->increaseSelectingOption();
      menu->show();
    }
    else if(js->turnedUpOnce()){
      menu->decreaseSelectingOption();
      menu->show();
    }
  }
  else if(state == "SettingsMenu"){
    if(js->hasButtonBeenPressed()){
      if(menu->getSelectingOption() == 0){
        state = "EditName";
      }
      else if(menu->getSelectingOption() == 1){
        state = "EditLevel";
      }
      else if(menu->getSelectingOption() == 2){
        state = "EditContrast";
      }
      else if(menu->getSelectingOption() == 3){
        state = "EditLCDBrightness";
      }      
      else if(menu->getSelectingOption() == 4){
        state = "EditMatrixBrightness";
      }
      else if(menu->getSelectingOption() == 5){
        state = "MainMenu";
        menu->resetMenu();
        menu->show();
      }
    }
    if(js->turnedDownOnce()){
      menu->increaseSelectingOption();
      menu->show();
    }
    else if(js->turnedUpOnce()){
      menu->decreaseSelectingOption();
      menu->show();
    }
  }
  else if(state == "About"){    
    if(js->hasButtonBeenPressed()){
      state = "MainMenu";
      menu->resetMenu();
      menu->show();
    }
    if(js->isLeft()){
      menu->scrollTextLeft();
      menu->show();
    }
    else if(js->isRight()){
      menu->scrollTextRight();
      menu->show();
    }
  }
  else if(state == "EditName"){
      menu->getSettings()->changeName();
      if(!menu->getSettings()->isChangingName()){
        state = "SettingsMenu";
        menu->resetMenu();
        menu->show();
      }
  }
  else if(state == "EditLevel"){
      menu->getSettings()->changeLevel();      
      if(!menu->getSettings()->isChangingLevel()){
        state = "SettingsMenu";
        menu->resetMenu();
        menu->show();
      }
  }
  else if(state == "EditContrast"){
      menu->getSettings()->changeContrast();      
      if(!menu->getSettings()->isChangingContrast()){
        state = "SettingsMenu";
        menu->resetMenu();
        menu->show();
      }    
  }  
  else if(state == "EditLCDBrightness"){
      menu->getSettings()->changeLCDBrightness();      
      if(!menu->getSettings()->isChangingLCDBrightness()){
        state = "SettingsMenu";
        menu->resetMenu();
        menu->show();
      }    
  }  
  else if(state == "EditMatrixBrightness"){
      menu->getSettings()->changeMatrixBrightness();      
      if(!menu->getSettings()->isChangingMatrixBrightness()){
        state = "SettingsMenu";
        menu->resetMenu();
        menu->show();
      }    
  }
  
}

void matrixSetup(){
  lc.shutdown(0, false); 
  int matrixBrightness = readFromEEPROM<int>(matrixBrightnessAddrs);  
  matrixBrightness == 0 ? matrixBrightness = defaultMatrixBrightness : true;
  lc.setIntensity(0, matrixBrightness); 
  lc.clearDisplay(0);
}
