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
const int maxLevel = 15;
const int levelDuration = 30;
const int powerUpDuration = 10;
const int powerUpCheckInterval = 1000;
const int powerUpDisplayDuration = 5;
const int moveInterval = 130;
const int defaultAsteroidDeploymentInterval = 600;
const int defaultAsteroidsSpeed = 1000;
const int defaultMaxAsteroids = 6;
const int powerUpBlinkingInterval = 150;
const int lifePowerUpIntroductionLevel = 5;
const int destructionPowerUpIntroductionLevel = 10;
const int asteroidSpeedIncreaseSlope = 40;
const int asteroidDeploymentIntervalIncreaseSlope = 30;
const int powerUpRandomness = 10;
const int playeBlnkingInterval = 15;
const String githubRepoLink = "https://github.com/MrNiceGuy090/Asteroids_Arduino-matrix-game";

//Hardware related
const int matrixSize = 8;
const int maxContrast = 255;
const int defaultContrast = 128;
const int maxLCDBrightness = 150;
const int defaultLCDBrightness = 75;
const int defaultMatrixBrightness = 2;
const int maxMatrixBrightness = 5;

const int lostLifeBuzzerFreq = 400;
const int lostLifeBuzzerDuration = 50;
const int powerUpActivationBuzzerFreq = 2000;
const int powerUpActivationBuzzerDuration = 100;
const int startLevelBuzzerFreq = 2000;
const int startLevelBuzzerDuration = 200;
const int endLevelBuzzerFreq = 2600;
const int endLevelBuzzerDuration = 200;
const int endGameBuzzerFreq = 1000;
const int endGameBuzzerDuration = 200;
const int endGameCongratsBuzzerFreq = 3000;
const int endGameCongratsBuzzerDuration = 200;
const int powerUpBlinkingBuzzerFreq = 2200;
const int powerUpBlinkingBuzzerDuration = 20;
const int joystickPressBuzzerFreq = 1200;
const int joystickPressBuzzerDuration = 30;

//Pins
const int RS = 13;
const int enable = 6;
const int d4 =7;
const int d5 =4;
const int d6 =9;
const int d7 =2;

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
const int contrastPin = 9;
const int brightnessPin = 5;

const byte pinSW = 8;
const byte pinX = A0;
const byte pinY = A1;

const int buzzerPin = 3;
