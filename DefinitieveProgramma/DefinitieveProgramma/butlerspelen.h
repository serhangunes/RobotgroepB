#include <analogWrite.h>

#include "movement.h"

bool canCheck = true;

void butlerInit() {
  motorInit();
  if (!lidar.begin()) {
    Serial.println(F("Failed to connect to VL53L0X"));
    while(1);
  }
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Failed to connect to SSD1306"));
    for(;;); // Don't proceed, loop forever
  }
}

void butlerLoop() {
  VL53L0X_RangingMeasurementData_t measure;
  lidar.rangingTest(&measure, false);

  //Afstand in cm
  float dist = measure.RangeMilliMeter / 10.0f - 2.0f;

  if (measure.RangeStatus != 4) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("d: ");
    display.println(dist);
    display.display();
    if (dist <= 30.0f && canCheck == true) {
      standStill();
      delay(1000);
      lookForHole();
    } else {
      driveForward(80);
    }
  }
}

float highestValue = 0;

void lookForHole() {
  canCheck = false;
  VL53L0X_RangingMeasurementData_t measure;
  lidar.rangingTest(&measure, false);

  //Afstand in cm
  float dist = measure.RangeMilliMeter / 10.0f - 2.0f;

  turnLeft(70);
  delay(400);
  standStill();
  delay(400);
  for (int i = 0; i < 8; i++) {
    turnRight(70);
    delay(100);
    VL53L0X_RangingMeasurementData_t measure;
    lidar.rangingTest(&measure, false);

    //Afstand in cm
    dist = measure.RangeMilliMeter / 10.0f - 2.0f;
    standStill();
    if (dist > highestValue) {
      highestValue = dist;
    }
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(i);
    display.print(": ");
    display.println(dist);
    display.display();
    delay(500);
  }

  standStill();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("h: ");
  display.println(highestValue);
  display.display();
  delay(2000);

  do {
    VL53L0X_RangingMeasurementData_t measure;
    lidar.rangingTest(&measure, false);

    //Afstand in cm
    dist = measure.RangeMilliMeter / 10.0f - 2.0f;
    turnLeft(70);
    canCheck = false;
  } while (dist >= highestValue - 5 && dist <= highestValue + 5);

  driveForward(80);
  canCheck = true;
}