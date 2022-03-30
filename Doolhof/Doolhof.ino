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
int IRPinR = 39;
int IRPinL = 34;

//declaring the variables
int pinR;
int pinL;
String colourR = "";
String colourL = "";

Adafruit_SSD1306 display(128, 64, &Wire, 4);

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
detectColours();

/*
 * display
 */
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
//clear the display
  display.clearDisplay();

//write the values from the IR sensor on the display
  display.setCursor(0,0);             // Start at top-left corner
  display.print("PinL: ");
  display.println(colourL);
  display.print("PinR: ");
  display.println(colourR);
  display.print("Dist: ");
  display.println(measure.RangeMilliMeter);
  display.display();

/*
 * maze
 */
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);

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
 * Detect colours
 */
void detectColours()  {
  if(pinR <= 80)  {
    colourR = "white";
  }else if(pinR > 80 && pinR <=1100)  {
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

 /*
  * Drive forward
  */

void driveForward(double percentage)  {
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

void turnLeft90() {
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, speedL);
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
