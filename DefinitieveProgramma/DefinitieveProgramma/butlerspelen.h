bool canCheck = true;

float measure() {
  VL53L0X_RangingMeasurementData_t measure;
  lidar.rangingTest(&measure, false);
  float dist = measure.RangeMilliMeter / 10.0f - 2.0f;

  return dist;
}

void lookForHole() {
  float highestValue = 0;
  canCheck = false;

  turnLeft(70);
  delay(400);
  standStill();
  delay(400);
  for (int i = 0; i < 8; i++) {
    turnRight(70);
    delay(100);

    float dist = measure();
    standStill();
    if (dist > highestValue) {
      highestValue = dist;
    }
    display.clearDisplay();
    display.setCursor(0, 1);
    display.print(i);
    display.print(": ");
    display.println(dist);
    display.display();
    delay(500);
  }

  standStill();
  display.clearDisplay();
  display.setCursor(0, 1);
  display.print("h: ");
  display.println(highestValue);
  display.display();
  delay(2000);

  float dist = 0;
  do {
    dist = measure();
    turnLeft(70);
    canCheck = false;
  } while (dist >= highestValue - 5 && dist <= highestValue + 5);

  driveForward(80);
  canCheck = true;
}

void butlerLoop() {
  VL53L0X_RangingMeasurementData_t measure;
  lidar.rangingTest(&measure, false);

  //Afstand in cm
  float dist = measure.RangeMilliMeter / 10.0f - 2.0f;

  if (measure.RangeStatus != 4) {
    display.clearDisplay();
    display.setCursor(0, 1);
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
