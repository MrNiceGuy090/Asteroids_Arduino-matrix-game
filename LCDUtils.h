#pragma once
#include<LiquidCrystal.h>
#include "EEPROMUtils.h"
#include "Consts.h"

extern LiquidCrystal lcd;

void LCDSetup();
void clearLine(int row);
void setContrast(int value);
void setLCDBrightness(int value);
