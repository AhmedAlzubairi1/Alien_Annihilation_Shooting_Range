
#include "TargetClass.h"
#include <Servo.h>
#include <LiquidCrystal.h>
//Library to store high score
#include <EEPROM.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int numberOfTargets = 12;
//44,42,40,38,36,34,32,30,28,26,24,22
Target* arrayOfTargets[numberOfTargets] = {new Target(22, 1), new Target(24, 2), new Target(26, 3), new Target(28, 4), new Target(30, 5), new Target(32, 6), new Target(34, 7), new Target(36, 8), new Target(38, 9), new Target(40, 10), new Target(42, 11), new Target(44, 12)};
// Target* arrayOfTargets[numberOfTargets]={new Target(8,1)};//, new Target(9,2) , new Target(7,3), new Target(44,15), };
//Digital Pins 1 and 0 don't work
//Digital Pins Used: 2,3,4,5,7,8,9,11,12,13     new Target(1,3)
//Analog Pins Used:1,2,3
int gameButton = 13;
bool startGame = false;
int levelTimer = 0;
int laserThreshHold = 250; //was 150 originally
int score[2];
int highScore = 0;
int buzzerPin = 8;

//cite for @Buzzer
const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;
//Ester Egg
int esterTarget = 0;
bool activateEster = false;
int esterLimit = 6;


int counter = 0;
void setup() {
  pinMode(10, OUTPUT);
  pinMode(gameButton, INPUT);
  pinMode(buzzerPin, OUTPUT);
  lcd.begin(16, 2);
  for (Target* x : arrayOfTargets) {
    x->servoTime();
  }
  Serial.begin(9600);
  //printScreen("Hello",1);
  resetTargets();
  highScore = EEPROM.read(0); //get the previously saved value from EEPROM address zero
  Serial.println("Current high score is " + String(highScore));


}

void loop() {
  Serial.println("Current high score is " + String(highScore));

  if (digitalRead(gameButton) == HIGH) {
    startGame = true;
  }
  if (startGame == true) {
    digitalWrite(10, HIGH);
    //Initiallly this delay delay(3000);
    delay(500);
    //Next two lines to reset player points
    score[0] = 0;
    score[1] = 0;
    gameStart("One");
    gameActivate(0);

    for (Target* x : arrayOfTargets) {
      x->hide();
    }

    if (activateEster == true) {
      activateEster = false;
      launchEsterEgg();


    }

    digitalWrite(buzzerPin, LOW);
    printScreen("Player 1 Score", 1);
    printScreen(String(score[0]), 2);
    delay(3000);
    gameStart("Two");
    gameActivate(1);

    for (Target* x : arrayOfTargets) {
      x->hide();
    }
    if (activateEster == true) {
      activateEster = false;
      launchEsterEgg();


    }
    digitalWrite(buzzerPin, LOW);
    result(score);
    startGame = false;
    if (score[0] > highScore | score[1] > highScore)
    {
      printScreen("NEW HIGHSCORE!!", 1);
      if (score[0] > score[1]) {
        highScore = score[0];//THIS IS TO PERMENATULY STORE DATA
        EEPROM.write(0, highScore);
      }
      else {
        highScore = score[1];
        EEPROM.write(0, highScore);

      }
    }
    else
    {
      printScreen("Highscore", 1);
      printScreen(String(highScore), 2);
    }
    score[0] = 0;
    score[1] = 0;

  }
  digitalWrite(10, LOW);
}
void result(int score[])
{
  printScreen("Player 1:" + String(score[0]), 1);
  printScreen("Player 2:" + String(score[1]), 2);
  delay(7000);
  printScreen("", 2);
  if (score[0] > score[1]) {
    printScreen("Player 1 WINS!", 1);
    printScreen("Score: " + String(score[0]), 2);

  }
  else if (score[1] > score[0]) {
    printScreen("Player 2 WINS!", 1);
    printScreen("Score: " + String(score[1]), 2);
  }
  else printScreen("TIE!!", 1);
  delay(10000);
}
void gameActivate(int player) {
  int timeElapsed;

  int currentTime = millis() / 1000;
  bool handOver = true;
  int servosLeft;
  long handTime = 0;
  long buzzertime = 0;
  int prevScore = 0;
  int prevTime = 60;
  int delayLevel;
  //Remember to increase player points
  while (millis() / 1000 < currentTime + 60) {
    timeElapsed = (millis() / 1000) - currentTime;

    //should be modified to only print when string changes to avoid flicker
    if (prevTime != 60 - timeElapsed || score[player] != prevScore)
    {
      printScreen(String(60 - timeElapsed), 2);
      printScreen(String(score[player]), 1);
      prevScore = score[player];
      prevTime = 60 - timeElapsed;
    }
    if (millis() > buzzertime + 50) {
      digitalWrite(buzzerPin, LOW);
    }
    if ((servosLeft <= 0 || millis() >= handTime + (timeElapsed / 3 * 100) + 500) && millis() >= handTime + 500) {
      handOver = true;
    }
    if (handOver == true) {
      Serial.print("handover");
      delay(300);
      for (Target* x : arrayOfTargets) {
        x->setHit(false);
        x->hide();
      }
      playTargets(timeElapsed);
      //Note: We need to activate targets in play Targets
      servosLeft = (timeElapsed / 15) * 2 + 1;
      // Serial.print(String(servosLeft));
      handTime = millis();
      handOver = false;
    }

    for (Target* x : arrayOfTargets) {
      //Serial.print(analogRead(x->getAnalogPin()));
      if (analogRead(x->getAnalogPin()) > x->getLimit() && x->getHit() == false) {
        score[player]++;
        //-----Code for esterEgg
        //    Serial.print("TARGET IS DONE AT ");
        //    Serial.println(x->getDigitalPin());
        // Serial.println();
        // Serial.println("Digital Pin is " + String(x->getDigitalPin()));
        if (x->getDigitalPin() == 32 || x->getDigitalPin() == 30) {
          /* 44 42 40 38
             36 34 32 30
             28 26 24 22
          */
          esterTarget++;
        }
        else {
          esterTarget = 0;
        }

        /*

           f=
           s=
           rd=
           4th=
           5th=
           6th=
        */
        //code for ester egg---
        x->setHit(true);
        x->hide();
        servosLeft--;
        digitalWrite(buzzerPin, HIGH);
        buzzertime = millis();
      }
      if (esterTarget >= esterLimit) {
        activateEster = true;
        break;

      }
    }
    //Ester egg break
    if (activateEster == true) {
      break;

    }
  }
  esterTarget = 0;

}
void playTargets(int timeElapsed)



//seems to be playing one fewer target than it should
{
  int previousRandom = -1000;
  int currentRandom = -1;
  // int[] usedTargets = new int[timeElapsed/40+1];
  int usedTargets[numberOfTargets];

  for (int i = 0; i < numberOfTargets; i++)
  {
    usedTargets[i] = -1;
  }
  bool alreadyExists = true;
  for (int i = 0; i < (timeElapsed / 15) * 2 + 1; i++)
  {

    alreadyExists = true;
    if (timeElapsed / 15 == 0)
    {
      if (currentRandom == -1) {
        currentRandom = random(0, numberOfTargets);
      }
      else {
        while (previousRandom == currentRandom)
          currentRandom = random(0, numberOfTargets);
      }
      usedTargets[0] = currentRandom;
      previousRandom = currentRandom;
    }
    else
    {
      if (i == 0)
      {
        usedTargets[i] = random(0, numberOfTargets);
      }
      else
      {
        while (alreadyExists)
        {
          currentRandom = random(0, numberOfTargets);
          alreadyExists = false;
          for (int j = 0; j < i; j++)
          {
            if (usedTargets[j] != -1 && currentRandom == usedTargets[j])
            {
              alreadyExists = true;
            }
          }
        }
        usedTargets[i] = currentRandom;
      }
    }
  }

  for (int x = 0; x < numberOfTargets; x++ ) {
    Serial.print(usedTargets[x]);
    if (usedTargets[x] != -1) arrayOfTargets[usedTargets[x]]->expose();
  }
}
void gameStart(String player) {
  printScreen("Round " + player + " Start", 1);
  printScreen("                ", 2);
  delay(3000);
  //resetTargets();
  printScreen("Player" + player, 1);
  printScreen("Get Ready!", 2);
  delay(2000);//Initially 3000
  printScreen("Game is Starting", 1);
  printScreen("In 5 Seconds", 2);
  delay(1500);//Initially 3000
  printScreen("COUNTDOWN", 1);
  countDown(5, 2);
}
void printScreen(String x, int line) {

  lcd.setCursor(0, line - 1);
  lcd.print("                ");
  lcd.setCursor(0, line - 1);
  lcd.print(x);
}
void resetTargets() {

  levelTimer = 5;
  //This makes the current level timer to be 5 seconds
  //Note for future code: to check each target if its hit, the digital reading (x) must be greater than the target's limit
  for (Target* x : arrayOfTargets) {
    x->expose();
    delay(1000);
    //I should read the analog
    int reading = analogRead(x->getAnalogPin());
    //CHECK IF CODE IS BY REFERENCE: CHECK IF BOTTOM CODE ACTUALLY ALTERS THE ARRAY
    //Bottom code sets the limits for each target's resistence. Whenever they pass that limit, player gets a point
    x->setLimit(reading + laserThreshHold);

    //Now I set all servos to neutralState AKA the state where they are hidden.

    x->hide();
    Serial.print("Hiding");
    //IMPORTANT FOR READERS: I wanted to ask, can I write this code? Does it work? Like what I am trying to do here is attach the myServo object to each pin and alter each servos that the targets have.
    //Is there any possible issues w/ this

    //I should add digitalReading (x) and limit (level) in target

    //NOTE TO SELF: I SHOULD SET STATE OF ALL RESISToRS HERE TOO
  }


}
void countDown(int x, int y) {
  for (int i = x; i > 0; i--) {
    printScreen(String(i), y);
    delay(1000);
  }

}
void classChange(int x, bool show) {
  if (x == 44) {
    if (show == true)
      arrayOfTargets[11]->expose();
    else
      arrayOfTargets[11]->hide();
  }
  else if (x == 42) {
    if (show == true)
      arrayOfTargets[10]->expose();
    else
      arrayOfTargets[10]->hide();
  }
  else if (x == 40) {
    if (show == true)
      arrayOfTargets[9]->expose();
    else
      arrayOfTargets[9]->hide();
  }
  else if (x == 38) {
    if (show == true)
      arrayOfTargets[8]->expose();
    else
      arrayOfTargets[8]->hide();
  }
  else if (x == 36) {
    if (show == true)
      arrayOfTargets[7]->expose();
    else
      arrayOfTargets[7]->hide();
  }
  else if (x == 34) {
    if (show == true)
      arrayOfTargets[6]->expose();
    else
      arrayOfTargets[6]->hide();
  }
  else if (x == 32) {
    if (show == true)
      arrayOfTargets[5]->expose();
    else
      arrayOfTargets[5]->hide();
  }
  else if (x == 30) {
    if (show == true)
      arrayOfTargets[4]->expose();
    else
      arrayOfTargets[4]->hide();
  }
  else if (x == 28) {
    if (show == true)
      arrayOfTargets[3]->expose();
    else
      arrayOfTargets[3]->hide();
  }
  else if (x == 26) {
    if (show == true)
      arrayOfTargets[2]->expose();
    else
      arrayOfTargets[2]->hide();
  }
  else if (x == 24) {
    if (show == true)
      arrayOfTargets[1]->expose();
    else
      arrayOfTargets[1]->hide();
  }
  else if (x == 22) {
    if (show == true)
      arrayOfTargets[0]->expose();
    else
      arrayOfTargets[0]->hide();

  }



  // 0-11
}
void launchEsterEgg() {

  //Play first section
  Serial.println("*****************************FIRST Start************");

  firstSection();
  //Play second section
  Serial.println("*****************************Second Start************");

  secondSection();

  Serial.println("*****************************Third Start************");



  //Variant 1
  beep(f, 250);
  classChange(44, true);

  beep(gS, 500);
  classChange(38, true);
  beep(f, 350);
  classChange(44, false);
  beep(a, 125);
  classChange(28, true);
  beep(cH, 500);
  classChange(22, true);
  beep(a, 375);
  classChange(28, false);
  beep(cH, 125);
  classChange(22, false);
  beep(eH, 650);
  classChange(30, true);
  for (Target* x : arrayOfTargets) {
    x->hide();
  }
  //delay(500);
  Serial.println("*****************************Fourth SAME AS SecondStart************");

  //Repeat second section
  secondSection();
  Serial.println("*****************************Fifth DONE************");

  //44,42,40,38,
  //36,34,32,30,
  //28,26,24,22


  //Variant 2
  beep(f, 250);
  classChange(44, true);
  beep(gS, 500);
  classChange(40, true);
  beep(f, 375);
  classChange(44, false);
  beep(cH, 125);
  classChange(28, true);
  beep(a, 500);
  classChange(30, true);
  beep(f, 375);
  classChange(44, true);
  beep(cH, 125);
  classChange(28, false);
  beep(a, 650);
  classChange(30, false);
  for (Target* x : arrayOfTargets) {
    x->hide();
  }

}

void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(buzzerPin, note, duration);

  //Play different LED depending on value of 'counter'
  if (counter % 2 == 0)
  {
    //   digitalWrite(ledPin1, HIGH);
    delay(duration);
    // digitalWrite(ledPin1, LOW);

  } else
  {

    //digitalWrite(ledPin2, HIGH);
    delay(duration);
    //digitalWrite(ledPin2, LOW);

  }
  Serial.println(duration);

  //Stop tone on buzzerPin
  noTone(buzzerPin);

  delay(50);

  //Increment counter
  counter++;
}

void firstSection()
{


  beep(a, 500);
  classChange(44, true);
  beep(a, 500);
  classChange(44, false);
  beep(a, 500);
  classChange(44, true);
  beep(f, 350);
  classChange(38, true);
  beep(cH, 150);
  classChange(36, true);
  beep(a, 500);
  classChange(44, false);
  beep(f, 350);
  classChange(38, false);
  beep(cH, 150);
  classChange(28, true);
  beep(a, 650);
  classChange(44, true);

  delay(500);

  beep(eH, 500);
  classChange(22, true);
  beep(eH, 500);
  classChange(22, false);
  beep(eH, 500);
  classChange(22, true);
  beep(fH, 350);
  classChange(34, true);
  beep(cH, 150);
  classChange(28, false);

  beep(gS, 500);
  classChange(24, true);
  beep(f, 350);
  classChange(38, true);
  beep(cH, 150);
  classChange(28, true);

  beep(a, 650);
  classChange(44, false);
  for (Target* x : arrayOfTargets) {
    x->hide();
  }
  delay(500);
}

void secondSection()
{




  beep(aH, 500);
  classChange(26, true);
  beep(a, 300);
  classChange(44, true);
  beep(a, 150);
  classChange(44, false);
  beep(aH, 500);
  classChange(26, false);
  beep(gSH, 325);
  classChange(28, true);
  beep(gH, 175);
  classChange(30, true);
  beep(fSH, 125);
  classChange(34, true);
  beep(fH, 125);
  classChange(32, true);
  beep(fSH, 250);
  classChange(34, false);

  delay(325);

  beep(aS, 250);
  classChange(42, true);
  beep(dSH, 500);
  classChange(38, true);
  beep(dH, 325);
  classChange(24, true);
  beep(cSH, 175);
  classChange(40, true);
  beep(cH, 125);
  classChange(22, true);
  beep(b, 125);
  classChange(44, true);
  beep(cH, 250);
  classChange(22, false);
  for (Target* x : arrayOfTargets) {
    x->hide();
  }
  delay(350);
}
