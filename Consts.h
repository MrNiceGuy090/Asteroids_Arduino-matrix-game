#pragma once

//Internal memory 
const int currentPlayerNameAddrs = 0;
const int levelAddrs = 10;
const int contrastAddrs = 12;
const int LCDBrightnessAddrs = 14;
const int matrixBrightnessAddrs = 16;
const int maxLengthName = 10;
const int highscoresAddrs = 18;

//States
const String mainMenuStates[4] = {"Game", "Highscores", "SettingsMenu", "About"};
// for states count tracking

//Game related
const int maxLevel = 20;
const int levelDuration = 30;
const int moveInterval = 100;
const int gameObjectDeploymentInterval = 400;

//Hardware related
const int matrixSize = 8;
const int maxContrast = 255;
const int defaultContrast = 128;
const int maxLCDBrightness = 150;
const int defaultLCDBrightness = 75;
const int defaultMatrixBrightness = 2;
const int maxMatrixBrightness = 5;

//Pins
const int RS = 13;
const int enable = 6;
const int d4 =7;
const int d5 =4;
const int d6 =3;
const int d7 =2;

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
const int contrastPin = 9;
const int brightnessPin = 5;

const byte pinSW = 8;
const byte pinX = A0;
const byte pinY = A1;
