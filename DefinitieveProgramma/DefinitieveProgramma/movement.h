#include <analogWrite.h>

int motorPinLA = 16;  //Rechterwiel achteruit
int motorPinLV = 17;  //Rechterwiel vooruit
int motorPinRV = 5;   //Linkerwiel vooruit
int motorPinRA = 18;  //Linkerwiel achteruit

float motorR = 255.0f;
float motorL = 255.0f;

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

  analogWrite(motorPinRA, speedR);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, speedL);
}

void turnLeft(double percentage) {
  int speedR = int((motorR / 100.0f) * percentage);
  int speedL = int((motorL / 100.0f) * percentage);

  analogWrite(motorPinRA, speedR);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, speedL);
  analogWrite(motorPinLA, 0);
}

void turnRight(double percentage) {
  int speedR = int((motorR / 100.0f) * percentage);
  int speedL = int((motorL / 100.0f) * percentage);

  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, speedR);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, speedL);
}