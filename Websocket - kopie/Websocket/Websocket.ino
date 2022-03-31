#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <analogWrite.h>

WebSocketsClient webSocket;

const char* ssid = "Hotspot van Yannieck"; //De naam van het netwerk
const char* password = "vmzm9931"; //Het wachtwoord van het netwerk
const char* ipadress = "battlebot1.serverict.nl"; //Het ip adres van de server
const int port = 33003; //De poort waar de websocket op draait

String macAdress = WiFi.macAddress();

unsigned long previousMillis = 0; //Te tijd sinds de laatst gemeten tijd
const long interval = 5000; //De tijd die de loop moet wachten
String status = "ready"; //Variable voor de status van de robot
bool isDriving = false; //De rijstatus van de robot
int acceleration = 0; //De acceleratie van de robot

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  int atempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("[WIFI] Connecting to WiFi..");
    atempts++;

    if (atempts >= 10) {
      Serial.println("[WIFI] Restarting device");
      exit(0);
    }
    //status = "connecting to wifi";
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
////  delay(4000);
//unsigned long currentMillis = millis();
//
//  if (currentMillis - previousMillis >= interval) {
//    //Sla de laatste keer dat je dit heb uitgevoerd op
//    previousMillis = currentMillis;
//
//    //Voer dit elke 5 seconden uit
//    webSocket.sendTXT("{\"status\": \"" + status + "\",\"isDriving\": " + isDriving + ",\"acceleration\":" + acceleration + "}");
//  }
    delay(800);
    webSocket.sendTXT("{\"status\": \"" + status + "\",\"isDriving\": " + isDriving + ",\"acceleration\":" + acceleration + "}");
 
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  //status = "connecting to ws";
  //Maak een nieuw JSON bestand aan en sla de verkregen informatie er in op, haal daarna de waarden uit de JSON en stop ze in variabelen
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload);

  bool loggedin = doc["loggedin"];
  String action = doc["action"];
  String game = doc["game"];
  
  char test[100];
  serializeJson(doc, test);

  switch (type) {
    case WStype_CONNECTED:
      //Wanneer er verbonden is met de websocket:
      Serial.println("[WS] Connected to the websocket");
      //Vraag de server om in te loggen
      webSocket.sendTXT("{\"action\": \"login\",\"id\": \"" + macAdress + "\"}");
      //status = "connected";
      break;
    case WStype_DISCONNECTED:
      //Wanneer de verbinding met de websocket is verbroken:
      Serial.println("[WS] Disconnected from the websocket");
      break;
    case WStype_TEXT:
        Serial.println(test);
//      //Wanneer er tekst is ontvangen:
//      //Als er succesvol is ingelogd, stuur een bericht naar de serial monitor
      if (loggedin) {
        Serial.println("[WS] Robot has logged in to the server");
      }
//      //Als er een action is ontvangen, kijk wat die action is en print het naar de serial monitor
//      if (action != "null") {
//        Serial.println("[WS] Recieved info from websocket:");
//        if (action == "prepare") {
//          if (!isInGame) {
//            Serial.print("[SERVER] prepare game: ");
//            Serial.println(game);
//            status = "preparing_game";
//            //Stuur naar de websocket wanneer de arduino klaar is om het spel te starten
//            webSocket.sendTXT("{\"status\": true,\"game\": \"" + game + "\"}");
//            status = "ready";
//            isInGame = true;
//          }
//        } else if (action == "start") {
//          Serial.print("[SERVER] start game: ");
//          Serial.println(game);
//          status = "in_game";
//        } else if (action == "ended") {
//          Serial.print("[SERVER] ended game: ");
//          Serial.println(game);
//          status = "finished";
//          isInGame = false;
//          delay(5000);
//          status = "ready";
//        }
//      }
      break;
    case WStype_PING:
      Serial.println("ping");
      break;
    case WStype_PONG:
      Serial.println("pong");
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
