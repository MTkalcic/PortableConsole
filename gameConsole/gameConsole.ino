//BUTTON INPUTS: 2,13,15,27

int scale = 20;
int buttonRight = 15;
int buttonDown = 2;
int buttonLeft = 13;
int buttonUp = 27;
int rotation = 0;
int snakePosX = 0;
int snakePosY = 100;
int appleX;
int appleY;
bool appleAte = 0;
int waitTime = 500;
unsigned long timeNow;
int oldRotation;
int xCoords[1000];
int yCoords[1000];
int enemyLevel[1000];
int steps = 0;
int bigness = 0;
int spaceshipX = 0;
int spaceshipY = 120;
int oldX;
int oldY;
int joystickY = 33;
int joystickX = 12;
int movement;
int bulletLive = 0;
int inputMade = 0;
int bulletX;
int bulletY;
int bulletDir;
int numOfEnemies;
int hit = 0;
int moveHelp;
int start = 1;
int yPos1;
int yPos2;
int yPosOld;
int enemyTimer = 1501;
int gameChoice = 0;
int gameChosen = 3;
int score = 0;
int enemiesAlive = 0;
int color = 7;
bool createEnemy = 0;
bool bulletOnScreen;
bool changedDirection;
bool choiceMade = 0;
bool firstTime;
bool died = 0;
String snakeHead;
String drawSquare = "fill ";
String drawBox = "draw ";
String drawCirc = "cirs ";
String startScreen = "page page0";
String cleanScreen = drawSquare + "0,0,320,240,BLACK";
String drawEnemy;
String deleteEnemy;
String colors[8] = {",BLACK", ",BLUE", ",BROWN", ",GREEN", ",YELLOW", ",RED", ",GRAY", ",WHITE"};
String scoreShip = "xstr 255,220,85,20,0,WHITE,BLACK,2,2,1,";
String enemiesCounter = "xstr 170,220,85,20,0,WHITE,BLACK,0,2,1,";
String displayNumOfEnemies;
String scoreSnake = "xstr 65,56,190,128,3,WHITE,BLACK,1,1,1,";
String choiceBox;


int SnakeRotation(int snakeRot){
  int rotated = 0;
  if ((digitalRead(buttonRight) == 0 or analogRead(joystickX) >= 4000) and rotated == 0 and snakeRot != 2){
    snakeRot = 0;
    rotated = 1;
  }
  if((digitalRead(buttonDown) == 0 or analogRead(joystickY) >= 4000) and rotated == 0 and snakeRot != 1){
    snakeRot = 3;
    rotated = 1;
  }
  if((digitalRead(buttonLeft) == 0 or analogRead(joystickX) <= 400) and rotated == 0 and snakeRot != 0){
    snakeRot = 2;
    rotated = 1;
  }
  if((digitalRead(buttonUp) == 0 or analogRead(joystickY) <= 400) and rotated == 0 and snakeRot != 3){
    snakeRot = 1;
    rotated = 1;
  }
  return snakeRot;
}

void Movement(int rotation,int *snakePosX, int *snakePosY){
  switch(rotation){
    case 0:
      *snakePosX += scale;
      *snakePosY == *snakePosY;
      break;
    case 1:
      *snakePosX == *snakePosX;
      *snakePosY -= scale;
      break;
    case 2:
      *snakePosX -= scale;
      *snakePosY == *snakePosY;
      break;
    case 3:
      *snakePosX == *snakePosX;
      *snakePosY += scale;
      break;
  }
}

void AppleSpawner(bool *appleAte){
  if (*appleAte == 1){
    appleX = (random(301)/scale)*scale;
    appleY = (random(221)/scale)*scale;
    bigness += 1;
    *appleAte = 0;
  }
}

void Restart(){
  start = 1;
  bigness = 0;
  steps = 0;
  snakePosX = 0;
  snakePosY = 100;
  memset(xCoords,500,sizeof(xCoords));
  memset(yCoords,500,sizeof(yCoords));
  appleX = (random(320)/scale)*scale;
  appleY = (random(240)/scale)*scale;
  appleAte = 0;
  rotation = 0;
  changedDirection = 1;
  waitTime = 500;
  scale = 20;
  
  numOfEnemies = 0;
  memset(enemyLevel,0,sizeof(enemyLevel));
  spaceshipX = 0;
  spaceshipY = 100;
  enemiesAlive = 0;
  score = 0;
  enemyTimer = 1501;
}


void BulletMaths(int i){
  enemyLevel[i] -= 1;
  deleteEnemy = drawSquare + xCoords[i] + "," + yCoords[i] + "," + scale + "," + scale + colors[enemyLevel[i]];
  Serial1.print(deleteEnemy);
  Serial1.print("\xFF\xFF\xFF");
  hit = 1;
  movement = 320;
  if(enemyLevel[i] == 0){
    score += 10;
    enemiesAlive -= 1;
    if(score % 100 == 0){
      enemyTimer -= 100;
    }
    String displayScore = scoreShip + "\"Score: " + String(score) + "\"";
    displayNumOfEnemies = enemiesCounter + "\"Enemies: " + String(enemiesAlive) + "\"";
    Serial1.print(displayNumOfEnemies);
    Serial1.print("\xFF\xFF\xFF");
    Serial1.print(displayScore);
    Serial1.print("\xFF\xFF\xFF");
  }
}
void DrawBullet(){
  if(inputMade == 0){
    if(digitalRead(buttonDown) == 0){
      bulletDir = 0;
      inputMade = 1;
    }
    if(digitalRead(buttonRight) == 0){
      bulletDir = 1;
      inputMade = 1;
    }
    if(digitalRead(buttonUp) == 0){
      bulletDir = 2;
      inputMade = 1;
    }
    if(digitalRead(buttonLeft) == 0){
      bulletDir = 3;
      inputMade = 1;
    }  
  }
  if(inputMade == 1){
    if(bulletLive == 0){
      bulletX = spaceshipX;
      bulletY = spaceshipY;
      String bulletRechargeDelete = drawSquare + "0,230,170,10,WHITE"; //full movement / 2 == 170
      Serial1.print(bulletRechargeDelete);
      Serial1.print("\xFF\xFF\xFF");
    }
    bulletLive = 1;
    String drawBullet;
    String deleteBullet;
    switch(bulletDir){
      case 0:
        if(hit == 0){
          for(int i = 0; i <= numOfEnemies; i++){
            if(xCoords[i] == bulletX){
              if(yCoords[i] == (bulletY + movement)){
                if(enemyLevel[i] >= 1)
                  BulletMaths(i);
                break;
              }
            }
          }
        }
        drawBullet = drawSquare + (bulletX+((scale/2)-3)) + "," + (bulletY+(scale+movement)) + ",6," + (scale-4) + ",RED";
        deleteBullet = drawSquare + (bulletX+((scale/2)-3)) + "," + (bulletY+movement) + ",6," + (scale-4) + ",BLACK";
        break;
    case 1:
      if(hit == 0){
        for(int i = 0; i <= numOfEnemies; i++){
          if(yCoords[i] == bulletY){
            if(xCoords[i] == (bulletX + movement)){
              if(enemyLevel[i] >= 1)
                BulletMaths(i);
              break;
            }
          }
        }
      }
      drawBullet = drawSquare + (bulletX+(scale+movement)) + "," + (bulletY+((scale/2)-3)) + "," + (scale-4) + ",6,RED";
      deleteBullet = drawSquare + (bulletX+movement) + "," + (bulletY+((scale/2)-3)) + "," + (scale-4) + ",6,BLACK";
      break;
    case 2:
      if(hit == 0){
        for(int i = 0; i <= numOfEnemies; i++){
          if(xCoords[i] == bulletX){
            if(yCoords[i] == (bulletY - movement)){
              if(enemyLevel[i] >= 1)
                BulletMaths(i);
              break;
            }
          }
        }
      }
      drawBullet = drawSquare + (bulletX+((scale/2)-3)) + "," + (bulletY-(scale+movement)) + ",6," + (scale-4) + ",RED";
      deleteBullet = drawSquare + (bulletX+((scale/2)-3)) + "," + (bulletY-movement) + ",6," + (scale-4) + ",BLACK"; 
      break;
    case 3:
      if(hit == 0){
        for(int i = 0; i <= numOfEnemies; i++){
          if(yCoords[i] == bulletY){
            if(xCoords[i] == (bulletX - movement)){
              if(enemyLevel[i] >= 1)
                BulletMaths(i);
              break;
            }
          }
        }
      }
      drawBullet = drawSquare + (bulletX-(scale+movement)) + "," + (bulletY+((scale/2)-3)) + "," + (scale-4) + ",6,RED";
      deleteBullet = drawSquare + (bulletX-movement) + "," + (bulletY+((scale/2)-3)) + "," + (scale-4) + ",6,BLACK";
      break;
    }
    Serial1.print(drawBullet);
    Serial1.print("\xFF\xFF\xFF");
    Serial1.print(deleteBullet);
    Serial1.print("\xFF\xFF\xFF");
    movement+=scale;
    String bulletRecharge = drawSquare + "0,230," + (movement/2) + ",10,RED";
    Serial1.print(bulletRecharge);
    Serial1.print("\xFF\xFF\xFF");
    if(movement >= 340 or hit == 1){
      bulletLive = 0;
      inputMade = 0;
      movement = 0;
      hit = 0;
    }
  }
}

int GameChoosing(){
  if(died == 1){
    Serial1.print(startScreen);
    Serial1.print("\xFF\xFF\xFF");
    Restart();
    died = 0;
  }
  if (choiceMade == 0){
    yPosOld = yPos1;
    switch(gameChoice){
      case 0:
        yPos1 = 30;
        yPos2 = 70;
        break;
      case 1:
        yPos1 = 100;
        yPos2 = 140;
        break;
      case 2:
        yPos1 = 170;
        yPos2 = 210;
        break;
    }
    if((digitalRead(buttonDown) == 0 or analogRead(joystickY) >= 4000) and changedDirection == 0){
      gameChoice += 1;
      if(gameChoice > 2){
        gameChoice = 0;
      }
    }
    if(digitalRead(buttonUp) == 0 or analogRead(joystickY) <= 400 and changedDirection == 0){
      gameChoice -= 1;
      if(gameChoice < 0){
        gameChoice = 2;
      }
    }
    if (yPosOld != yPos1){
      choiceBox = drawBox + 60 + "," + yPosOld + "," + 260 + "," + (yPosOld+40) + ",WHITE";
      Serial1.print(choiceBox);
      Serial1.print("\xFF\xFF\xFF");
    }
    choiceBox = drawBox + 60 + "," + yPos1 + "," + 260 + "," + yPos2 + ",RED";
    Serial1.print(choiceBox);
    Serial1.print("\xFF\xFF\xFF");
    if(digitalRead(buttonRight) == 0){
      choiceMade = 1;
      firstTime = 1;
      return gameChoice;
    }
    return 3;
  }
}

void Art(String artScene){
  if(firstTime == 1){
    Serial1.print(artScene);
    Serial1.print("\xFF\xFF\xFF");
    delay(3000);
    Serial1.print(cleanScreen);
    Serial1.print("\xFF\xFF\xFF");
    firstTime = 0;
    if(gameChosen == 1){
      String bulletRecharge = drawSquare + "0,230,170,10,RED";
      Serial1.print(bulletRecharge);
      Serial1.print("\xFF\xFF\xFF");
    }
  }
}

void ShipMovement(){
  if(analogRead(joystickY) <= 400 and changedDirection == 0){
    spaceshipY -= scale;
    changedDirection = 1;
  }
  if(analogRead(joystickY) >= 4000 and changedDirection == 0){
    spaceshipY += scale;
    changedDirection = 1;
  }
  if(analogRead(joystickX) >= 4000 and changedDirection == 0){
    spaceshipX += scale;
    changedDirection = 1;
  }
  if(analogRead(joystickX) <= 400 and changedDirection == 0){
    spaceshipX -= scale;
    changedDirection = 1;
  }
}

int ColorChange(){
  if(digitalRead(buttonDown) == 0){
    if(color == 7){
      scale = 10;
    }
    color = 7;
  }
  if(digitalRead(buttonRight) == 0){
    color = 0;
  }
  if(digitalRead(buttonUp) == 0){
    if(color == 4){
      scale = 20;
    }
    color = 4;
  }
  if(digitalRead(buttonLeft) == 0){
    color = 5;
  }  
  return color;
}
void setup() {
  // put your setup code here, to run once:
  pinMode(buttonRight, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);
  pinMode(buttonLeft, INPUT_PULLUP);
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(joystickY, INPUT);
  randomSeed(analogRead(37));
  appleX = (random(301)/scale)*scale;
  appleY = (random(221)/scale)*scale;
  Serial.begin(9600);
  Serial1.begin(115200, SERIAL_8N1, 25, 26); //25 rx; 26 tx
  delay(1);
  Serial1.print(startScreen);
  Serial1.print("\xFF\xFF\xFF");
  pinMode(joystickX, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  timeNow = millis();
  if (choiceMade == 0){
    gameChosen = GameChoosing();
    changedDirection = 0;
    while(millis() < timeNow + 160){
    }
  }

  if(choiceMade == 1){
    if(gameChosen == 0){
      String art = "page page1";
      Art(art);
      changedDirection = 0;
      Movement(rotation, &snakePosX, &snakePosY);
      if(snakePosX >= 320 or snakePosX < 0 or snakePosY >= 240 or snakePosY < 0){
        choiceMade = 0;
        died = 1;
      }
      xCoords[steps] = snakePosX;
      yCoords[steps] = snakePosY;
    
    
      if(snakePosX == appleX && snakePosY == appleY){
        appleAte = 1;
        score += 1;
        String displayScore = scoreSnake + "\"" + String(score) + "\"";
        Serial1.print(displayScore);
        Serial1.print("\xFF\xFF\xFF");
      }
      
      AppleSpawner(&appleAte);
      String stringApple = drawCirc + (appleX+(scale/2)) + "," + (appleY+(scale/2)) + "," + ((scale/2)-1) + ",GREEN";
      Serial1.print(stringApple);
      Serial1.print("\xFF\xFF\xFF");
    
      snakeHead = drawSquare + snakePosX + "," + snakePosY + "," + scale + "," + scale + ",WHITE";
      Serial1.print(snakeHead);
      Serial1.print("\xFF\xFF\xFF");
      for(int i = 1; i <= bigness; i++){
        String stringTwo = drawSquare + xCoords[steps - i] + "," + yCoords[steps - i] + "," + scale + "," + scale + ",WHITE";
        if(snakePosX == xCoords[steps - i] and snakePosY == yCoords[steps-i]){
          choiceMade = 0;
          died = 1;
          break;
        }
        Serial1.print(stringTwo);
        Serial1.print("\xFF\xFF\xFF");
      }
      steps += 1;
      if (steps % 100 == 0){
        waitTime -= 50;
      }
      else if (steps % 501 == 0){
        Serial1.print(cleanScreen);
        Serial1.print("\xFF\xFF\xFF");
        scale = 10;
      }
      while(millis() < timeNow + waitTime){
        if(changedDirection == 0){
          oldRotation = rotation;
          rotation = SnakeRotation(rotation);
          if (oldRotation != rotation)
            changedDirection = 1;
        }
      }
      String cleanScreen = drawSquare + xCoords[steps - bigness-1] + "," + yCoords[steps - bigness-1] + "," + scale + "," + scale + ",BLACK";
      Serial1.print(cleanScreen);
      Serial1.print("\xFF\xFF\xFF");
    }
    if (gameChosen == 1){
      String art = "page page3";
      Art(art);

      while(millis() < timeNow + waitTime){
        if(millis() % 100 == 0){
          DrawBullet();
          ShipMovement();
          for(int i = 0; i <= numOfEnemies; i++){
            if(xCoords[i] == spaceshipX and yCoords[i] == spaceshipY){
              if(enemyLevel[i] > 0){
                choiceMade = 0;
                died = 1;
              }
            } 
          }
          if (spaceshipX >= 320 or spaceshipX < 0 or spaceshipY >= 220 or spaceshipY < 0 or enemiesAlive > 10){
            choiceMade = 0;
            died = 1;
          }
        }
    
        if(millis() % 201 == 0){
          if (oldX != spaceshipX or oldY != spaceshipY){
            String deleteShip = drawSquare + oldX + "," + oldY + "," + scale + "," + scale + ",BLACK";
            Serial1.print(deleteShip);
            Serial1.print("\xFF\xFF\xFF");
          }
          oldX = spaceshipX;
          oldY = spaceshipY;
          String drawShip = drawSquare + spaceshipX + "," + spaceshipY + "," + scale + "," + scale + ",WHITE";
          Serial1.print(drawShip);
          Serial1.print("\xFF\xFF\xFF");
          changedDirection = 0; 
        }
    
        if(millis() % enemyTimer == 0){
          int leveledUp = 0;
          xCoords[numOfEnemies] = (random(320)/scale)*scale;
          yCoords[numOfEnemies] = (random(220)/scale)*scale;
          if(xCoords[numOfEnemies] != spaceshipX or yCoords[numOfEnemies] != spaceshipY){
            for(int i = 0; i <= (numOfEnemies-1); i++){
              if(xCoords[numOfEnemies] == xCoords[i] and yCoords[numOfEnemies] == yCoords[i]){
                if(enemyLevel[i] == 0){
                  enemiesAlive += 1;
                  displayNumOfEnemies = enemiesCounter + "\"Enemies: " + String(enemiesAlive) + "\"";
                  Serial1.print(displayNumOfEnemies);
                  Serial1.print("\xFF\xFF\xFF");
                }
                enemyLevel[i] += 1;
                leveledUp = 1;
                drawEnemy = drawSquare + xCoords[i] + "," + yCoords[i] + "," + scale + "," + scale + colors[enemyLevel[i]];
                break;
              }
            }
            if(leveledUp == 0){
              enemyLevel[numOfEnemies] = 1;
              drawEnemy = drawSquare + xCoords[numOfEnemies] + "," + yCoords[numOfEnemies] + "," + scale + "," + scale + colors[enemyLevel[numOfEnemies]];
              numOfEnemies += 1;
              enemiesAlive += 1;
              displayNumOfEnemies = enemiesCounter + "\"Enemies: " + String(enemiesAlive) + "\"";
              Serial1.print(displayNumOfEnemies);
              Serial1.print("\xFF\xFF\xFF");
            }
          }
          Serial1.print(drawEnemy);
          Serial1.print("\xFF\xFF\xFF");
        }
          delay(1);
        }
      }
    if(gameChosen == 2){
      if(firstTime == 1){
        Serial1.print(cleanScreen);
        Serial1.print("\xFF\xFF\xFF");
        firstTime = 0;
      }
      if(!digitalRead(buttonDown) and !digitalRead(buttonLeft)){
        choiceMade = 0;
        died = 1;
      }
      while(millis() < timeNow + waitTime){
        if(millis() % 200 == 0){
          int oldBrushX = spaceshipX;
          int oldBrushY = spaceshipY;
          ShipMovement();
          String drawShip = drawSquare + oldBrushX + "," + oldBrushY + "," + scale + "," + scale + colors[ColorChange()];
          String drawShipOutline = drawBox + spaceshipX + "," + spaceshipY + "," + (spaceshipX+(scale-1)) + "," + (spaceshipY+(scale-1)) + colors[3];
          Serial1.print(drawShip);
          Serial1.print("\xFF\xFF\xFF");
          Serial1.print(drawShipOutline);
          Serial1.print("\xFF\xFF\xFF");
          changedDirection = 0;
        }
        delay(1);
      }
    }
  }
}
