bool canCheck = true;
int valCap = 50;

float measure() {
  VL53L0X_RangingMeasurementData_t measure;
  lidar.rangingTest(&measure, false);
  float dist = measure.RangeMilliMeter / 10.0f - 2.0f;
  return dist;
}

void lookForHole() {
  float dist = measure();
  canCheck = false;
  turnLeft(60);
  delay(400);

  do {
    dist = measure();
    turnRight(60);
  } while (dist < valCap);
  standStill();

  delay(500);

  driveForward(60);

  canCheck = true;
}

void butlerLoop() {
  VL53L0X_RangingMeasurementData_t measure;
  lidar.rangingTest(&measure, false);
  float dist = measure.RangeMilliMeter / 10.0f - 2.0f;

  if (measure.RangeStatus != 4) {
    writeToDisplay("d: " + String(dist), 0, 0);
    writeToDisplay("L: " + String(IRValL) + " R: " + String(IRValR), 0, 1);
    if (dist <= 30.0f && canCheck == true) {
      standStill();
      lookForHole();
    } else {
      driveForward(80);
    }
  }
//  if (IRValL >= 2000 && IRValR >= 2000) {
//    botStatus = "finished";
//  }
}
