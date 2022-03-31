int motorPinLA = 16; //Linkerwiel achteruit
int motorPinLV = 17; //Linkerwiel vooruit
int motorPinRV = 5; //Rechterwiel vooruit
int motorPinRA = 18; //Rechterwiel achteruit
int IRPinR = 39;// IR pin rechts
int IRPinL = 34;// IR pin links
int pinR = 0;
int pinL = 0;
int colorThreshold = 1100;

void raceLoop() {
  //display
  display.clearDisplay();
  display.setCursor(0, 0);            // Start at top-left corner
  display.print("PinL: ");
  display.println(analogRead(IRPinL));
  display.print("PinR: ");
  display.println(analogRead(IRPinR));
  display.display();

  //Read the IR pins
  int pinL = analogRead(IRPinL) / colorThreshold;
  int pinR = analogRead(IRPinR) / colorThreshold;


  //if robot sees black with both, stand still
  if (pinL >= 1 && pinR >= 1) {
    standStill();
    delay(500);
    driveBackwards(100);
    delay(500);
    //if robot sees black on the right then drive back for a moment, turn left and drive forward
  } else if (pinL == 0 && pinR >= 1) {
    driveBackwards();
    turnLeft(80);
    //if robot sees black on the left then drive back for a moment, turn right and drive forward
  } else if (pinL >= 1 && pinR == 0) {
    driveBackwards();
    turnRight(80);
    //if the robot sees no black then drive forward
  } else if (pinL == 0 && pinR == 0) {
    driveForward(80);
  }
}
