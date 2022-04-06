//declaring the variables
String colourR = "";
String colourL = "";

int whiteVal = 80;
int grayVal = 400;

/*
   --------------------------------------------------------------
   -------------------------Functions----------------------------
   --------------------------------------------------------------
*/

/*
 * Look if there is a road ahead.
 */
void lookFunction() {
  turnLeft90Maze();
  driveBackwards(70);
  if(colourL == "black" && colourR == "black")  {
    standStill();
    delay(500);
    turnRight90Maze();
  }else{
    driveForward(70);
  }
}

/*
  Read the pins and give a colour
*/
void readPins() {

//if the sensor reads less than 80 it's white.
  if(IRValR <= whiteVal)  {
    colourR = "white";
//if the sensor read more than 80 and less than 250 it's grey.
  }else if(IRValR > whiteVal && IRValR <=grayVal)  {
    colourR = "grey";
//if the sensor reads more than 250 then it's black.
  }else if(IRValR > grayVal) {
    colourR = "black";
  }

//if the sensor reads less than 80 it's white.
  if(IRValL <= whiteVal)  {
    colourL = "white";
//if the sensor read more than 80 and less than 250 it's grey.
  }else if(IRValL >whiteVal && IRValL <=grayVal) {
    colourL = "grey";
//if the sensor reads more than 250 then it's black.
  }else if(IRValL >grayVal)  {
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
  display.println(IRValR);
  display.print("PinL: ");
  display.println(IRValL);
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
    turnLeftMaze(70);
  }
//if right is grey then adjust to the right.
  else if(colourL == "white" && colourR == "grey") 
  {
    turnRightMaze(70);
  }
//if right is black then turn 90 degrees to the right.
  else if(colourL == "white" && colourR == "black")
  {
    turnRight90Maze();
  }
//if right is black then turn 90 degrees to the right.
  else if(colourL == "grey" && colourR == "black")
  {
    turnRight90Maze();
  }
//if both are grey then turn 180 degrees.
  else if(colourL == "grey" && colourR == "grey")  
  {
    standStill();
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
    turnRight90Maze();
  }
//else just stand still.
  else
  {
    standStill();
  }
}
