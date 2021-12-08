#include "Settings.h"

Settings *Settings::instance = 0;

Settings::Settings(){
  name = "";
  value = 0;
  currentPosition = 0;
  toWriteCharacter = ' ';
  changingName = false;
  changingLevel = false;
  changingContrast = false;
  changingLCDBrightness = false;
  changingMatrixBrightness = false;
  changingLCDBrightness = false;
  js = Joystick::getInstance();
}

Settings* Settings::getInstance(){
  if(!Settings::instance){
    Settings::instance = new Settings;
  }
  return Settings::instance;
}

void Settings::changeName(){
  
  if(!changingName){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Name:");
    lcd.setCursor(0,1);    
    
    toWriteCharacter = 'A';
    name = readPlayerNameFromEEPROM(currentPlayerNameAddrs);
    
    if(name.length() == 0) name += toWriteCharacter;
    lcd.print(name);
    
    currentPosition = name.length()-1;
    lcd.setCursor(currentPosition, 1);
    toWriteCharacter = name[name.length()-1];
    changingName = true;
  }
  
  if(js->turnedDownOnce()){
    if(toWriteCharacter == 'A') toWriteCharacter = 'Z';
    else toWriteCharacter--;
  }  
  if(js->turnedUpOnce()){
    if(toWriteCharacter == 'Z') toWriteCharacter = 'A';
    else toWriteCharacter++;
  }
  if(js->turnedRightOnce()){
    if(currentPosition < maxLengthName) {
      name[currentPosition] = toWriteCharacter;
      lcd.print(toWriteCharacter);
      currentPosition++;
      if (currentPosition > name.length()-1) {
        toWriteCharacter = 'A';
        name += toWriteCharacter;
      }
      else toWriteCharacter = name[currentPosition];
    }
  }
  
  if(js->turnedLeftOnce()){
    if(currentPosition > 0) {
      name.remove(currentPosition, 1);
      currentPosition--;
      toWriteCharacter = name[currentPosition];
    }
  }
  
  if(millis() - blinkingTime > blinkingInterval){
    if(name[currentPosition] == ' ') name[currentPosition] = toWriteCharacter;
    else name[currentPosition] = ' ';
    lcd.print(name[currentPosition]);
    lcd.setCursor(currentPosition, 1);
    blinkingTime = millis();
  }
 
  if(js->hasButtonBeenPressed()){
    name[currentPosition] = toWriteCharacter;
    for(int i = currentPlayerNameAddrs; i < currentPlayerNameAddrs + maxLengthName; i++){
      if(name.length() > i){
        saveToEEPROM<char>(i, name[i-currentPlayerNameAddrs]);
      }
      else saveToEEPROM<char>(i, 0);
    }
    changingName = false;
    name = "";
  }
};

void Settings::changeLevel(){
  if(!changingLevel){
    value = readFromEEPROM<int>(levelAddrs);
    if(value == 0) value = 1;
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Level:");
    lcd.setCursor(0,1);    
    lcd.print(value); 
    changingLevel = true;
  }
  if(js->turnedDownOnce()){
    if(value > 1) value--;
    clearLine(1);  
    lcd.print(value);
  }  
  if(js->turnedUpOnce()){
    if(value < maxLevel) value++;
    clearLine(1);
    lcd.print(value); 
  }  
  
  if(js->hasButtonBeenPressed()){
    changingLevel = false;
    saveToEEPROM<int>(levelAddrs, value);
  }
}

void Settings::changeContrast(){
  if(!changingContrast){
    value = readFromEEPROM<int>(contrastAddrs);
    if(value == 0) value = defaultContrast;
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Contrast:");
    lcd.setCursor(0,1);    
    lcd.print(value); 
    changingContrast = true;
  }
  if(js->isDown()){
    if(value > 1) value--;
    clearLine(1);  
    lcd.print(value);
    setContrast(value);
    delay(10);
  }  
  if(js->isUp()){
    if(value < maxContrast) value++;
    clearLine(1);
    lcd.print(value); 
    setContrast(value);
    delay(10);
  }  
  
  if(js->hasButtonBeenPressed()){
    changingContrast = false;
    saveToEEPROM<int>(contrastAddrs, value);
  }
}

void Settings::changeLCDBrightness(){
  if(!changingLCDBrightness){
    value = readFromEEPROM<int>(LCDBrightnessAddrs);
    if(value == 0) value = defaultLCDBrightness;
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("LCD Brightness:");
    lcd.setCursor(0,1);    
    lcd.print(value); 
    changingLCDBrightness = true;
  }
  if(js->isDown()){
    if(value > 1) value--;
    clearLine(1);  
    lcd.print(value);
    setLCDBrightness(value);
    delay(10);
  }  
  if(js->isUp()){
    if(value < maxLCDBrightness) value++;
    clearLine(1);
    lcd.print(value); 
    setLCDBrightness(value);
    delay(10);
  }  
  
  if(js->hasButtonBeenPressed()){
    changingLCDBrightness = false;
    saveToEEPROM<int>(LCDBrightnessAddrs, value);
  }
}

void Settings::changeMatrixBrightness(){
  if(!changingMatrixBrightness){
    value = readFromEEPROM<int>(matrixBrightnessAddrs);
    if(value == 0) value = defaultMatrixBrightness;
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Matrix Brightness:");
    lcd.setCursor(0,1);    
    lcd.print(value); 
    for(int i=0; i<8; i++)
      lc.setLed(0,0,i,1);
    changingMatrixBrightness = true;
  }
  if(js->turnedDownOnce()){
    if(value > 1) value--;
    clearLine(1);  
    lcd.print(value);
    lc.setIntensity(0, value); 
    delay(10);
  }  
  if(js->turnedUpOnce()){
    if(value < maxMatrixBrightness) value++;
    clearLine(1);
    lcd.print(value); 
    lc.setIntensity(0, value); 
    delay(10);
  }  
  
  if(js->hasButtonBeenPressed()){
    changingMatrixBrightness = false;
    saveToEEPROM<int>(matrixBrightnessAddrs, value);
    for(int i=0; i<8; i++)
      lc.setLed(0,0,i,0);
  }
}

bool Settings::isChangingName(){
  return changingName;
}
bool Settings::isChangingLevel(){
  return changingLevel;
}
bool Settings::isChangingContrast(){
  return changingContrast;
}
bool Settings::isChangingLCDBrightness(){
  return changingLCDBrightness;
}
bool Settings::isChangingMatrixBrightness(){
  return changingMatrixBrightness;
}

int Settings::getMaxLevel(){
  return maxLevel;
}
