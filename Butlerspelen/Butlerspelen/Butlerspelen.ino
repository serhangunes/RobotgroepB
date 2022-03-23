#include <Arduino.h>
#include <analogWrite.h>
#include <Adafruit_VL53L0X.h> //Afstandssensor
#include <Adafruit_GFX.h> //Graphics library
#include <Adafruit_SSD1306.h> //Schermpje

int motorPinLA = 16; //Rechterwiel achteruit
int motorPinLV = 17; //Rechterwiel vooruit
int motorPinRV = 5; //Linkerwiel vooruit
int motorPinRA = 18; //Linkerwiel achteruit

Adafruit_VL53L0X lidar = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);
  if (!lidar.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lidar.rangingTest(&measure, false);

  //Afstand in cm
  int dist = measure.RangeMilliMeter/10 - 2;
  
  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
//    if(dist <= 20){
//      Serial.print("Distance (mm): "); Serial.println(dist);
//      standStill();
//    } else {
//      driveForward(80);
//    }
      driveForward(100);
      delay(1000);
      standStill();
      delay(1000);
  }
}

void standStill() {
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
  delay(70);
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
}

void driveForward(double percentage) {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((225.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, speedR);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, speedL);
}
