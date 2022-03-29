#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <analogWrite.h>

Adafruit_SSD1306 display(128, 32, &Wire, 4);

WebSocketsClient webSocket;

const int port = 3003;
String macAdress = WiFi.macAddress();
String currentGame;

bool isInGame = false;
bool isDone = false;

String status = "ready";
bool isDriving;
int acceleration;

unsigned long previousMillis = 0;
const long interval = 5000;

int motorPinLA = 16;  //Rechterwiel achteruit
int motorPinLV = 17;  //Rechterwiel vooruit
int motorPinRV = 5;   //Linkerwiel vooruit
int motorPinRA = 18;  //Linkerwiel achteruit

void setup() {
  pinMode(motorPinRA, OUTPUT);
  pinMode(motorPinRV, OUTPUT);
  pinMode(motorPinLV, OUTPUT);
  pinMode(motorPinLA, OUTPUT);

  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

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

  //Stuur elke 5s de status van de robot
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    //Sla de laatste keer dat je dit heb uitgevoerd op
    previousMillis = currentMillis;

    //Voer dit elke 5 seconden uit
    webSocket.sendTXT("{\"status\": \"" + status + "\",\"isDriving\": " + isDriving + ",\"acceleration\":" + acceleration + "}");
  }
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(macAdress);
  display.println(status);
  display.display();

  if (status == "finished") {
    isDone = true;
    analogWrite(motorPinRA, 0);
    analogWrite(motorPinRV, 0);
    analogWrite(motorPinLV, 0);
    analogWrite(motorPinLA, 0);
  } else if (status == "in_game") {
    analogWrite(motorPinRA, 0);
    analogWrite(motorPinRV, 255);
    analogWrite(motorPinLV, 255);
    analogWrite(motorPinLA, 0);
  }
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  //status = "connecting to ws";
  //Maak een nieuw JSON bestand aan en sla de verkregen informatie er in op, haal daarna de waarden uit de JSON en stop ze in variabelen
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload);

  bool loggedin = doc["loggedin"];
  String action = doc["action"];
  String game = doc["game"];
  currentGame = game;

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
      //Wanneer er tekst is ontvangen:
      //Als er succesvol is ingelogd, stuur een bericht naar de serial monitor
      if (loggedin) {
        Serial.println("[WS] Robot has logged in to the server");
      }
      //Als er een action is ontvangen, kijk wat die action is en print het naar de serial monitor
      if (action != "null") {
        Serial.println("[WS] Recieved info from websocket:");
        if (action == "prepare") {
          if (!isInGame) {
            Serial.print("[SERVER] prepare game: ");
            Serial.println(game);
            status = "preparing_game";
            //Stuur naar de websocket wanneer de arduino klaar is om het spel te starten
            webSocket.sendTXT("{\"status\": true,\"game\": \"" + game + "\"}");
            status = "ready";
            isInGame = true;
          }
        } else if (action == "start") {
          Serial.print("[SERVER] start game: ");
          Serial.println(game);
          status = "in_game";
        } else if (action == "ended") {
          Serial.print("[SERVER] ended game: ");
          Serial.println(game);
          status = "finished";
          isInGame = false;
          delay(5000);
          status = "ready";
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
