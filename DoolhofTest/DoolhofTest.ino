//include libraries that we need.
#include <Arduino.h>
#include <analogWrite.h>
#include <Adafruit_VL53L0X.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>  
#include "WiFi.h"

//declaring the pins
int motorPinLV = 16;
int motorPinLA = 17;
int motorPinRV = 18;
int motorPinRA = 5;
int IRPinR = 39;
int IRPinL = 34;

//declaring the variables
int pinR;
int pinL;
String colourR = "";
String colourL = "";

Adafruit_SSD1306 display(128, 64, &Wire, 4);

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
//setup display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
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
readPins();
/*
 * display
 */
//clear the display
  display.clearDisplay();

//write the values from the IR sensor on the display
  display.setCursor(0,0);             // Start at top-left corner
  display.print("PinL: ");
  display.println(colourL);
  display.print("PinR: ");
  display.println(colourR);
  display.display();

//If both are white then drive forward
if(colourL == "white" && colourR == "white")
{
  driveForward(100);
}
//if right is grey then adjust to the right.
else if(colourL == "white" && colourR == "grey")
{
  turnRight(100);
}
//if right is black then turn 90 degrees right and if there is a road ahead.
else if(colourL == "white" && colourR == "black")
{
  fancy function
}
//if left is grey then adjust to the left.
else if(colourL == "grey" && colourR == "white")
{
  turnLeft(100);
}
//if both are grey then turn 180 degrees.
else if(colourL == "grey" && colourR == "grey")
{
  turn180();
}
//if right is black then turn right and check if there is a road ahead.
else if(colourL == "grey" && colourR == "black")
{
  fancy function
}
//If left is black then turn 90 degrees to the left.
if(colourL == "black" && colourR == "white")
{
  turnLeft90();
}
//if left is black then turn 90 degrees to the left.
if(colourL == "black" && colourR == "grey")
{
  turnLeft90();
}
//if both are black then turn 90 degrees to the left.
else if(colourL == "black" && colourR == "black")
{
  turnLeft90();
}
else
{
  standStill();
}
//  if(colourL == "white" && colourR == "white")  {
//    driveForward(100);
//  }else if(colourL == "grey" && colourR == "white") {
//    turnLeft(100);
//  }else if(colourL == "white" && colourR == "grey") {
//    turnRight(100);
//  }else if(colourL == "grey" && colourR == "grey")  {
//    standStill();
//  }else{
//    standStill();
//  }
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
pinL = analogRead(IRPinL);
pinR = analogRead(IRPinR);

  if(pinR <= 85)  {
    colourR = "white";
  }else if(pinR > 85 && pinR <=1100)  {
    colourR = "grey";
  }else if(pinR > 1100) {
    colourR = "black";
  }

  if(pinL <= 70)  {
    colourL = "white";
  }else if(pinL >70 && pinL <=1100) {
    colourL = "grey";
  }else if(pinL >1100)  {
    colourL = "black";
  }
}

void driveForward(double percentage) {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((255.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, speedR);
  analogWrite(motorPinLV, speedL);
  analogWrite(motorPinLA, 0);
}

/*
 * Stand still
 */

void standStill() {
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
}

/*
 * turn left
 */

void turnLeft(double percentage) {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((200.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, speedL);
  analogWrite(motorPinLA, 0);
}

/*
 * turn right
 */

void turnRight(double percentage) {
  int speedR = int((200.0f / 100.0f) * percentage);
  int speedL = int((255.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, speedR);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
}

/*
 * turn left 90
 */

void turnLeft90() {
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 170);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
  delay(500);
}

/*
 * turn right 90
 */

void turnRight90()  {
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 255);
  analogWrite(motorPinLA, 0);
  delay(500);
}

/*
 * drive backwards
 */

void driveBackwards(double percentage){
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((225.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, speedR);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, speedL);
}

/*
 * turn 180
 */

void turn180()  {
  analogWrite(motorPinRA, 255);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
  delay(1000);
}

/*
 * turn 90 backwards
 */
void turn90Backwards()  {
  analogWrite(motorPinRA, 255);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
  delay(500);
}
