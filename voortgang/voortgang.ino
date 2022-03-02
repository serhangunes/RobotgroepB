#include <Arduino.h>
#include <analogWrite.h>
#include <Adafruit_VL53L0X.h>

int motorPinRA = 16; //Rechterwiel achteruit
int motorPinRV = 17; //Rechterwiel vooruit
int motorPinLV = 5; //Linkerwiel vooruit
int motorPinLA = 18; //Linkerwiel achteruit
int IRPinR = 39;//
int IRPinL = 34;//
int pinR = 0;
int pinL = 0;

void setup() {
  Serial.begin(9600);
  pinMode(motorPinRA, OUTPUT);
  pinMode(motorPinRV, OUTPUT);
  pinMode(motorPinLV, OUTPUT);
  pinMode(motorPinLA, OUTPUT);
}

void loop(){

//aflezen pin rechts
pinL = analogRead(IRPinL)/2600;
//Serial.println(pinL);

//aflezen pin links
pinR = analogRead(IRPinR)/2600;
//Serial.println(pinR);

if(pinR == 1 && pinL ==1)
{
  driveForward();
}else if(pinR == 0 && pinL == 1)
{
  turnRight();
}
else if(pinR == 1 && pinL == 0)
{
  turnLeft();
}
else if(pinR == 0 && pinL == 0)
{
standStill();
delay(1000);
driveBackwards();
delay(500);
//turnRight();
//delay(250);


}
}

/*
 * functions
 */

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
 * Drive Forward
 */
void driveForward() {
  analogWrite(motorPinRA, 178);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 203);
}
/*
 * Turn 90 degrees left
 */

void turnLeft() {
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 175);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 192);
//  delay(340);
}

void turnRight()  {
  analogWrite(motorPinRA, 175);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 192);
  analogWrite(motorPinLA, 0);
}

void turnRight90(){
  analogWrite(motorPinRA, 187);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 203);
  analogWrite(motorPinLA, 0);
  delay(350);
  standStill();
}

void turn180(){
  analogWrite(motorPinRA, 187);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 203);
  analogWrite(motorPinLA, 0);
  delay(553);
  standStill();
}

void driveBackwards(){
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 234);
  analogWrite(motorPinLV, 254);
  analogWrite(motorPinLA, 0);
}
