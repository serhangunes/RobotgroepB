#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>

WebSocketsClient webSocket;

const char* ssid = "Wifi is saai"; //Naam van het netwerk
const char* password =  "2much4u2day"; //Wachtwoord van het netwerk
const char* ipadress = "192.168.210.155";
const int port = 3003;

int currentLine = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");

  // server address, port and URL
  webSocket.begin(ipadress, port, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);

  // try ever 5000 again if connection has failed
  webSocket.setReconnectInterval(5000);
}
 
void loop() {
  webSocket.loop();
  webSocket.sendTXT("Robot van groep B is verbonden!");
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

  switch(type) {
    case WStype_CONNECTED:
      //Wanneer er verbonden is met de websocket:
      Serial.print("[WSc] Connected!\n");
      webSocket.sendTXT("Robot van groep B is verbonden!");
      break;
    case WStype_DISCONNECTED:
      //Wanneer de verbinding met de websocket weg is
      Serial.println("[WSc] Disconnected!\n");
      break;    
    case WStype_TEXT:
      //Wanneer je iets ontvangt
      Serial.printf("[WSc] get text: %s\n", payload);

      // send message to server
      // webSocket.sendTXT("message here");
      break;
    case WStype_BIN:
    case WStype_ERROR:      
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
  }

}
