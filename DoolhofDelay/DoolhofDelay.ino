#include <Arduino.h>
#include <WiFi.h> //WiFi library
#include <WebSocketsClient.h> //websocket library
#include <ArduinoJson.h> //arduino JSON library
#include <analogWrite.h> //analogWrite library

#include <Adafruit_VL53L0X.h> //liDAR library
#include <Adafruit_GFX.h> //graphics library
#include <Adafruit_SSD1306.h> //display library
int amount = 0;

Adafruit_VL53L0X lidar = Adafruit_VL53L0X(); //LiDAR variable

//declaring the screen.
Adafruit_SSD1306 display(128, 64, &Wire, 4);

int IRPinR = 34; //right IR sensor
int IRPinL = 39; //left IR sensor
int IRValR;
int IRValL;

int motorPinLV = 16;  //Rechterwiel achteruit
int motorPinLA = 17;  //Rechterwiel vooruit
int motorPinRA = 5;   //Linkerwiel vooruit
int motorPinRV = 18;  //Linkerwiel achteruit

float motorR = 255.0f;
float motorL = 255.0f;

//declaring the variables
String colourR = "";
String colourL = "";

int whiteValL = 85;
int whiteValR = 80;
int greyValL = 250;
int greyValR = 250;
int blackValL = 800;
int blackValR = 800;

void setup() {
//setup display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.display();

//setup pins
  Serial.begin(115200);
  pinMode(motorPinRA, OUTPUT);
  pinMode(motorPinRV, OUTPUT);
  pinMode(motorPinLV, OUTPUT);
  pinMode(motorPinLA, OUTPUT);

}

void loop() {
mazeLoop();

}

/*
 * --------------------------------------------------------------
 * -------------------------Functions----------------------------
 * --------------------------------------------------------------
 */

/*
 * Read the pins and give a colour
 */
void readPins() {
  IRValL = analogRead(IRPinL);
  IRValR = analogRead(IRPinR);
//if the sensor reads less than 80 it's white.
  if(IRValR <= whiteValR)  {
    colourR = "white";
//if the sensor read more than 80 and less than 250 it's grey.
  }else if(IRValR > whiteValR && IRValR <=greyValR)  {
    colourR = "grey";
//if the sensor reads more than 250 then it's black.
  }else if(IRValR > blackValR) {
    colourR = "black";
  }

//if the sensor reads less than 80 it's white.
  if(IRValL <= whiteValL)  {
    colourL = "white";
//if the sensor read more than 80 and less than 250 it's grey.
  }else if(IRValL >whiteValL && IRValL <=greyValL) {
    colourL = "grey";
//if the sensor reads more than 250 then it's black.
  }else if(IRValL >blackValL)  {
    colourL = "black";
  }
}

void writeToDisplay(String text, int x, int y) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(x, y);
  display.println(text);
  display.display();
}

void motorInit() {
  pinMode(motorPinRA, OUTPUT);
  pinMode(motorPinRV, OUTPUT);
  pinMode(motorPinLV, OUTPUT);
  pinMode(motorPinLA, OUTPUT);
}

void standStill() {
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
}

void driveForward(double percentage) {
  int speedR = int((motorR / 100.0f) * percentage);
  int speedL = int((motorL / 100.0f) * percentage);

  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, speedR);
  analogWrite(motorPinLV, speedL);
  analogWrite(motorPinLA, 0);
}

void driveBackwards(double percentage) {
  int speedR = int((motorR / 100.0f) * percentage);
  int speedL = int((motorL / 100.0f) * percentage);

  analogWrite(motorPinRA, speedR);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, speedL);
}

void turnLeft(double percentage) {
  int speedR = int((motorR / 100.0f) * percentage);
  int speedL = int((motorL / 100.0f) * percentage);

  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, speedR);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, speedL);
}

void turnRight(double percentage) {
  int speedR = int((motorR / 100.0f) * percentage);
  int speedL = int((motorL / 100.0f) * percentage);

  analogWrite(motorPinRA, speedR);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, speedL);
  analogWrite(motorPinLA, 0);
}

void turnLeftMaze(double percentage) {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((255.0f / 100.0f) * percentage);
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, speedR);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
}

void turnRightMaze(double percentage) {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((255.0f / 100.0f) * percentage);
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, speedL);
  analogWrite(motorPinLA, 0);
}

void turn180Maze(double percentage)  {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((225.0f / 100.0f) * percentage);
  analogWrite(motorPinRA, speedR);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, speedL);
  analogWrite(motorPinLA, 0);
  delay(1500);
}

/*
 * turn 90 degrees backwards 
 */
void turn90Backwards(double percentage)  {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((225.0f / 100.0f) * percentage);
  analogWrite(motorPinRA, 255);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
  delay(500);
}

/* 
 *  turn left 90
 */
void turnRight90Maze() {
    driveForward(75);
    delay(125);
    standStill();
    delay(500);
    turnRightMaze(75);
    delay(800);
    standStill();
    delay(1000);
}


/* 
 *  turn right 90
 */
void turnLeft90Maze() {
    driveForward(75);
    delay(100);
    standStill();
    delay(500);
    turnLeftMaze(77);
    delay(950);
    standStill();
    delay(1000);
}

void mazeLoop() {
  //detect the different tapes.
  readPins();

  /*
   * display
   */
  //clear the display
  display.clearDisplay();

  //write the values from the IR sensor on the display
  display.setCursor(0, 0);            // Start at top-left corner
  display.print("PinR: ");
  display.println(IRValR);
  display.print("PinL: ");
  display.println(IRValL);
  display.display();

//this works for following the line of the maze.
//If both are white then drive forward
  if(colourL == "white" && colourR == "white")  
  {
    driveForward(75);
    amount = 0;
  }
//if left is grey then adjust to the left.
  else if(colourL == "grey" && colourR == "white") 
  {
    turnLeftMaze(75);
    amount = 0;
  }
//if right is grey then adjust to the right.
  else if(colourL == "white" && colourR == "grey") 
  {
    turnRightMaze(75);
    amount = 0;
  }
//if right is black then turn 90 degrees to the right.
  else if(colourL == "white" && colourR == "black")
  {
    turnRight90Maze();
    amount = 0;
  }
//if right is black then turn 90 degrees to the right.
  else if(colourL == "grey" && colourR == "black")
  {
    turnRight90Maze();
    amount = 0;
  }
//if both are grey then turn 180 degrees.
  else if(colourL == "grey" && colourR == "grey")  
  {
    standStill();
    amount = amount + 1;
    if(amount == 10){
      turn180Maze(75);
    }
  }
//if left is black then turn 90 degrees to the right.
  else if(colourL == "black" && colourR == "white")
  {
    turnLeft90Maze();
    amount = 0;
  }
//if left is black then turn 90 degrees to the right.
  else if(colourL == "black" && colourR == "grey")
  {
    turnLeft90Maze(); 
    amount = 0;  
  }
  else if(colourL == "black" && colourR == "black") {
    turnRight90Maze();
  }
//else just stand still.
  else
  {
    standStill();
    amount = 0;
  }
}
