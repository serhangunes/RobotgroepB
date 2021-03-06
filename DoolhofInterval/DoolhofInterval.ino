//include libraries that we need.
#include <Arduino.h>
#include <analogWrite.h>
#include <Adafruit_VL53L0X.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>  
#include "WiFi.h"

//millis variables
unsigned long timerMillis = 0;
unsigned long currentTime = millis();
String gameAction = "";

//declaring the pins
int motorPinLV = 16;
int motorPinLA = 17;
int motorPinRV = 18;
int motorPinRA = 5;
int IRPinR = 34;
int IRPinL = 39;

//declaring the variables
int pinR;
int pinL;
String colourR = "";
String colourL = "";

//declaring the screen.
Adafruit_SSD1306 display(128, 64, &Wire, 4);\


//declaring interval
unsigned long time_1 = 0;
unsigned long time_2 = 0;
unsigned long time_3 = 0;
unsigned long time_4 = 0;

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
  //detect the different tapes.
  readPins();

/*
 * display
 */
//clear the display
  display.clearDisplay();

//write the values from the IR sensor on the display
  display.setCursor(0,0);             // Start at top-left corner
  display.print("PinR: ");
  display.println(pinR);
  display.print("PinL: ");
  display.println(pinL);
  display.display();


//turnLeft90(80);
//standStill();
//delay(1000);

////If both are white then drive forward
//if(colourL == "white" && colourR == "white")
//{
//  driveForward(80);
//}
////if right is grey then adjust to the right.
//else if(colourL == "white" && colourR == "grey")
//{
//  turnRight(80);
//}
////if right is black then turn 90 degrees right and if there is a road ahead.
//else if(colourL == "white" && colourR == "black")
//{
//  turnRight90(80);
//}
////if left is grey then adjust to the left.
//else if(colourL == "grey" && colourR == "white")
//{
//  turnLeft(80);
//}
////if both are grey then turn 180 degrees.
//else if(colourL == "grey" && colourR == "grey")
//{
//  standStill();
//}
////if right is black then turn right and check if there is a road ahead.
//else if(colourL == "grey" && colourR == "black")
//{
//  turnRight90(80);
//}
////If left is black then turn 90 degrees to the left.
//if(colourL == "black" && colourR == "white")
//{
//  turnLeft90(80);
//}
////if left is black then turn 90 degrees to the left.
//if(colourL == "black" && colourR == "grey")
//{
//  turnLeft90(80);
//}
////if both are black then turn 90 degrees to the left.
//else if(colourL == "black" && colourR == "black")
//{
//  turnLeft90(80);
//}
//else 
//{
//  standStill();
//}


  if(colourL == "white" && colourR == "white")  
  {
    driveForward(70);
  }
  else if(colourL == "grey" && colourR == "white") 
  {
    turnLeft(70);
  }
  else if(colourL == "white" && colourR == "grey") 
  {
    turnRight(70);
  }
  else if(colourL == "white" && colourR == "black")
  {
    turn90RightAdvanced();
  }
  else if(colourL == "grey" && colourR == "black")
  {
    turn90RightAdvanced();
  }
  else if(colourL == "grey" && colourR == "grey")  
  {
    standStill();
  }
  else if(colourL == "black" && colourR == "white")
  {
    turn90LeftAdvanced();
  }
  else if(colourL == "black" && colourR == "grey")
  {
  turn90LeftAdvanced();  
  }
  else
  {
    standStill();
  }
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

  if(pinR <= 90)  {
    colourR = "white";
  }else if(pinR > 80 && pinR <=400)  {
    colourR = "grey";
  }else if(pinR > 400) {
    colourR = "black";
  }

  if(pinL <= 90)  {
    colourL = "white";
  }else if(pinL >85 && pinL <=400) {
    colourL = "grey";
  }else if(pinL >400)  {
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
  int speedL = int((255.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, speedR);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
}

/*
 * turn right
 */

void turnRight(double percentage) {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((255.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, speedL);
  analogWrite(motorPinLA, 0);
}

/*
 * turn left 90
 */

void turnLeft90(double percentage) {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((255.0f / 100.0f) * percentage);
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, speedR);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
  delay(600);
  standStill();
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

void turn90RightAdvanced() {
  if(millis() < time_1 + INTERVAL_MESSAGE1)
  {
    driveForward(70);
  }
  if(millis() >= time_1 + INTERVAL_MESSAGE1 && millis() < time_2 + INTERVAL_MESSAGE2)
  {
    standStill();
  }
  if(millis() >= time_2 + INTERVAL_MESSAGE2 && millis() < time_3 + INTERVAL_MESSAGE3)
  {
    turnRight(70);
  }
  if(millis() >= time_3 + INTERVAL_MESSAGE3 && millis() < time_4 + INTERVAL_MESSAGE4)
  {
    standStill();
  }
}

void turn90LeftAdvanced() {
  if(millis() < time_1 + INTERVAL_MESSAGE1)
  {
    driveForward(70);
  }
  if(millis() >= time_1 + INTERVAL_MESSAGE1 && millis() < time_2 + INTERVAL_MESSAGE2)
  {
    standStill();
  }
  if(millis() >= time_2 + INTERVAL_MESSAGE2 && millis() < time_3 + INTERVAL_MESSAGE3)
  {
    turnLeft(70);
  }
  if(millis() >= time_3 + INTERVAL_MESSAGE3 && millis() < time_4 + INTERVAL_MESSAGE4)
  {
    standStill();
  }
}
