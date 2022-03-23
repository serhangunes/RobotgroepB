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
int motorPinRA = 18;
int motorPinRV = 5;
int IRPinL = 39;
int IRPinR = 34;

//declaring the variables
int pinR;
int pinL;
int grey = 120;
int black = 1100;
int white = 75;

Adafruit_SSD1306 display(128, 32, &Wire, 4);

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

/*
 * --------------------------------------------------------------
 * -------------------------Setup--------------------------------
 * --------------------------------------------------------------
 */

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

/*
 * --------------------------------------------------------------
 * -------------------------Loop---------------------------------
 * --------------------------------------------------------------
 */
void loop() {
  // read the IR sensor
pinL = analogRead(IRPinL);
pinR = analogRead(IRPinR);

/*
 * display
 */
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
//clear the display
  display.clearDisplay();

//write the values form the IR sensor on the display
  display.setCursor(0,0);             // Start at top-left corner
  display.print("PinL: ");
  display.println(analogRead(IRPinL));
  display.print("PinR: ");
  display.println(analogRead(IRPinR));
  display.print("Dist: ");
  display.println(measure.RangeMilliMeter);
  display.display();

/*
 * maze
 */
 standStill();

//if(pinL < white && pinR < white){
//    driveForward(100);
//}else if(pinL > white && pinL < grey && pinR < white){
//    turnLeft(100);
//}else if(pinL < white && pinR > white && pinR < grey) {
//  turnRight(100);
//}else{
//  standStill();
//}
// if(pinL < white && pinR < white)  
// {
//    driveForward(70);
// }
// else if(pinL < white && pinR > white && pinR < black) 
// {
//    turnRight(70);
// }
// else if(pinL < white && pinR > black)  
// {
//    turnRight90();
//    standStill();
//    if(pinL > black && pinR > black)  
//    {
//        turn90Backwards();
//        driveForward(70);
//    }
//    else if(pinL < black && pinR > black)  
//    {
//        driveForward(70);
//    }
// }
// else if(pinL > white && pinL < black && pinR < white) 
// {
//    turnLeft(70);
// }
// else if(pinL > white && pinL < black && pinR > white && pinR < black) 
// {
//    turn180();
// }
// else if(pinL > white && pinL < black && pinR > black) 
// {
//    turnRight90();
//    standStill();
//    if(pinL > black && pinR > black)  
//    {
//      turn90Backwards();
//      driveForward(70);
//    }
//    else if(pinL < black && pinR > black)  
//    {
//      driveForward(70);
//    }
// }
// else if(pinL > black && pinR < white) 
// {
//    turnLeft90();
// }
// else if(pinL > black && pinR > white && pinR < black) 
// {
//    turnLeft90();
// }
// else if(pinL > black && pinR > black) 
// {
//    turnLeft90();
// }


 
}

/*
 * --------------------------------------------------------------
 * -------------------------Functions----------------------------
 * --------------------------------------------------------------
 */

 /*
  * Drive forward
  */

void driveForward(double percentage)  {
  int speedR = int((170.0f / 100.0f) * percentage);
  int speedL = int((170.0f / 100.0f) * percentage);
  
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
  int speedL = int((225.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 200);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
}

/*
 * turn right
 */

void turnRight(double percentage) {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((225.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 200);
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
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 170);
  delay(500);
}

/*
 * drive backwards
 */

void driveBackwards(double percentage){
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((225.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, speedR);
  analogWrite(motorPinLV, speedL);
  analogWrite(motorPinLA, 0);
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
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 255);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
  delay(500);
}
