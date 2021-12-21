#include "LCDUtils.h"

void LCDSetup(){
  pinMode(brightnessPin, OUTPUT);
  pinMode(contrastPin, OUTPUT);
  lcd.begin(16,2);

  // set brightness from EEPROM
  int lcdBrightness = readFromEEPROM<int>(LCDBrightnessAddrs);
  lcdBrightness == 0 ? lcdBrightness = defaultLCDBrightness : true;
  analogWrite(brightnessPin, lcdBrightness);
  
  // set contrast from EEPROM
  int lcdContrast = readFromEEPROM<int>(contrastAddrs);
  lcdContrast == 0 ? lcdContrast = defaultContrast : true;
  analogWrite(contrastPin, lcdContrast);
  
  lcd.print("Welcome to");
  lcd.setCursor(0,1);  
  lcd.print("Asteroids");

  // create symbols
  lcd.createChar(0, downArrow);
  lcd.createChar(1, upArrow);
  
}
void clearLine(int row){
  lcd.setCursor(0,row);   
  lcd.print("               ");    
  lcd.setCursor(0,row);  
};

void setContrast(int percent){
  analogWrite(contrastPin, int(percent * maxContrastAnalog/100));
}

void setLCDBrightness(int percent){
  analogWrite(brightnessPin, int(percent * maxLCDBrightnessAnalog/100));
}
