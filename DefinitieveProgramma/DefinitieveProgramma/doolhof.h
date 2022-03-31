//declaring the variables
String colourR = "";
String colourL = "";
/*
   --------------------------------------------------------------
   -------------------------Functions----------------------------
   --------------------------------------------------------------
*/

/*
  Read the pins and give a colour
*/
void readPins() {
  pinL = analogRead(IRPinL);
  pinR = analogRead(IRPinR);

  if (pinR <= 90)  {
    colourR = "white";
  } else if (pinR > 80 && pinR <= 400)  {
    colourR = "grey";
  } else if (pinR > 400) {
    colourR = "black";
  }

  if (pinL <= 90)  {
    colourL = "white";
  } else if (pinL > 85 && pinL <= 400) {
    colourL = "grey";
  } else if (pinL > 400)  {
    colourL = "black";
  }
}


/* 
 *  turn left 90
 */
void turnRightAdvanced() {
    driveForward(70);
    delay(100);
    standStill();
    delay(500);
    turnRight(80);
    delay(600);
    standStill();
    delay(500);
}


/* 
 *  turn right 90
 */
void turnLeftAdvanced() {
    driveForward(70);
    delay(100);
    standStill();
    delay(500);
    turnLeft(80);
    delay(600);
    standStill();
    delay(500);
}

void turn180(double percentage)  {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((225.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, speedR);
  analogWrite(motorPinLV, 0);  
  analogWrite(motorPinLA, 0);
  delay(800);
}

/*
 * --------------------------------------------------------------
 * ---------------------------Loop-------------------------------
 * --------------------------------------------------------------
 */

void mazeLoop() {
  //detect the different tapes.
  readPins();

  /*
   * display
   */
  //clear the display
  display.clearDisplay();

  //write the values from the IR sensor on the display
  display.setCursor(0, 0);            // Start at top-left corner
  display.print("PinR: ");
  display.println(pinR);
  display.print("PinL: ");
  display.println(pinL);
  display.display();

  if (colourL == "white" && colourR == "white")
  {
    driveForward(70);
  }
  else if (colourL == "grey" && colourR == "white")
  {
    turnLeft(70);
  }
  else if (colourL == "white" && colourR == "grey")
  {
    turnRight(70);
  }
  else if (colourL == "white" && colourR == "black")
  {
    turnRightAdvanced();
  }
  else if (colourL == "grey" && colourR == "black")
  {
    turnRightAdvanced();
  }
  else if (colourL == "grey" && colourR == "grey")
  {
    standStill();
  }
  else if (colourL == "black" && colourR == "white")
  {
    turnLeftAdvanced();
  }
  else if (colourL == "black" && colourR == "grey")
  {
    turnLeftAdvanced();
  }
  else
  {
    standStill();
  }
}
