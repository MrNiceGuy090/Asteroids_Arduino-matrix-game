#include "LedControl.h"
#include "Menu.h"
#include "Joystick.h"
#include "LCDUtils.h"
#include "Game.h"
#include "Consts.h"
#include<LiquidCrystal.h>

LiquidCrystal lcd(RS,enable,d4,d5,d6,d7);
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); 

char* state;
Menu *menu= Menu::getInstance();
Joystick *js= Joystick::getInstance();
Game *game;

void setup()
{
  Serial.begin(9600);
  pinMode(pinSW,INPUT_PULLUP);
  
  state = states[0];
  LCDSetup();
  matrixSetup();
  
  delay(2000);
  
  menu->show();
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
    if(game == NULL){
      game = new Game();
    }
    game->playGame();

    // if state changed in the playGame() method of game object
    if(state == "MainMenu"){
      delete game;
      game = NULL;
      menu->resetMenu();
      menu->show();
    }
  }
  else if(state == "Highscores"){   
    if(js->hasButtonBeenPressed() && menu->getSelectingOption() == highscoreListLength){
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
      if(settingsMenuOptions[menu->getSelectingOption()] == "Name"){
        state = "EditName";
      }
      else if(settingsMenuOptions[menu->getSelectingOption()] == "Level"){
        state = "EditLevel";
      }
      else if(settingsMenuOptions[menu->getSelectingOption()] == "Player speed"){
        state = "EditPlayerSpeed";
      }
      else if(settingsMenuOptions[menu->getSelectingOption()] == "Contrast"){
        state = "EditContrast";
      }
      else if(settingsMenuOptions[menu->getSelectingOption()] == "LCD brightness"){
        state = "EditLCDBrightness";
      }      
      else if(settingsMenuOptions[menu->getSelectingOption()] == "Game brightness"){
        state = "EditMatrixBrightness";
      }   
      else if(settingsMenuOptions[menu->getSelectingOption()] == "Sound"){
        state = "EditSound";
      }
      else if(settingsMenuOptions[menu->getSelectingOption()] == "Reset highscore"){
        state = "ResetHighscore";
      }
      else if(settingsMenuOptions[menu->getSelectingOption()] == "Back"){
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
    if(js->hasButtonBeenPressed() && menu->getSelectingOption() == aboutInfoOptionsLength){
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
    
    // if the githubRepo is on screen
    if(menu->getSelectingOption() == aboutInfoOptionsLength-1){
      if(js->isLeft()){
        menu->scrollTextLeft();
        menu->show();
      }
      else if(js->isRight()){
        menu->scrollTextRight();
        menu->show();
      }
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
  else if(state == "EditPlayerSpeed"){
      menu->getSettings()->changePlayerSpeed();      
      if(!menu->getSettings()->isChangingPlayerSpeed()){
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
  else if(state == "EditSound"){
      menu->getSettings()->changeSound();      
      if(!menu->getSettings()->isChangingSound()){
        state = "SettingsMenu";
        menu->resetMenu();
        menu->show();
      }    
  }  
  else if(state == "ResetHighscore"){
      menu->getSettings()->resetHighscore();      
      if(!menu->getSettings()->isResetingHighscore()){
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
