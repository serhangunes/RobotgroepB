#include <Arduino.h>
#include <analogWrite.h>
#include <Adafruit_VL53L0X.h>

int motorPinRA = 16; //Rechterwiel achteruit
int motorPinRV = 17; //Rechterwiel vooruit
int motorPinLV = 5; //Linkerwiel vooruit
int motorPinLA = 18; //Linkerwiel achteruit
//int analogPin1 = 39;//
//int analogPin2 = 34;//
int pin1 = 0;

void setup() {
  Serial.begin(9600);
  pinMode(motorPinRA, OUTPUT);
  pinMode(motorPinRV, OUTPUT);
  pinMode(motorPinLV, OUTPUT);
  pinMode(motorPinLA, OUTPUT);
}

void loop(){

//aflezen
pin1 = analogRead(12);
Serial.println(pin1);

//
standStill();
  

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
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 233);
  analogWrite(motorPinLV, 254);
  analogWrite(motorPinLA, 0);
}

void turnLeft() {
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 233);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
}
