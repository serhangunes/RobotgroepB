#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

WebSocketsClient webSocket;

const char* ssid = "Wifi is saai"; //Naam van het netwerk
const char* password =  "2much4u2day"; //Wachtwoord van het netwerk
const char* ipadress = "192.168.210.155";
const int port = 3003;
String macAdress = WiFi.macAddress();
String status;
String currentGame;

unsigned long previousMillis = 0;
const long interval = 5000;  

int currentLine = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("[WIFI] Connecting to WiFi..");
  }

  Serial.println("[WIFI] Connected to the WiFi network");

  //Start de websocket met het ip, de poort en de URL
  webSocket.begin(ipadress, port, "/");

  //Voer de event functie uit wanneer er iets gebeurt met de websocket
  webSocket.onEvent(webSocketEvent);

  //Als ze websocket verbinding faalt, probeer met over 5s weer
  webSocket.setReconnectInterval(5000);
}
 
void loop() {
  //Houd de websocket verbinding gaande
  webSocket.loop();
  
  //Stuur elke 5s de status van de robot
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    //Sla de laatste keer dat je dit heb uitgevoerd op
    previousMillis = currentMillis;

    //Voer dit elke 5 seconden uit
    webSocket.sendTXT("{\"status\": "+status+",\"game\": \""+currentGame+"\"}");
  }
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  //Maak een nieuw JSON bestand aan en sla de verkregen informatie er in op, haal daarna de waarden uit de JSON en stop ze in variabelen
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload);

  bool loggedin = doc["loggedin"];
  String action = doc["action"];
  String game = doc["game"];
  currentGame = game;

  switch(type) {
    case WStype_CONNECTED:
      //Wanneer er verbonden is met de websocket:
      Serial.println("[WS] Connected to the websocket");
      //Vraag de server om in te loggen
      webSocket.sendTXT("{\"action\": \"login\",\"id\": \""+macAdress+"\"}");
      break;
    case WStype_DISCONNECTED:
      //Wanneer de verbinding met de websocket is verbroken:
      Serial.println("[WS] Disconnected from the websocket");
      break;    
    case WStype_TEXT:
      //Wanneer er tekst is ontvangen:
      //Als er succesvol is ingelogd, stuur een bericht naar de serial monitor
      if(loggedin) {
        Serial.println("[WS] Device has logged in to the server");
      }
      //Als er een action is ontvangen, kijk wat die action is en print het naar de serial monitor
      if(action != "null") {
        Serial.println("[WS] Recieved info from websocket:");
        if(action == "prepare") {
          Serial.print("prepare game: ");
          Serial.println(game);
          status = "prepare";
          //Stuur naar de websocket wanneer de arduino klaar is om het spel te starten
          webSocket.sendTXT("{\"status\": true,\"game\": \""+game+"\"}");
          status = "ready";
        } else if (action == "start") {
          Serial.print("start game: ");
          Serial.println(game);
        } else if (action == "ended") {
          Serial.print("ended game: ");
          Serial.println(game);
        }
      }
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
