bool canCheck = true;
int valCap = 50;

//Function for measuring the distance from the LiDAR sensor
float measure() {
  VL53L0X_RangingMeasurementData_t measure;
  lidar.rangingTest(&measure, false);
  float dist = measure.RangeMilliMeter / 10.0f - 2.0f;
  return dist;
}

//Execute this function when the robot needs to look for the hole in the wall
void lookForHole() {
  float dist = measure();
  canCheck = false;
  
  //Turn a bit to the left, then start turning right.
  //When the sensor gets above a certain threshold, move forwards.
  turnLeft(70);
  delay(400);

  do {
    dist = measure();
    turnRight(70);
    delay(50);
  } while (dist < valCap);
  standStill();

  delay(500);

  driveForward(60);

  canCheck = true;
}

//This is the loop for the butlergame
void butlerLoop() {
  VL53L0X_RangingMeasurementData_t measure;
  lidar.rangingTest(&measure, false);
  float dist = measure.RangeMilliMeter / 10.0f - 2.0f;

  if (measure.RangeStatus != 4) {
    writeToDisplay("d: " + String(dist), 0, 0);
    writeToDisplay("L: " + String(IRValL) + " R: " + String(IRValR), 0, 1);
    
    //When the distance is under a certain threshold, start looking for the hole in the wall
    if (dist <= 30.0f && canCheck == true) {
      standStill();
      lookForHole();
    } else {
      driveForward(80);
    }
  }
  //When the bot sees tape op the ground, finish the game
  if (IRValL >= 2000 && IRValR >= 2000) {
    botStatus = "finished";
  }
}
