#include "LCDUtils.h"

void LCDSetup(){
  pinMode(brightnessPin, OUTPUT);
  pinMode(contrastPin, OUTPUT);
  lcd.begin(16,2);
  
  int lcdBrightness = readFromEEPROM<int>(LCDBrightnessAddrs);
  lcdBrightness == 0 ? lcdBrightness = defaultLCDBrightness : true;
  analogWrite(brightnessPin, lcdBrightness);
  
  int lcdContrast = readFromEEPROM<int>(contrastAddrs);
  lcdContrast == 0 ? lcdContrast = defaultContrast : true;
  analogWrite(contrastPin, lcdContrast);
  lcd.print("Welcome to");
  lcd.setCursor(0,1);  
  lcd.print("Asteroids");
}
void clearLine(int row){
  lcd.setCursor(0,row);   
  lcd.print("               ");    
  lcd.setCursor(0,row);  
};

void setContrast(int value){
  analogWrite(contrastPin, value);
}

void setLCDBrightness(int value){
  analogWrite(brightnessPin, value);
}
