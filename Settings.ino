#include "Settings.h"

Settings *Settings::instance = 0;

Settings::Settings(){
  name = "";
  value = 0;
  currentPosition = 0;
  toWriteCharacter = ' ';
  
  changingName = false;
  changingLevel = false;
  changingPlayerSpeed = false;
  changingContrast = false;
  changingLCDBrightness = false;
  changingMatrixBrightness = false;
  changingLCDBrightness = false;
  changingSound = false;
  resetingHighscore = false;
  
  js = Joystick::getInstance();
  sound = Sound::getInstance();
}

Settings* Settings::getInstance(){
  if(!Settings::instance){
    Settings::instance = new Settings;
  }
  return Settings::instance;
}

void Settings::changeName(){
  // init change name setting
  if(!changingName){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Name:");
    lcd.setCursor(0,1);    

    // initialize the character to be written with 'A'
    toWriteCharacter = 'A';
    name = readPlayerNameFromEEPROM(currentPlayerNameAddrs);

    // if there is no name in EEPROM, name will be 'A'
    if(name.length() == 0) name += toWriteCharacter;
    lcd.print(name);
    
    currentPosition = name.length()-1;
    lcd.setCursor(currentPosition, 1);
    // in case the name is in EEPROM, it is desired to highlight the last character as 
    // if would be written now
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
  // add letter
  if(js->turnedRightOnce()){
    if(currentPosition < maxLengthName - 1) {
      // save the last letter
      name[currentPosition] = toWriteCharacter;
      lcd.print(toWriteCharacter);
      
      // initialize the next later with 'A'
      currentPosition++;
      toWriteCharacter = 'A';
      name += toWriteCharacter;
    }
  }
  
  // erase the last letter
  if(js->turnedLeftOnce()){
    if(currentPosition > 0) {
      name.remove(currentPosition, 1);
      currentPosition--;
      toWriteCharacter = name[currentPosition];
    }
  }

  // blink selecting character
  if(millis() - blinkingTime > blinkingInterval){
    if(name[currentPosition] == ' ') name[currentPosition] = toWriteCharacter;
    else name[currentPosition] = ' ';
    lcd.print(name[currentPosition]);
    lcd.setCursor(currentPosition, 1);
    blinkingTime = millis();
  }

  // confirm name change 
  if(js->hasButtonBeenPressed()){
    name[currentPosition] = toWriteCharacter;
    // save to EEPROM
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

  //decrease level
  if(js->turnedDownOnce()){
    if(value > 1) value--;
    clearLine(1);  
    lcd.print(value);
  }  
  
  //increase level
  if(js->turnedUpOnce()){
    if(value < maxLevel) value++;
    clearLine(1);
    lcd.print(value); 
  }  

  // confirm level and save to EEPROM
  if(js->hasButtonBeenPressed()){
    changingLevel = false;
    saveToEEPROM<int>(levelAddrs, value);
  }
}

void Settings::changePlayerSpeed(){
  if(!changingPlayerSpeed){
    value = readFromEEPROM<int>(playerSpeedAddrs);
    if(value == 0) value = 1;
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Player speed:");
    lcd.setCursor(0,1);    
    lcd.print(value); 
    changingPlayerSpeed = true;
  }

  //decrease speed
  if(js->turnedDownOnce()){
    if(value > 1) value--;
    clearLine(1);  
    lcd.print(value);
  }  
  
  //increase speed
  if(js->turnedUpOnce()){
    if(value < playerSpeedMaxSteps) value++;
    clearLine(1);
    lcd.print(value); 
  }  

  // confirm player speed and save to EEPROM
  if(js->hasButtonBeenPressed()){
    changingPlayerSpeed = false;
    saveToEEPROM<int>(playerSpeedAddrs, value);
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

  // decrease contrast continuously
  if(js->isDown()){
    if(value > 1) value--;
    clearLine(1);  
    lcd.print(value);
    setContrast(value);
    delay(10);
  }  
  
  // increase contrast continuously
  if(js->isUp()){
    if(value < maxContrast) value++;
    clearLine(1);
    lcd.print(value); 
    setContrast(value);
    delay(10);
  }  

  // confirm contrast change and save to EEPROM
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
  
  // decrease LCD brightness continuously
  if(js->isDown()){
    if(value > 1) value--;
    clearLine(1);  
    lcd.print(value);
    setLCDBrightness(value);
    delay(10);
  }  
  
  // increase LCD brightness continuously
  if(js->isUp()){
    if(value < maxLCDBrightness) value++;
    clearLine(1);
    lcd.print(value); 
    setLCDBrightness(value);
    delay(10);
  }  

  
  // confirm LCD brightness change and save to EEPROM
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
    // light up the matrix
    for(int row = 0; row < matrixSize; row++){
      for(int col = 0; col < matrixSize; col++){
          lc.setLed(0, row, col, 1); 
        }
    }
    changingMatrixBrightness = true;
  }
  
  // decrease matrix brightness
  if(js->turnedDownOnce()){
    if(value > 1) value--;
    clearLine(1);  
    lcd.print(value);
    lc.setIntensity(0, value); 
    delay(10);
  }  
  
  // increase matrix brightness
  if(js->turnedUpOnce()){
    if(value < maxMatrixBrightness) value++;
    clearLine(1);
    lcd.print(value); 
    lc.setIntensity(0, value); 
    delay(10);
  }  

  
  // confirm matrix brightness change and save to EEPROM
  if(js->hasButtonBeenPressed()){
    changingMatrixBrightness = false;
    saveToEEPROM<int>(matrixBrightnessAddrs, value);
    // clear matrix
    for(int row = 0; row < matrixSize; row++){
      for(int col = 0; col < matrixSize; col++){
          lc.setLed(0, row, col, 0); 
        }
    }
  }
}

void Settings::changeSound(){
  Sound* sound = Sound::getInstance(); 
  if(!changingSound){
    value = sound->getSound();;
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sound:");
    lcd.setCursor(0,1);  
    if(value){
      lcd.print(">On       Off"); 
    }  
    else{
      lcd.print(" On      >Off"); 
    }
    changingSound = true;
  }

  if(js->turnedRightOnce()){
    clearLine(1);  
    sound->playSound(menuSliderBuzzerFreq,menuSliderBuzzerDuration);
    value = false;
    lcd.print(" On      >Off"); 
  }  
  
  if(js->turnedLeftOnce()){
    clearLine(1);  
    sound->playSound(menuSliderBuzzerFreq,menuSliderBuzzerDuration);
    value = true;
    lcd.print(">On       Off"); 
  }  
  
  if(js->hasButtonBeenPressed()){
    changingSound = false;
    sound->setSound(value);
  }
}

void Settings::resetHighscore(){
  if(!resetingHighscore){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Are you sure:");
    lcd.setCursor(0,1);  
    lcd.print(">Yes    Cancel"); 

    value = 1;
    resetingHighscore = true;
  }

  if(js->turnedRightOnce()){
    clearLine(1);  
    sound->playSound(menuSliderBuzzerFreq,menuSliderBuzzerDuration);
    value = 0;
    lcd.print(" Yes   >Cancel");
  }  
  
  if(js->turnedLeftOnce()){
    clearLine(1);  
    sound->playSound(menuSliderBuzzerFreq,menuSliderBuzzerDuration);
    value = 1;
    lcd.print(">Yes    Cancel"); 
  }  
  
  if(js->hasButtonBeenPressed()){
    resetingHighscore = false;
    if(value){
      for(int i = highscoresAddrs; i < highscoresAddrs + highscoreListLength*(scoreByteLength + maxLengthName); i++){
        saveToEEPROM<byte>(i, 0);
      }
    }
  }
}


bool Settings::isChangingName(){
  return changingName;
}
bool Settings::isChangingLevel(){
  return changingLevel;
}
bool Settings::isChangingPlayerSpeed(){
  return changingPlayerSpeed;
};
bool Settings::isChangingContrast(){
  return changingContrast;
}
bool Settings::isChangingLCDBrightness(){
  return changingLCDBrightness;
}
bool Settings::isChangingMatrixBrightness(){
  return changingMatrixBrightness;
}
bool Settings::isChangingSound(){
  return changingSound;
}
bool Settings::isResetingHighscore(){
  return resetingHighscore;
}
