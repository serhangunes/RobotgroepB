#include <Arduino.h>
#include <analogWrite.h>
#include <Adafruit_VL53L0X.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>  
#include "WiFi.h"

int motorPinLA = 16; //Rechterwiel achteruit
int motorPinLV = 17; //Rechterwiel vooruit
int motorPinRV = 5; //Linkerwiel vooruit
int motorPinRA = 18; //Linkerwiel achteruit
int IRPinR = 39;//
int IRPinL = 34;//
int pinR = 0;
int pinL = 0;

const char* ssid = "";
const char* password = "";

int colorThreshold = 1100;



Adafruit_SSD1306 display(128, 32, &Wire, 4);

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.display();
  
  Serial.begin(115200);
  pinMode(motorPinRA, OUTPUT);
  pinMode(motorPinRV, OUTPUT);
  pinMode(motorPinLV, OUTPUT);
  pinMode(motorPinLA, OUTPUT);

  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  //aflezen pin rechts
  pinL = analogRead(IRPinL);
  //Serial.print(pinL);
  pinR = analogRead(IRPinR);
  //Serial.println(pinR);


  display.clearDisplay();
  
  display.setCursor(0,0);             // Start at top-left corner
  display.print("PinL: ");
  display.println(analogRead(IRPinL));
  display.print("PinR: ");
  display.println(analogRead(IRPinR));
  display.print("Dist: ");
  display.println(measure.RangeMilliMeter);
  display.display();

  //Serial.print(" ");
  //aflezen pin links
  

  standStill();
  /*
   * lijn tussen sensoren
   */
  
//  // standStill();
//  if(measure.RangeMilliMeter > 300) {
//    if(pinR >= colorThreshold && pinL >= colorThreshold)
//    {
//      standStill();
//    }
//    else if(pinR < colorThreshold && pinL >= colorThreshold)
//    {
//      turnRight(100);
//    }
//    else if(pinR >= colorThreshold && pinL < colorThreshold)
//    {
//      turnLeft(100);
//    }
//    else if(pinR < colorThreshold && pinL <colorThreshold)
//    {
//      driveForward(100);
//    }
//  } else {
//    standStill();
//  }
}

/*
 * functions
 */

/*
 * Stand still
 */
void standStill() {
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
}

/*
 * Drive Forward
 */
void driveForward(double percentage) {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((225.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, speedR);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, speedL);
}

void driveForwardFor(double percentage, int duration) {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((225.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, speedR);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, speedL);
  delay(duration);
}
/*
 * Turn 90 degrees left
 */

void turnLeft(double percentage) {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((225.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, speedR);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
//  delay(340);
}

void turnRight(double percentage)  {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((225.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, speedL);
}

void turnRight90(){
  analogWrite(motorPinRA, 187);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 203);
  analogWrite(motorPinLA, 0);
  delay(350);
  standStill();
}

void turn180(){
  analogWrite(motorPinRA, 187);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 203);
  analogWrite(motorPinLA, 0);
  delay(553);
  standStill();
}

void driveBackwards(){
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 234);
  analogWrite(motorPinLV, 254);
  analogWrite(motorPinLA, 0);
}
