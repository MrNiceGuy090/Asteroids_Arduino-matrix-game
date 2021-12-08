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
  score = 0;
  js = Joystick::getInstance();
  gameEnded = false;
  gameObjectsList = LinkedList<GameObject*>();
  
  playerPos[0] = 4;
  playerPos[1] = 4; 
  lastPlayerPos[0] = 4;
  lastPlayerPos[1] = 4; 
  
  matrixChanged = true;
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
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Lvl:");
  lcd.print(level);
  lcd.setCursor(0,1);
  lcd.print("Lives:");
  updateLives();
  updateScoreAndTimer();
}

void Game::placeNewGameObject(){
  GameObject* ob = new GameObject();
  while(displayMatrix[ob->getX()][ob->getY()] == 1)
    ob = new GameObject();
  gameObjectsList.add(ob);  
  displayMatrix[ob->getX()][ob->getY()] = 1;
}

void Game::playLevel(){
  if(millis() - lastDeployedGameObjectTime > gameObjectDeploymentInterval){
    updateGameObjectsPosition();
    placeNewGameObject();
    lastDeployedGameObjectTime = millis();
    updateDisplay();
  }
  if(millis() - lastMoved > moveInterval){
    updatePlayerPosition();
    lastMoved = millis(); 
  }
  if(matrixChanged == true){
    matrixChanged = false;
    updateDisplay();
  }
  if(js->hasButtonBeenPressed()){
    lives--;
    updateLives();
  }
  
  if(lives == 0){
    delay(1000);
    endGame();
  }
  
}

void Game::updateLives(){
  lcd.setCursor(6, 1);
  lcd.print(" ");
  lcd.print(lives);  
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
    score+=1;
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
  delay(5000);
}
void Game::endGame(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Game end!");
  delay(2000);
  int highscorePos = checkAndSaveHighscore();
  if(highscorePos > 0){
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
  lcd.print(" >Menu");
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
    else{
      playerPos[0] = 0;
    }
  }
  if(js->isUp()){
    if(playerPos[0] > 0){
      playerPos[0]--;
    }
    else{
      playerPos[0] = matrixSize - 1;
    }
  }

  if(js->isRight()){
    if(playerPos[1] < matrixSize - 1){
      playerPos[1]++;
    }
    else{
      playerPos[1] = 0;
    }
  }
  if(js->isLeft()){
    if(playerPos[1] > 0){
      playerPos[1]--;
    }
    else{
      playerPos[1] = matrixSize - 1;
    }
  }

  if(lastPlayerPos[0] != playerPos[0] || lastPlayerPos[1] != playerPos[1]){
    matrixChanged = true; 
    if( displayMatrix[playerPos[0]][playerPos[1]] == 1 ){
      lives--;
      updateLives();
      if(lives == 0) return;
    }
    displayMatrix[playerPos[0]][playerPos[1]] = 1;
    displayMatrix[lastPlayerPos[0]][lastPlayerPos[1]] = 0;    
   } 
}

void Game::updateGameObjectsPosition(){
  for(int i = 0; i < gameObjectsList.size()-1; i++ ){
    int x = gameObjectsList.get(i)->getX();
    int y = gameObjectsList.get(i)->getY();
    
    if(displayMatrix[x][y] == 0){
      delete(gameObjectsList.get(i));
      gameObjectsList.remove(i);
      continue;
    }
    else 
      displayMatrix[x][y] = 0;
      
    gameObjectsList.get(i)->advance();
    
    x = gameObjectsList.get(i)->getX();
    y = gameObjectsList.get(i)->getY();    
    
    if(x > 7 || x < 0 || y < 0 || y > 7){
      delete(gameObjectsList.get(i));
      gameObjectsList.remove(i);
      continue;
    }
    
    if(displayMatrix[x][y] == 1){
      if(x == playerPos[0] && y == playerPos[1]){
        lives--; 
        updateLives();
        if(lives == 0) return;
      }
      delete(gameObjectsList.get(i));
      gameObjectsList.remove(i);
    
    }
    else{
      displayMatrix[x][y] = 1;
    }
  }
}

void Game::updateDisplay(){
  for(int row = 0; row < matrixSize; row++){
    for(int col = 0; col < matrixSize; col++){
        lc.setLed(0, row, col, displayMatrix[row][col]); 
      }
  }
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
