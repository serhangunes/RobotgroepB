//declaring the variables
String colourR = "";
String colourL = "";
/*
   --------------------------------------------------------------
   -------------------------Functions----------------------------
   --------------------------------------------------------------
*/

/*
 * Look if there is a road ahead.
 */
void lookFunction() {
  turnRight90Maze();
  driveBackwards(70);
  if(colourL == "black" && colourR == "black")  {
    standStill();
    delay(500);
    turnLeft90Maze();
  }else{
    driveForward(70);
  }
}

/*
  Read the pins and give a colour
*/
void readPins() {
pinL = analogRead(IRPinL);
pinR = analogRead(IRPinR);

//if the sensor reads less than 80 it's white.
  if(pinR <= 80)  {
    colourR = "white";
//if the sensor read more than 80 and less than 250 it's grey.
  }else if(pinR > 80 && pinR <=250)  {
    colourR = "grey";
//if the sensor reads more than 250 then it's black.
  }else if(pinR > 250) {
    colourR = "black";
  }

//if the sensor reads less than 80 it's white.
  if(pinL <= 85)  {
    colourL = "white";
//if the sensor read more than 80 and less than 250 it's grey.
  }else if(pinL >85 && pinL <=250) {
    colourL = "grey";
//if the sensor reads more than 250 then it's black.
  }else if(pinL >250)  {
    colourL = "black";
  }
}

/*
 * --------------------------------------------------------------
 * ---------------------------Maze-------------------------------
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

//this works for following the line of the maze.
//If both are white then drive forward
  if(colourL == "white" && colourR == "white")  
  {
    driveForward(70);
  }
//if left is grey then adjust to the left.
  else if(colourL == "grey" && colourR == "white") 
  {
    turnLeft(70);
  }
//if right is grey then adjust to the right.
  else if(colourL == "white" && colourR == "grey") 
  {
    turnRight(70);
  }
//if right is black then turn 90 degrees to the right.
  else if(colourL == "white" && colourR == "black")
  {
    lookFunction();
  }
//if right is black then turn 90 degrees to the right.
  else if(colourL == "grey" && colourR == "black")
  {
    lookFunction();
  }
//if both are grey then turn 180 degrees.
  else if(colourL == "grey" && colourR == "grey")  
  {
    turn180Maze(90);
  }
//if left is black then turn 90 degrees to the right.
  else if(colourL == "black" && colourR == "white")
  {
    turnLeft90Maze();
  }
//if left is black then turn 90 degrees to the right.
  else if(colourL == "black" && colourR == "grey")
  {
    turnLeft90Maze();   
  }
  else if(colourL == "black" && colourR == "black") {
    turnLeft90Maze();
  }
//else just stand still.
  else
  {
    standStill();
  }
}
