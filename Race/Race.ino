#include <Arduino.h>
#include <analogWrite.h>
#include <Adafruit_VL53L0X.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_VL53L0X.h"

int motorPinLA = 16; //Linkerwiel achteruit
int motorPinLV = 17; //Linkerwiel vooruit
int motorPinRV = 5; //Rechterwiel vooruit
int motorPinRA = 18; //Rechterwiel achteruit
int IRPinR = 39;// IR pin rechts
int IRPinL = 34;// IR pin links
int pinR = 0;
int pinL = 0;
int colorThreshold = 1100;

Adafruit_SSD1306 display(128, 32, &Wire, 4);

void setup(){
  Serial.begin(9600);
  pinMode(motorPinRA, OUTPUT);
  pinMode(motorPinRV, OUTPUT);
  pinMode(motorPinLV, OUTPUT);
  pinMode(motorPinLA, OUTPUT);

  //display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void loop(){
//display
  display.clearDisplay();
  
  display.setCursor(0,0);             // Start at top-left corner
  display.print("PinL: ");
  display.println(analogRead(IRPinL));
  display.print("PinR: ");
  display.println(analogRead(IRPinR));
  display.display();
  
  //Read the IR pins
  int pinL = analogRead(IRPinL)/1100;
  int pinR = analogRead(IRPinR)/1100;

standStill();
  if robot sees black with both, stand still
  if(pinL >= 1 && pinR >= 1){
    standStill();
    delay(500);
    driveBackwards();
    delay(500);
    //if robot sees black on the right then drive left
  }else if(pinL == 0 && pinR >= 1){
    turnLeft(80);
    //if robot sees black on the left then drive right
  }else if(pinL >= 1 && pinR == 0){
    turnRight(80);
    //if the robot sees no black then drive forward
  }else if(pinL == 0 && pinR == 0){
    driveForward(80);
  }
}


/*
 * functions
 */

 /*
  * Drive Forward
  */
void driveForward(double percentage) {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((225.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, speedR);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, speedL);
}


/*
 * turn left
 */
void turnLeft(double percentage) {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((225.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, speedR);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
//  delay(340);
}


/*
 * turn right
 */
void turnRight(double percentage)  {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((225.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, speedL);
}


/*
 * stand still
 */
void standStill() {
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
}

/*
 * drive backwards
 */
void driveBackwards(){
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
}
