#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <analogWrite.h>

#include <Adafruit_VL53L0X.h>  //Afstandssensor
#include <Adafruit_GFX.h>      //Graphics library
#include <Adafruit_SSD1306.h>  //Schermpje

Adafruit_SSD1306 display(128, 32, &Wire, 4);
Adafruit_VL53L0X lidar = Adafruit_VL53L0X();

#include "movement.h"
#include "butlerspelen.h"

WebSocketsClient webSocket;

const char* ssid = "Hotspot van Yannieck";        //Naam van het netwerk
const char* password = "vmzm9931";                //Wachtwoord van het netwerk
const char* ipadress = "battlebot1.serverict.nl";
const int port = 33003;

String macAdress = WiFi.macAddress();

String currentGame = "idle";

String status = "ready";
bool isDriving;
int acceleration;

unsigned long previousMillis = 0;
const long interval = 5000;

void setup() {
  butlerInit();

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
    webSocket.sendTXT("{\"status\": \"" + status + "\",\"isDriving\": " + isDriving + ",\"acceleration\":" + acceleration + "}");
  }
  if(status == "in_game") {
    if(currentGame == "butler") {    
      //butlerLoop();
      driveForward(100);
    }
  }
  if(status == "finished") {
    standStill();
  }

  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.print("St: ");
  display.println(status);
  display.display();
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  //status = "connecting to ws";
  //Maak een nieuw JSON bestand aan en sla de verkregen informatie er in op, haal daarna de waarden uit de JSON en stop ze in variabelen
  
  const size_t capacity = JSON_OBJECT_SIZE(3);
  DynamicJsonDocument doc(capacity);
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
      currentGame = "idle";
      status = "ready";
      break;
    case WStype_TEXT:
      //Wanneer er tekst is ontvangen:
      //Als er succesvol is ingelogd, stuur een bericht naar de serial monitor
      Serial.println(test);
      if (loggedin) {
        Serial.println("[WS] Robot has logged in to the websocket");
      }
      //Als er een action is ontvangen, kijk wat die action is en print het naar de serial monitor
      if (action != "null") {
        //Serial.println("[WS] Recieved info from websocket:");
        if (action == "prepare") {
          if (currentGame == "idle") {
            Serial.print("[SERVER] prepare game: ");
            Serial.println(game);
            status = "preparing_game";

            //Stuur naar de websocket wanneer de arduino klaar is om het spel te starten
            webSocket.sendTXT("{\"status\": true,\"game\": \"" + game + "\"}");
            status = "ready";
          }
        } else if (action == "start") {
          Serial.print("[SERVER] start game: ");
          Serial.println(game);
          status = "in_game";
            currentGame = game;
        } else if (action == "ended") {
          Serial.print("[SERVER] ended game: ");
          Serial.println(game);
          status = "finished";
          currentGame = "idle";
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
