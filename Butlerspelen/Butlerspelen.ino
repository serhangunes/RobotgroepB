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

float motorR = 255.0f;
float motorL = 255.0f;

bool canCheck = true;

void setup() {
  Serial.begin(115200);
  if (!lidar.begin()) {
    Serial.println(F("Failed to connect to VL53L0X"));
    while(1);
  }
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Failed to connect to SSD1306"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void loop() {
  // driveForward(100);
  // delay(2000);
  // standStill();
  // delay(2000);
  VL53L0X_RangingMeasurementData_t measure;
  lidar.rangingTest(&measure, false);

  //Afstand in cm
  float dist = measure.RangeMilliMeter/10.0f - 2.0f;
  
  if (measure.RangeStatus != 4) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("d: ");
    display.println(dist);
    display.display();
    if(dist <= 30.0f && canCheck == true){
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
  canCheck = false;
  VL53L0X_RangingMeasurementData_t measure;
  lidar.rangingTest(&measure, false);

  //Afstand in cm
  float dist = measure.RangeMilliMeter/10.0f - 2.0f;
  
  turnLeft(70);
  delay(400);
  standStill();
  delay(400);
  for (int i = 0; i < 8; i++) {
    turnRight(70);
    delay(100);
    VL53L0X_RangingMeasurementData_t measure;
    lidar.rangingTest(&measure, false);

    //Afstand in cm
    dist = measure.RangeMilliMeter/10.0f - 2.0f;
    standStill();
    if(dist > highestValue) {
      highestValue = dist;
    }    
    display.clearDisplay();
    display.setCursor(0,0);
    display.print(i);
    display.print(": ");
    display.println(dist);
    display.display();
    delay(500);
  }
    
  standStill();
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("h: ");
  display.println(highestValue);
  display.display();
  delay(2000);

  do {
    VL53L0X_RangingMeasurementData_t measure;
    lidar.rangingTest(&measure, false);

    //Afstand in cm
    dist = measure.RangeMilliMeter/10.0f - 2.0f;
    turnLeft(70);    
    canCheck = false;
  } while (dist >= highestValue - 5 && dist <= highestValue + 5);

  driveForward(80);
  canCheck = true;
}

void standStill() {
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, 0);
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, 0);
}

void driveForward(double percentage) {
  int speedR = int((motorR / 100.0f) * percentage);
  int speedL = int((motorL / 100.0f) * percentage);
  
  analogWrite(motorPinRA, speedR);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, speedL);
}

void turnLeft(double percentage) {
  int speedR = int((motorR / 100.0f) * percentage);
  int speedL = int((motorL / 100.0f) * percentage);
  
  analogWrite(motorPinRA, speedR);
  analogWrite(motorPinRV, 0);
  analogWrite(motorPinLV, speedL);
  analogWrite(motorPinLA, 0);
}

void turnRight(double percentage)  {
  int speedR = int((motorR / 100.0f) * percentage);
  int speedL = int((motorL / 100.0f) * percentage);
  
  analogWrite(motorPinRA, 0);
  analogWrite(motorPinRV, speedR);
  analogWrite(motorPinLV, 0);
  analogWrite(motorPinLA, speedL);
}
