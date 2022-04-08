//declaring the variables
String colourR = "";
String colourL = "";

//Declaring the colours
int whiteValL = 80;
int whiteValR = 80;
int greyValL = 200;
int greyValR = 200;

/*
   --------------------------------------------------------------
   -------------------------Functions----------------------------
   --------------------------------------------------------------
*/

/*
   Look if there is a road ahead.
*/
void lookFunction() {
  //turn left and drivebackward until they see black on both sides.
  turnLeft90Maze();
  driveBackwards(70);
  if (colourL == "black" && colourR == "black")  {
    //stand still and turn right with 90 degrees if both sides are black.
    standStill();
    delay(500);
    turnRight90Maze();
  } else {
    //drive forward if both sides are not black.
    driveForward(70);
  }
}

/*
  Read the pins and give a colour
*/
void readPins() {

  //if the sensor reads less than the white value it's white.
  if (IRValR <= whiteValR)  {
    colourR = "white";
    //if the sensor read more than the white value and less than the grey value it's grey.
  } else if (IRValR > whiteValR && IRValR <= greyValR)  {
    colourR = "grey";
    //if the sensor reads more than the grey value then it's black.
  } else if (IRValR > greyValR) {
    colourR = "black";
  }

  //if the sensor reads less than the white value it's white.
  if (IRValL <= whiteValL)  {
    colourL = "white";
    //if the sensor read more than the white value and less than the grey value it's grey.
  } else if (IRValL > whiteValL && IRValL <= greyValL) {
    colourL = "grey";
    //if the sensor reads more than the grey value then it's black.
  } else if (IRValL > greyValL)  {
    colourL = "black";
  }
}

/*
   --------------------------------------------------------------
   ---------------------------Maze-------------------------------
   --------------------------------------------------------------
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
  display.println(IRValR);
  display.print("PinL: ");
  display.println(IRValL);
  display.display();


  //If both are white then drive forward
  if (colourL == "white" && colourR == "white")
  {
    driveForward(70);
  }
  //if left is grey then adjust to the left.
  else if (colourL == "grey" && colourR == "white")
  {
    turnLeftMaze(70);
  }
  //if right is grey then adjust to the right.
  else if (colourL == "white" && colourR == "grey")
  {
    turnRightMaze(70);
  }
  //if right is black then turn 90 degrees to the right.
  else if (colourL == "white" && colourR == "black")
  {
    turnRight90Maze();
  }
  //if right is black then turn 90 degrees to the right.
  else if (colourL == "grey" && colourR == "black")
  {
    turnRight90Maze();
  }
  //if both are grey then turn 180 degrees.
  else if (colourL == "grey" && colourR == "grey")
  {
    turn180Maze(70);
  }
  //if left is black then turn 90 degrees to the right.
  else if (colourL == "black" && colourR == "white")
  {
    turnLeft90Maze();
  }
  //if left is black then turn 90 degrees to the right.
  else if (colourL == "black" && colourR == "grey")
  {
    turnLeft90Maze();
  }
  else if (colourL == "black" && colourR == "black") {
    turnRight90Maze();
  }
  //else just stand still.
  else
  {
    standStill();
  }
}
