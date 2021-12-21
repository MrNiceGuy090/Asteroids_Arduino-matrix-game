#pragma once

//Internal memory 
const int currentPlayerNameAddrs = 0;
const int levelAddrs = 10;
const int contrastAddrs = 12;
const int LCDBrightnessAddrs = 14;
const int matrixBrightnessAddrs = 16;
const int highscoresAddrs = 18;
const int playerSpeedAddrs = 54;
const int soundOnOffAddrs = 56;


//States and menus options
const char* states[13] = {"MainMenu", "Game", "Highscores", "SettingsMenu", "About", "EditName", "EditLevel", "EditPlayerSpeed","EditContrast", "EditLCDBrightness", "EditGameBrightness", "EditSound", "ResetHighscore"};
const int numberOfStates = 13;

const char* mainMenuStates[4] = {"Game", "Highscores", "SettingsMenu", "About"};
const int numberOfMainMenuStates = 4;

const char* mainMenuOptions[4] = {"Start game", "Highscore", "Settings", "About"}; 
const int numberOfMainMenuOptions = 4;

const char* settingsMenuOptions[9] = {"Name", "Level", "Player speed", "Contrast", "LCD brightness", "Game brightness", "Sound", "Reset highscore", "Back"}; 
const int numberOfSettingsMenuOptions = 9;

const int aboutInfoOptionsLength = 3;

//Game related
const int maxLevel = 15;
const int levelDuration = 20000;
const int highscoreListLength = 3;
const int scoreByteLength = 2;
const int maxLengthName = 10;

const int powerUpCheckInterval = 1000;
const int powerUpDisplayDuration = 5000;
const int powerUpBlinkingInterval = 150;
const int lifePowerUpIntroductionLevel = 5;
const int destructionPowerUpIntroductionLevel = 10;
const int powerUpRandomness = 10;

const int minPlayerSpeed = 100;
const int playerSpeedStep = 10;
const int playerSpeedMaxSteps = 10;
const int playeBlnkingInterval = 15;
const bool isPlayerBlinkingDefault = true;

const int defaultAsteroidDeploymentInterval = 600;
const int defaultAsteroidsSpeed = 1000;
const int defaultMaxAsteroids = 6;
const int asteroidSpeedIncreaseSlope = 40;
const int asteroidDeploymentIntervalIncreaseSlope = 30;

const int livesAtStartGame = 5;
const byte soundOnOffDefault = true;

const char* githubRepoLink = "https://github.com/MrNiceGuy090/Asteroids_Arduino-matrix-game";
const int githubRepoLinkLength = 61;
const int oneSecondInMillis = 1000;
const int githubRepoTextDisplayMinLength = 10;

//Joystick
const int minThreshold = 400;
const int maxThreshold = 600;
const int debounceInterval = 100;

//Hardware related
const int matrixSize = 8;
const int maxContrastAnalog = 255;
const int maxContrast = 100;
const int defaultContrast = 128;
const int maxLCDBrightnessAnalog = 255;
const int maxLCDBrightness = 100;
const int defaultLCDBrightness = 75;
const int defaultMatrixBrightness = 2;
const int maxMatrixBrightness = 15;
const int lcdLength = 16;

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
const int menuSliderBuzzerFreq = 2200;
const int menuSliderBuzzerDuration = 30;

//Symbols
const byte downArrow[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B11111,
  B01110,
  B00100,
};
const byte upArrow[8] = {
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,
};

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
