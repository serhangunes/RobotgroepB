#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WebSocketsClient.h>

WiFiMulti WiFiMulti;
WebSocketsClient webSocket;
#define USE_SERIAL Serial1

const char* ssid = "Hotspot van Yannieck"; //Naam van het netwerk
const char* password =  "vmzm9931"; //Wachtwoord van het netwerk
const char* ipadress = "192.168.0.123";
const int port = 81;
 
void setup() {
 
  Serial.begin(115200);
  // USE_SERIAL.begin(921600);
  //USE_SERIAL.begin(115200);

  //Serial.setDebugOutput(true);
  //USE_SERIAL.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for(uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFiMulti.addAP(ssid, password);

  //WiFi.disconnect();
  while(WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  // server address, port and URL
  webSocket.begin(ipadress, port, "/");
  
  webSocket.setReconnectInterval(5000);
}
 
void loop() {
  webSocket.loop();
}
