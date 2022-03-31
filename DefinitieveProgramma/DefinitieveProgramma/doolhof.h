//declaring the variables
String colourR = "";
String colourL = "";

//millis variables
unsigned long timerMillis = 0;
unsigned long currentTime = millis();
String gameAction = "";
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
   turn left 90
*/


void turn90RightAdvanced() {
  currentTime = millis();
  if (timerMillis == 0)  {
    timerMillis = millis();
  }
  if (currentTime - timerMillis <= 100) {
    driveForward();
  } else if (currentTime - timerMillis <= 600)  {
    standStill();
  } else if (currentTime - timerMillis <= 1200) {
    turnRightMaze();
  } else if (currentTime - timerMillis <= 1700) {
    standStill();
  } else {
    timerMillis = 0;
    gameAction = "";
  }
}

void turn90LeftAdvanced() {
  currentTime = millis();
  if (timerMillis == 0)  {
    timerMillis = millis();
  }
  if (currentTime - timerMillis <= 100) {
    driveForward();
  } else if (currentTime - timerMillis <= 600)  {
    standStill();
  } else if (currentTime - timerMillis <= 1200) {
    turnLeftMaze();
  } else if (currentTime - timerMillis <= 1700) {
    standStill();
  } else {
    timerMillis = 0;
    gameAction = "";
  }
}

void turn180()  {
  currentTime = millis();
  if (timerMillis == 0)  {
    timerMillis = millis();
  }
  if (currentTime - timerMillis <= 100) {
    driveForward();
  } else if (currentTime - timerMillis <= 600)  {
    standStill();
  } else if (currentTime - timerMillis <= 1200) {
    turnLeftMaze();
  } else if (currentTime - timerMillis <= 1700) {
    standStill();
  } else {
    timerMillis = 0;
    gameAction = "";
  }
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
     display
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

  if (gameAction == "turnLeft")
  {
    turnLeftAdvanced();
    return;
  } else if (gameAction == "turnRight") {
    turnRightAdvanced();
    return;
  }

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
    gameAction = "turnRight";
  }
  else if (colourL == "grey" && colourR == "black")
  {
    gameAction = "turnRight";
  }
  else if (colourL == "grey" && colourR == "grey")
  {
    standStill();
  }
  else if (colourL == "black" && colourR == "white")
  {
    gameAction = "turnLeft";
  }
  else if (colourL == "black" && colourR == "grey")
  {
    gameAction = "turnLeft";
  }
  else
  {
    standStill();
  }
}
