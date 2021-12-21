#include "Menu.h"

Menu *Menu::instance = 0;

Menu::Menu(){
  settings = Settings::getInstance();
  sound = Sound::getInstance();
  
  selectingOption = 0;
  textStartingPos = 0;
}
Menu* Menu::getInstance(){
  if(!Menu::instance){
    Menu::instance = new Menu;
  }
  return Menu::instance;
}

void Menu::show(){
  lcd.clear();  
  if(state == "MainMenu") displayMenuOptions();
  else if(state == "Highscores") displayHighscore();
  else if(state == "SettingsMenu") displaySettings();
  else if(state == "About") displayAboutInfo();
        
}

void Menu::displayMenuOptions(){
  displayMenuWindow(selectingOption, numberOfMainMenuOptions, mainMenuOptions);
}

void Menu::displaySettings(){
  displayMenuWindow(selectingOption, numberOfSettingsMenuOptions, settingsMenuOptions);
}

void Menu::displayHighscore(){
  if(selectingOption < highscoreListLength){
    int highscoreAddrs = highscoresAddrs + selectingOption*(scoreByteLength+maxLengthName);
    int score = readFromEEPROM<int>(highscoreAddrs);
    String playerName = readPlayerNameFromEEPROM(highscoreAddrs+scoreByteLength);
    
    lcd.print(selectingOption+1);
    lcd.print(") ");
    if(score > 0){
      lcd.print(playerName);
      lcd.setCursor(0,1);
      lcd.print(score);
    }
    else{
      lcd.print("NONE");
    }
    lcd.setCursor(lcdLength-1,1);
    lcd.write(byte(0));
  }
  else{
    lcd.print(">Back");
    lcd.setCursor(lcdLength-1,1);
    lcd.write(byte(1));
  }
}

void Menu::displayAboutInfo(){
  if(selectingOption == 0){
    lcd.clear();
    lcd.print("Game name:");
    lcd.setCursor(0,1);
    lcd.print("Asteroids");
    lcd.setCursor(lcdLength-1,1);
    lcd.write(byte(0));
  }
  else if(selectingOption == 1){
    lcd.clear();
    lcd.print("Developer:");
    lcd.setCursor(0,1);
    lcd.print("Smeu Stefan");
    lcd.setCursor(lcdLength-1,1);
    lcd.write(byte(0));
  } 
  else if(selectingOption == 2){
    lcd.clear();
    lcd.print("Project Repo:");
    lcd.setCursor(0,1);
    for(int i = textStartingPos; i < textStartingPos + lcdLength-1 && i < githubRepoLinkLength; i++){
      lcd.print(githubRepoLink[i]);
    }
    lcd.setCursor(lcdLength-1,1);
    lcd.write(byte(0));
    delay(100);
  }
  else {
    lcd.clear();
    lcd.print(">Back");
    lcd.setCursor(lcdLength-1,1);
    lcd.write(byte(1));
  }
}

// function to help display 2 options from a list on display 
void Menu::displayMenuWindow(int selectingOption, int optionsSize, const char** options){
   // if the cursor is on last option, display the last 2
   if(selectingOption == optionsSize-1){
    lcd.print(options[optionsSize-2]);
    lcd.setCursor(0,1); 
    lcd.print(">");
    lcd.print(options[optionsSize-1]);
    lcd.setCursor(lcdLength-1,1);
    lcd.write(byte(1));
  }
  // if the cursor is not on the last one, display the cursor on first row and next option on the other line
  else
  for(int option = 0; option < optionsSize - 1; option++ ){
    if(selectingOption == option){
      lcd.print(">");
      lcd.print(options[option]);
      lcd.setCursor(0,1); 
      lcd.print(options[option+1]);
      lcd.setCursor(lcdLength-1,1);
      lcd.write(byte(0));
    }
  }
}

void Menu::increaseSelectingOption(){  
  int maxPos = getMaxSelectingOption();
  sound->playSound(menuSliderBuzzerFreq,menuSliderBuzzerDuration);
  if(selectingOption == maxPos)
    selectingOption = 0;
  else {
    selectingOption++;
  }
}

void Menu::decreaseSelectingOption(){
  int maxPos = getMaxSelectingOption();
  sound->playSound(menuSliderBuzzerFreq,menuSliderBuzzerDuration);
  if(selectingOption == 0)
    selectingOption = maxPos;
  else selectingOption--;
}

void Menu::scrollTextRight(){  
  int maxPos = 0;
  // githubRepoTextDisplayMinLength should be lower than githubRepoLink.length()
  // so that there are some blank spaces at the end of the link
  // so that textStartingPos does not go to 0 right after the last letter of the link is shown
  if(state == "About") maxPos = githubRepoLinkLength - githubRepoTextDisplayMinLength;
   
  if(textStartingPos == maxPos)
    textStartingPos = 0;
  else {
    textStartingPos++;
  }
}

void Menu::scrollTextLeft(){  
  if(textStartingPos > 0)
    textStartingPos--;
}

void Menu::resetMenu(){
  selectingOption = 0;
}

char* Menu::getState(){
 return state;
}

void Menu::setState(char* value){
  state = value;
}

int Menu::getSelectingOption(){
   return selectingOption;
}

int Menu::getMaxSelectingOption(){
  if(state == "MainMenu") return numberOfMainMenuStates-1;
  else if(state == "SettingsMenu") return numberOfSettingsMenuOptions-1;
  else if(state == "Highscores") return highscoreListLength;
  else if(state == "About") return aboutInfoOptionsLength;
}

Settings* Menu::getSettings(){
  return settings;
}
