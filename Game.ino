#include "Game.h"
#include "Joystick.h"

Game::Game(){
  lives = 5;
  time = 0;
  int memLevel = readFromEEPROM<int>(levelAddrs);
  currentLevel = memLevel == 0? 1 : memLevel;
  isPlayingLevel = false;
  levelStartingTime = 0;
  lastTimerChangeTime = 0;
  playeBlnkingTime = 0;
  score = 0;
  js = Joystick::getInstance();
  gameEnded = false;
  isPlayerBlinking = false;
  asteroidsList = LinkedList<Asteroid*>();
  
  playerPos[0] = 4;
  playerPos[1] = 4; 
  
  powerUp[0] = -1;
  powerUp[1] = -1;
  lastPowerUpCheckTime = 0;
  isPowerUpDisplayed = false;
  powerUpDisplayTime = 0;
  powerUpInitTime = 0;
  
  displayMatrix[playerPos[0]][playerPos[1]] = 1;  
}

void Game::playGame(){
  if(gameEnded){
    if(js->hasButtonBeenPressed()){
      for(int row = 0; row < matrixSize; row++){
        for(int col = 0; col < matrixSize; col++){
            lc.setLed(0, row, col, 0); 
          }
      }
      state = "MainMenu";
    }
  }
  else{
    if(!isPlayingLevel) 
      startLevel(currentLevel);
    else if(levelDuration < (millis() - levelStartingTime)/1000){
      endLevel(currentLevel);
      if(currentLevel == maxLevel) 
        endGame();
      else
        startLevel(++currentLevel);
    }
    
    updateScoreAndTimer();
    playLevel();
  }
}

void Game::startLevel(int level){
  updateDisplay();
  //TO DO: move playerName init to start of game, not level
  playerName = readPlayerNameFromEEPROM(currentPlayerNameAddrs);
  levelStartingTime = millis();
  lastTimerChangeTime = millis();
  isPlayingLevel = true;  
  asteroidDeploymentInterval = defaultAsteroidDeploymentInterval - level*asteroidDeploymentIntervalIncreaseSlope;
  asteroidsSpeed = defaultAsteroidsSpeed - level*asteroidSpeedIncreaseSlope;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Lvl:");
  lcd.print(level);
  lcd.setCursor(0,1);
  lcd.print("Lives:");
  updateLives();
  updateScoreAndTimer();
  tone(buzzerPin, startLevelBuzzerFreq,startLevelBuzzerDuration);
}

void Game::playLevel(){
  if(js->hasButtonBeenPressed()){
    isPlayerBlinking = !isPlayerBlinking;
  }
  // blink player
  if(isPlayerBlinking && millis() - playeBlnkingTime > playeBlnkingInterval){
    displayMatrix[playerPos[0]][playerPos[1]] = !displayMatrix[playerPos[0]][playerPos[1]];
    playeBlnkingTime = millis();
    lc.setLed(0, playerPos[0], playerPos[1], displayMatrix[playerPos[0]][playerPos[1]]); 
  }
  else if(!isPlayerBlinking){
    displayMatrix[playerPos[0]][playerPos[1]] = 1;
    lc.setLed(0, playerPos[0], playerPos[1], displayMatrix[playerPos[0]][playerPos[1]]);
  }
  //place asteroids
  //check for time to deplay and asteroids max count
  if(millis() - lastDeployedAsteroidTime > asteroidDeploymentInterval && maxAsteroids() > asteroidsList.size() ){
    placeNewAsteroid();
    lastDeployedAsteroidTime = millis();
    updateDisplay();
  }
  // move asteroids
  if(millis() - lastAsteroidMovedTime > asteroidsSpeed){
    updateAsteroidsPosition();
    lastAsteroidMovedTime = millis();
    updateDisplay();
  }
  // place powerUps
  if(isPowerUpDisplayed == false && millis() - lastPowerUpCheckTime > powerUpCheckInterval && currentLevel >= lifePowerUpIntroductionLevel){
    lastPowerUpCheckTime = millis();
    providePowerUp();
  }
  // blink powerUps
  if(isPowerUpDisplayed == true && millis() - powerUpDisplayTime > powerUpBlinkingInterval){
    displayMatrix[powerUp[0]][powerUp[1]] = !displayMatrix[powerUp[0]][powerUp[1]]; 
    tone(buzzerPin, powerUpBlinkingBuzzerFreq,powerUpBlinkingBuzzerDuration);
    powerUpDisplayTime = millis();
  }
  // delete powerUps
  if(millis() -  powerUpInitTime > powerUpDisplayDuration*1000){
    isPowerUpDisplayed = false;
    displayMatrix[powerUp[0]][powerUp[1]] = 0;
  }
  
  if(millis() - lastMoved > moveInterval){
    updatePlayerPosition();
    lastMoved = millis(); 
    updateDisplay();
  }
  
  if(lives == 0){
    tone(buzzerPin, endGameBuzzerFreq,endGameBuzzerDuration);
    delay(1000);
    endGame();
  }
  
}

void Game::updateLives(){
  lcd.setCursor(6, 1);
  lcd.print(" ");
  lcd.print(lives);
  if(lives < 10){
    lcd.print(" ");
  }  
}

void Game::updateScoreAndTimer(){  
  long time = levelDuration - (millis() - levelStartingTime)/1000;
  if(time < 0){
    lcd.setCursor(11,1);
    lcd.print("FIN "); 
  }
  else if(millis() - lastTimerChangeTime > 1000){
    //timer
    int minutes = time / 60;
    int seconds = time % 60;
    lcd.setCursor(11,1);
    lcd.print(minutes);
    lcd.print(":");
    seconds < 10 ? lcd.print("0") : true;
    lcd.print(seconds);
    //score
    score+=currentLevel;
    lcd.setCursor(11,0);
    lcd.print(score);
    
    lastTimerChangeTime = millis();
  }
}

void Game::endLevel(int level){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Lvl ");
  lcd.print(currentLevel);
  lcd.print(" finished!");
  lcd.setCursor(0,1);
  lcd.print("Score: ");
  lcd.print(score);

  while(asteroidsList.size() != 0){    
    int x = asteroidsList.get(0)->getX();
    int y = asteroidsList.get(0)->getY();
    displayMatrix[x][y] = 0;
    delete(asteroidsList.get(0));
    asteroidsList.remove(0);
  }
  displayMatrix[powerUp[0]][powerUp[1]] = 0;
  displayMatrix[playerPos[0]][playerPos[1]] = 1;
  updateDisplay();
  resetPowerUp();
  tone(buzzerPin, endLevelBuzzerFreq,endLevelBuzzerDuration);
  delay(5000);
}
void Game::endGame(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Game end!");
  delay(2000);
  int highscorePos = checkAndSaveHighscore();
  if(highscorePos > 0){
    tone(buzzerPin, endGameCongratsBuzzerFreq,endGameCongratsBuzzerDuration);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Congrats!");
    lcd.setCursor(0,1);
    lcd.print("New highscore!");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("You are now in");
    lcd.setCursor(0,1);
    lcd.print("place ");
    lcd.print(highscorePos);
    delay(2000);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(playerName);
  lcd.setCursor(0,1);
  lcd.print("Score: ");
  lcd.print(score);
  lcd.print(">Menu");
  gameEnded = true;
}

void Game::updatePlayerPosition(){
  js->read();
  int xValue = js->getX();
  int yValue = js->getY();  

  lastPlayerPos[0] = playerPos[0];
  lastPlayerPos[1] = playerPos[1];

  if(js->isDown()){
    if(playerPos[0] < matrixSize - 1){
      playerPos[0]++;
    }
  }
  if(js->isUp()){
    if(playerPos[0] > 0){
      playerPos[0]--;
    }
  }

  if(js->isRight()){
    if(playerPos[1] < matrixSize - 1){
      playerPos[1]++;
    }
  }
  if(js->isLeft()){
    if(playerPos[1] > 0){
      playerPos[1]--;
    }
  }

  if(lastPlayerPos[0] != playerPos[0] || lastPlayerPos[1] != playerPos[1]){

    if(playerPos[0] == powerUp[0] && playerPos[1] == powerUp[1]){
      activatePowerUp();
    }
    else if( displayMatrix[playerPos[0]][playerPos[1]] == 1 ){
      lives--;
      tone(buzzerPin, lostLifeBuzzerFreq,lostLifeBuzzerDuration);
      updateLives();
      if(lives == 0) return;
      for(int i = 0; i < asteroidsList.size()-1; i++ ){
        int x = asteroidsList.get(i)->getX();
        int y = asteroidsList.get(i)->getY();
        if(x == playerPos[0] && y == playerPos[1]){
          delete(asteroidsList.get(i));
          asteroidsList.remove(i);
          break;
        }
      }
    }  
    displayMatrix[lastPlayerPos[0]][lastPlayerPos[1]] = 0;    
   } 
}

void Game::activatePowerUp(){
  int option;
  if(currentLevel >= destructionPowerUpIntroductionLevel){
    option = random(0,2);
  }
  else if( currentLevel >= lifePowerUpIntroductionLevel){
    option = 0;
  }
  else {
    resetPowerUp();
    return;
  }
  if(option == 0){
    lives++;
    updateLives();
  }
  else if(option == 1){
    for(int i = 0; i < asteroidsList.size()-1; i++ ){
      int x = asteroidsList.get(i)->getX();
      int y = asteroidsList.get(i)->getY();      
   
      delete(asteroidsList.get(i));
      asteroidsList.remove(i);
      displayMatrix[x][y] = 0;
      i--;
    }
  }
  resetPowerUp();
  tone(buzzerPin, powerUpActivationBuzzerFreq,powerUpActivationBuzzerDuration);
}

void Game::placeNewAsteroid(){
  Asteroid* ob = new Asteroid();
  while(displayMatrix[ob->getX()][ob->getY()] == 1){
    delete ob;
    ob = new Asteroid();
  }
  asteroidsList.add(ob);  
  displayMatrix[ob->getX()][ob->getY()] = 1;
}

void Game::updateAsteroidsPosition(){
  for(int i = 0; i < asteroidsList.size()-1; i++ ){
    int x = asteroidsList.get(i)->getX();
    int y = asteroidsList.get(i)->getY();
    
    if(displayMatrix[x][y] == 0){
      delete(asteroidsList.get(i));
      asteroidsList.remove(i);
      continue;
    }
    else 
      displayMatrix[x][y] = 0;
      
    asteroidsList.get(i)->advance();
    
    x = asteroidsList.get(i)->getX();
    y = asteroidsList.get(i)->getY();    
    if(x > 7 || x < 0 || y < 0 || y > 7){
      delete(asteroidsList.get(i));
      asteroidsList.remove(i);
      i--;
      continue;
    }
    
    if(displayMatrix[x][y] == 1){
      if( isPowerUpDisplayed && x == powerUp[0] && y == powerUp[1] ){
        continue;
      }
      else {
        if(x == playerPos[0] && y == playerPos[1]){
          lives--; 
          updateLives();
          tone(buzzerPin, lostLifeBuzzerFreq,lostLifeBuzzerDuration);
          if(lives == 0) return;
        }
        delete(asteroidsList.get(i));
        asteroidsList.remove(i);
      }
    }
    else{
      displayMatrix[x][y] = 1;
    }
  }
}

void Game::resetPowerUp(){
  isPowerUpDisplayed = false;
  powerUpInitTime = 0;
  if(powerUp[0] != playerPos[0] || powerUp[1] != playerPos[1])
    displayMatrix[powerUp[0]][powerUp[1]] = 0;
  powerUp[0] = -1;
  powerUp[1] = -1;
}

void Game::providePowerUp(){
 int chance = random(0,powerUpRandomness);
  if( chance != 0 ) return;
  do{
    powerUp[0] = random(0,8);
    powerUp[1] = random(0,8);
  }while(displayMatrix[powerUp[0]][powerUp[1]] != 0);
  
  displayMatrix[powerUp[0]][powerUp[1]] = 1;
    
  powerUpInitTime = millis();
  powerUpDisplayTime = millis();
  isPowerUpDisplayed = true;
  
}

int Game::checkAndSaveHighscore(){
  for(int i=0; i<3;i++){
    int currentAddrs = highscoresAddrs + i*(2+maxLengthName);
    int value = readFromEEPROM<int>(currentAddrs);
    if(score > value){
      // move places down
      if( i < 2 ){
        int nextPlaceAddrs = currentAddrs + 2+maxLengthName;
        for(int addrs = nextPlaceAddrs + (2-i)*(2 + maxLengthName) -1; addrs >= nextPlaceAddrs ; addrs-- ){
          int bit = EEPROM.read(addrs - 2 - maxLengthName);
          EEPROM.write(addrs, bit);
        }
      }
      //save to EEPROM
      saveToEEPROM<int>(currentAddrs, score);
      for(int pos = currentAddrs + 2; pos < currentAddrs + 2 + maxLengthName; pos++){
        if(playerName.length() > pos-currentAddrs-2){
          saveToEEPROM<char>(pos, playerName[pos-currentAddrs-2]);
        }
        else saveToEEPROM<char>(pos, 0);
      }
      return i+1; 
    }
  }
  return 0;  
}

int Game::maxAsteroids(){
  return defaultMaxAsteroids + (currentLevel/3) - (currentLevel/14);
}

void Game::updateDisplay(){
  for(int row = 0; row < matrixSize; row++){
    for(int col = 0; col < matrixSize; col++){
        lc.setLed(0, row, col, displayMatrix[row][col]); 
      }
  }
}
