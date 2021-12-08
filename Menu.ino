#include "Menu.h"

Menu *Menu::instance = 0;

Menu::Menu(){
  settings = Settings::getInstance();
  selectingOption = 0;
  lcd.begin(16,2);
  lcd.print("Greetings!");
  textStartingPos = 0;
}
Menu* Menu::getInstance(){
  if(!Menu::instance){
    Menu::instance = new Menu;
  }
  return Menu::instance;
}

void Menu::show(){
  if(state == "MainMenu") displayMenuOptions();
  else if(state == "Highscores") displayHighscore();
  else if(state == "SettingsMenu") displaySettings();
  else if(state == "About") displayAboutInfo();
        
}

void Menu::displayMenuOptions(){
  lcd.clear();
  int optionsSize = 4;
  String options[optionsSize] = {"Start game", "Highscore", "Settings", "About"}; 
  displayMenuWindow(selectingOption, optionsSize, options);
}

void Menu::displaySettings(){
  lcd.clear();  
  int optionsSize = 6;
  String options[optionsSize] = {"Enter name", "Level", "Contrast", "LCD Brightness", "Game brightness", "Back"}; 
  displayMenuWindow(selectingOption, optionsSize, options);
}


void Menu::displayMenuWindow(int selectingOption, int optionsSize, String* options){
   if(selectingOption == optionsSize-1){
    lcd.print(options[optionsSize-2]);
    lcd.setCursor(0,1); 
    lcd.print(">" + options[optionsSize-1]);
  }else
  for(int option = 0; option < optionsSize - 1; option++ ){
    if(selectingOption == option){
      lcd.print(">" + options[option]);
      lcd.setCursor(0,1); 
      lcd.print(options[option+1]);
    }
  }
}

void Menu::increaseSelectingOption(){  
  int maxPos = 0;
  if(state == "MainMenu") maxPos = 3;
  else if(state == "SettingsMenu") maxPos = 5;
  else if(state == "Highscores") maxPos = 4;
  
  if(selectingOption == maxPos)
    selectingOption = 0;
  else {
    selectingOption++;
  }
}

void Menu::decreaseSelectingOption(){
  int maxPos = 0;
  if(state == "MainMenu") maxPos = 3;
  else if(state == "SettingsMenu") maxPos = 5;
  else if(state == "Highscores") maxPos = 4;
  
  if(selectingOption == 0)
    selectingOption = maxPos;
  else selectingOption--;
}

void Menu::displayHighscore(){
  lcd.clear();  
  if(selectingOption < 3){
    int highscoreAddrs = highscoresAddrs + selectingOption*(2+maxLengthName);
    int score = readFromEEPROM<int>(highscoreAddrs);
    String playerName = readPlayerNameFromEEPROM(highscoreAddrs+2);
    if(score > 0){
      lcd.print(playerName);
      lcd.setCursor(0,1);
      lcd.print(selectingOption+1);
      lcd.print(") ");
      lcd.print(score);
    }
    else{
      lcd.print(selectingOption+1);
      lcd.print(") ");
      lcd.print("NONE");
    }
  }
  else{
    lcd.print(">Back");
  }
}

void Menu::displayAboutInfo(){
  lcd.clear();
  for(int i = textStartingPos; i < textStartingPos + 15 && i < githubRepoLink.length(); i++){
    lcd.print(githubRepoLink[i]);
  }
  lcd.setCursor(0,1);
  lcd.print(">Back");
  delay(100);
}

void Menu::scrollTextRight(){  
  int maxPos = 0;
  if(state == "About") maxPos = githubRepoLink.length()-10;
   
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

String Menu::getState(){
 return state;
}

void Menu::setState(String value){
  state = value;
}

int Menu::getSelectingOption(){
   return selectingOption;
}

Settings* Menu::getSettings(){
  return settings;
}
