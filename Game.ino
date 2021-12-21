#include "Game.h"

Game::Game(){
  lives = livesAtStartGame;
  
  playerName = readPlayerNameFromEEPROM(currentPlayerNameAddrs);
  playerSpeed = calculatePlayerSpeed();
  
  int memLevel = readFromEEPROM<int>(levelAddrs);
  currentLevel = memLevel == 0? 1 : memLevel;
  
  isPlayingLevel = false;
  gameEnded = false;
  
  isPlayerBlinking = isPlayerBlinkingDefault;
  isPowerUpDisplayed = false;
  
  levelStartingTime = 0;
  lastTimerChangeTime = 0;
  playeBlnkingTime = 0;
  lastPowerUpCheckTime = 0;  
  powerUpDisplayTime = 0;
  powerUpInitTime = 0;
  score = 0;
  
  js = Joystick::getInstance();
  sound = Sound::getInstance();
  asteroidsList = LinkedList<Asteroid*>();
  
  playerPos[0] = 4;
  playerPos[1] = 4; 
  
  powerUp[0] = -1;
  powerUp[1] = -1;
  
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
    else if(levelDuration/oneSecondInMillis < (millis() - levelStartingTime)/oneSecondInMillis){
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
  levelStartingTime = millis();
  isPlayingLevel = true;  
  
  asteroidDeploymentInterval = defaultAsteroidDeploymentInterval - level*asteroidDeploymentIntervalIncreaseSlope;
  asteroidsSpeed = defaultAsteroidsSpeed - level*asteroidSpeedIncreaseSlope;
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Level:");
  lcd.print(level);
  lcd.setCursor(0,1);
  lcd.print("Lives:");
  
  updateLivesDisplay();
  initScoreAndTimer();
  
  sound->playSound(startLevelBuzzerFreq,startLevelBuzzerDuration);
}

void Game::playLevel(){
  bool changeHasBeenMade = false;
  
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
  
  // move asteroids
  if(millis() - lastAsteroidMovedTime > asteroidsSpeed){
    updateAsteroidsPosition();
    lastAsteroidMovedTime = millis();
    changeHasBeenMade = true;
  }
  
  //place asteroids
  //check for time to deploy and asteroids max count
  if(millis() - lastDeployedAsteroidTime > asteroidDeploymentInterval && maxAsteroids() > asteroidsList.size() ){
    placeNewAsteroid();
    lastDeployedAsteroidTime = millis();
    changeHasBeenMade = true;
  }
  
  // place powerUps
  if(isPowerUpDisplayed == false && millis() - lastPowerUpCheckTime > powerUpCheckInterval && currentLevel >= lifePowerUpIntroductionLevel){
    lastPowerUpCheckTime = millis();
    providePowerUp();
    changeHasBeenMade = true;
  }
  
  // blink powerUps
  if(isPowerUpDisplayed == true && millis() - powerUpDisplayTime > powerUpBlinkingInterval){
    displayMatrix[powerUp[0]][powerUp[1]] = !displayMatrix[powerUp[0]][powerUp[1]]; 
    sound->playSound(powerUpBlinkingBuzzerFreq,powerUpBlinkingBuzzerDuration);
    powerUpDisplayTime = millis();
    changeHasBeenMade = true;
  }
  
  // delete powerUps
  if(millis() -  powerUpInitTime > powerUpDisplayDuration){
    isPowerUpDisplayed = false;
    displayMatrix[powerUp[0]][powerUp[1]] = 0;
    changeHasBeenMade = true;
  }

  // move player
  if(millis() - lastMoved > playerSpeed){
    updatePlayerPosition();
    lastMoved = millis(); 
    changeHasBeenMade = true;
  }

  if(changeHasBeenMade == true){
    updateDisplay();
  }

  // end game condition
  if(lives <= 0){
    sound->playSound(endGameBuzzerFreq,endGameBuzzerDuration);
    delay(oneSecondInMillis);
    endGame();
  }
}

void Game::updateLivesDisplay(){
  lcd.setCursor(6, 1);
  lcd.print(lives);
  if(lives < 10){
    lcd.print(" ");
  }  
}

void Game::initScoreAndTimer(){
  unsigned long long time = levelDuration/oneSecondInMillis - (millis() - levelStartingTime)/oneSecondInMillis;
     
  //timer
  int minutes = time / 60;
  int seconds = time % 60;
  lcd.setCursor(9,1);
  lcd.print("T: ");
  lcd.print(minutes);
  lcd.print(":");
  seconds < 10 ? lcd.print("0") : true;
  lcd.print(seconds);
  
  //score
  lcd.setCursor(9,0);
  lcd.print("S: ");
  lcd.print(score);
    
  lastTimerChangeTime = millis();
}

void Game::updateScoreAndTimer(){  
  unsigned long long time = levelDuration/oneSecondInMillis - (millis() - levelStartingTime)/oneSecondInMillis;
  if(time >= 0 && millis() - lastTimerChangeTime > oneSecondInMillis){
    //timer
    int minutes = time / 60;
    int seconds = time % 60;
    lcd.setCursor(12,1);
    lcd.print(minutes);
    lcd.setCursor(14,1);
    seconds < 10 ? lcd.print("0") : true;
    lcd.print(seconds);
    //score
    score+=currentLevel;
    lcd.setCursor(12,0);
    lcd.print(score);
    
    lastTimerChangeTime = millis();
  }
}

void Game::endLevel(int level){
  // end level display
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Lvl ");
  lcd.print(currentLevel);
  lcd.print(" finished!");
  lcd.setCursor(0,1);
  lcd.print("Score: ");
  lcd.print(score);

  // delete asteroids
  deleteAllAsteroids();
  
  //delete powerUps
  displayMatrix[powerUp[0]][powerUp[1]] = 0;
  displayMatrix[playerPos[0]][playerPos[1]] = 1;
  
  updateDisplay();
  resetPowerUp();
  
  sound->playSound(endLevelBuzzerFreq,endLevelBuzzerDuration);
  delay(5*oneSecondInMillis);
}
void Game::endGame(){
  // display palyer in case game ended while it was blinking
  displayMatrix[playerPos[0]][playerPos[1]] = 1;
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Game end!");
  delay(2*oneSecondInMillis);
  
  int highscorePos = checkAndSaveHighscore();

  // display Highscore message
  if(highscorePos > 0){
    sound->playSound(endGameCongratsBuzzerFreq,endGameCongratsBuzzerDuration);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Congrats!");
    lcd.setCursor(0,1);
    lcd.print("New highscore!");
    delay(2*oneSecondInMillis);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("You are now in");
    lcd.setCursor(0,1);
    lcd.print("place ");
    lcd.print(highscorePos);
    delay(2*oneSecondInMillis);
  }

  // end game details
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(playerName);
  lcd.setCursor(0,1);
  lcd.print("Score:");
  lcd.print(score);
  lcd.print(" >Menu");
  gameEnded = true;

  deleteAllAsteroids();
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

    // handle player-powerUp collision
    if(playerPos[0] == powerUp[0] && playerPos[1] == powerUp[1]){
      activatePowerUp();
    }
    
    // if the cell the player goes is 1 and is not a powerUp
    // then is a asteroid
    // handle player-asteroid collision
    else if( displayMatrix[playerPos[0]][playerPos[1]] == 1 ){
      lives--;
      sound->playSound(lostLifeBuzzerFreq,lostLifeBuzzerDuration);
      updateLivesDisplay();
      
      if(lives <= 0) return;

      //remove collided asteroid from asteroidList
      for(int i = 0; i < asteroidsList.size(); i++ ){
        int x = asteroidsList.get(i)->getX();
        int y = asteroidsList.get(i)->getY();
        if(x == playerPos[0] && y == playerPos[1]){
          deleteAsteroid(i);
          break;
        }
      }
    }  
    
    //display player movement
    displayMatrix[lastPlayerPos[0]][lastPlayerPos[1]] = 0;   
    displayMatrix[playerPos[0]][playerPos[1]] = 1;  
   } 
}

void Game::activatePowerUp(){
  int option;

  // chose randomly a powerUp based on level
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

  // lives++ powerUp
  if(option == 0){
    lives++;
    updateLivesDisplay();
  }
  // destroy asteroids powerUp
  else if(option == 1){
    // delete asteroids
    deleteAllAsteroids();
  }
  
  resetPowerUp();
  sound->playSound(powerUpActivationBuzzerFreq,powerUpActivationBuzzerDuration);
}

void Game::placeNewAsteroid(){
  Asteroid* ob = new Asteroid();
  // verify that the place the Asteroid spawns is empty
  while(displayMatrix[ob->getX()][ob->getY()] == 1){
    Serial.println("brrr");
    delete ob;
    ob = new Asteroid();
  }
  
  asteroidsList.add(ob);  
  displayMatrix[ob->getX()][ob->getY()] = 1;
  
  // display asteroid on matrix to avoid possible late call of updateDisplay function
  lc.setLed(0, ob->getX(), ob->getY(), 1);
}

void Game::updateAsteroidsPosition(){
  for(int i = 0; i < asteroidsList.size(); i++ ){
    int x = asteroidsList.get(i)->getX();
    int y = asteroidsList.get(i)->getY();

    displayMatrix[x][y] = 0;
    asteroidsList.get(i)->advance();
    
    x = asteroidsList.get(i)->getX();
    y = asteroidsList.get(i)->getY();    

    // if asteroid moved outside the matrix, delete it
    if(x > 7 || x < 0 || y < 0 || y > 7){
      deleteAsteroid(i);
      i--;
      continue;
    }
        
    // collision handler
    // asteroid-player collision, decrease number of lives, destroy asteroid
    if(x == playerPos[0] && y == playerPos[1]){
      lives--; 
      updateLivesDisplay();
      sound->playSound(lostLifeBuzzerFreq,lostLifeBuzzerDuration);
      if(lives <= 0) return;
      deleteAsteroid(i);
      i--;
    }
    // asteroid-powerUp collision, do nothing
    else if( isPowerUpDisplayed && x == powerUp[0] && y == powerUp[1] ){
      continue;
    }
    //asteroid-asteroid collision
    else if(displayMatrix[x][y] == 1){
      // at collision, delete just the current asteroid
      deleteAsteroid(i);
      i--;
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

//display power up based on chance
void Game::providePowerUp(){
 int chance = random(0,powerUpRandomness);
  if( chance != 0 ) return;
  // display powerUp on empty cell
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
  for(int i=0; i<highscoreListLength;i++){
    // addres of the last highscore
    int currentAddrs = highscoresAddrs + i*(highscoreListLength -1 + maxLengthName);
    int value = readFromEEPROM<int>(currentAddrs);
    
    if(score > value){
      // shift highscores downards
      if( i < highscoreListLength-1 ){
        int nextPlaceAddrs = currentAddrs + scoreByteLength + maxLengthName;
        for(int addrs = nextPlaceAddrs + (highscoreListLength-1-i)*(scoreByteLength + maxLengthName) -1; addrs >= nextPlaceAddrs ; addrs-- ){
          int bit = EEPROM.read(addrs - scoreByteLength - maxLengthName);
          EEPROM.write(addrs, bit);
        }
      }
      //save to EEPROM
      saveToEEPROM<int>(currentAddrs, score);
      for(int pos = currentAddrs + scoreByteLength; pos < currentAddrs + scoreByteLength + maxLengthName; pos++){
        if(playerName.length() > pos-currentAddrs-scoreByteLength){
          saveToEEPROM<char>(pos, playerName[pos-currentAddrs-scoreByteLength]);
        }
        else saveToEEPROM<char>(pos, 0);
      }
      // returns the position the score is saved in the highscore list
      return i+1; 
    }
  }
  return 0;  
}

void Game::deleteAsteroid(int pos){
  delete(asteroidsList.get(pos));
  asteroidsList.remove(pos);
}

void Game::deleteAllAsteroids(){
  while(asteroidsList.size() != 0){   
    int x = asteroidsList.get(0)->getX();
    int y = asteroidsList.get(0)->getY();
    displayMatrix[x][y] = 0;
    delete(asteroidsList.get(0));
    asteroidsList.remove(0);
  }
}

// formula to determine the maxAsteroids number
int Game::maxAsteroids(){
  return defaultMaxAsteroids + (currentLevel/3) - (currentLevel/14);
}

int Game::calculatePlayerSpeed(){
  return minPlayerSpeed + playerSpeedStep*(playerSpeedMaxSteps- readFromEEPROM<int>(playerSpeedAddrs));
}

void Game::updateDisplay(){
  for(int row = 0; row < matrixSize; row++){
    for(int col = 0; col < matrixSize; col++){
        lc.setLed(0, row, col, displayMatrix[row][col]); 
      }
  }
}
