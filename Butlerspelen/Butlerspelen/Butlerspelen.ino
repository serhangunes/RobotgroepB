#include <Arduino.h>
#include <analogWrite.h>
#include <Adafruit_VL53L0X.h> //Afstandssensor
#include <Adafruit_GFX.h> //Graphics library
#include <Adafruit_SSD1306.h> //Schermpje

int motorPinLA = 16; //Rechterwiel achteruit
int motorPinLV = 17; //Rechterwiel vooruit
int motorPinRV = 5; //Linkerwiel vooruit
int motorPinRA = 18; //Linkerwiel achteruit

Adafruit_SSD1306 display(128, 32, &Wire, 4);
Adafruit_VL53L0X lidar = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);
  if (!lidar.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
  lidar.rangingTest(&measure, false);

  //Afstand in cm
  float dist = measure.RangeMilliMeter/10.0f - 2.0f;
  
  if (measure.RangeStatus != 4) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("h: ");
    display.println(dist);
    display.display();
    if(dist <= 20.0f){
      //Serial.print("Distance (mm): "); Serial.println(dist);
      standStill();
      delay(1000);
      lookForHole();
    } else {
      driveForward(80);
    }
  }
}

float highestValue = 0;

void lookForHole() {
  VL53L0X_RangingMeasurementData_t measure;
  lidar.rangingTest(&measure, false);

  //Afstand in cm
  float dist = measure.RangeMilliMeter/10.0f - 2.0f;
  
  turnLeft(80);
  delay(400);
  standStill();
  delay(800);
  turnRight(80);
  if(highestValue < dist) highestValue = dist;
  delay(400);
  standStill();
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("h: ");
  display.println(highestValue);
  display.display();
  delay(2000);
}

void standStill() {
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
  delay(70);
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
}

void driveForward(double percentage) {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((225.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, speedR);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, speedL);
}

void turnLeft(double percentage) {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((225.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, speedR);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, speedL);
  analogWrite(motorPinLA, 0);
}

void turnRight(double percentage)  {
  int speedR = int((255.0f / 100.0f) * percentage);
  int speedL = int((225.0f / 100.0f) * percentage);
  
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, speedR);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, speedL);
}
