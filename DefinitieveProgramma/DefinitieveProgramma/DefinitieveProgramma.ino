#include <Arduino.h>
#include <WiFi.h> //WiFi library
#include <WebSocketsClient.h> //websocket library
#include <ArduinoJson.h> //arduino JSON library
#include <analogWrite.h> //analogWrite library

#include <Adafruit_VL53L0X.h> //liDAR library
#include <Adafruit_GFX.h> //graphics library
#include <Adafruit_SSD1306.h> //display library

Adafruit_SSD1306 display(128, 32, &Wire, 4); //display variable
Adafruit_VL53L0X lidar = Adafruit_VL53L0X(); //LiDAR variable
WebSocketsClient webSocket; //websocket variable

const char* ssid = ""; //network name
const char* password = ""; //network password
const char* ipadress = ""; //server ip-adress
const int port = 0; //websocket port

const String games[3] = {"butler", "maze", "race"}; //the 3 games

bool isPrepared = false; //robot is done preparing ? true : false

String currentGame = "idle"; //currently active game
String botStatus = "ready"; //status variable
bool isDriving = false; //driving status
int acceleration = 0; //De acceleratie van de robot

unsigned long previousMillis = 0; //time since last measured time
const long interval = 5000; //loop timer

int IRPinR = 34; //right IR sensor
int IRPinL = 39; //left IR sensor
int IRValR;
int IRValL;

#include "functions.h" //import motor functions
#include "butlerspelen.h" //import butler game
#include "race.h" //import race game
#include "doolhof.h" //import maze game

void setup() {
  //Start serial monitor
  Serial.begin(115200);  
  
  //Initialize the lidar
  if (!lidar.begin()) {
    Serial.println(F("Failed to connect to VL53L0X (LiDAR)"));
    while (1);
  }
  //Initialize the display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Failed to connect to SSD1306 (display)"));
    while (1);
  }

  //Connect to the WiFi network. try 10 times, else restart device
  WiFi.begin(ssid, password);
  
  int atempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("[WIFI] Connecting to WiFi..");
    writeToDisplay("WiFI...", 0, 1);
    atempts++;
    if (atempts >= 10) {
      Serial.println("[WIFI] Restarting device");
      exit(0);
    }
  }
  Serial.println("[WIFI] Connected to the WiFi network");
  writeToDisplay("Conn WiFI", 0, 1);

  //Initialize the websocket. (ip, port, URL)
  webSocket.begin(ipadress, port, "/");

  //Connect event function to the events
  webSocket.onEvent(webSocketEvent);

  //Try to reconnect every 5s when connection is lost
  webSocket.setReconnectInterval(5000);
  
  //initialize motor functions
  motorInit();

  Serial.println(WiFi.macAddress());
}

void loop() {
  //Keep the websocket active
  webSocket.loop();
  
  IRValL = analogRead(IRPinL);
  IRValR = analogRead(IRPinR);

  //Send websocket the robot info every [interval] milliseconds
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    //Execute this every [interval] milliseconds:
    webSocket.sendTXT("{\"status\": \"" + botStatus + "\",\"isDriving\": " + isDriving + ",\"acceleration\":" + acceleration + "}");
  }

  //When the robot is in a game and is fully ready, keep the game active
  if (botStatus == "in_game" && isPrepared == true) {
    if (currentGame == games[0]) { //Butler
      butlerLoop();
    } else if (currentGame == games[1]) { //Maze
      mazeLoop();
    } else if (currentGame == games[2]) { //Race
      raceLoop();
    }
  }

  //If the bot has finished a game, stop the bot.
  if (botStatus == "finished" || isPrepared == false) {
    standStill();
  }

  if(botStatus != "in_game") {
    //Write the status to the display
    writeToDisplay("St: " + botStatus, 0, 0);
  }
}

//Execute this when something happens with the websocket
void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  //Deserialize the json recieved from the websocket
  const size_t capacity = JSON_OBJECT_SIZE(3);
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, payload);

  bool loggedin = doc["loggedin"];
  String action = doc["action"];
  String game = doc["game"];

  char recievedMsg[100];
  serializeJson(doc, recievedMsg);

  switch (type) {
    case WStype_CONNECTED:
      Serial.println("[WS] Connected to the websocket");
      webSocket.sendTXT("{\"action\": \"login\",\"id\": \"" + WiFi.macAddress() + "\"}");
      break;
    case WStype_DISCONNECTED:
      //Wanneer de verbinding met de websocket is verbroken:
      Serial.println("[WS] Disconnected from the websocket");
      currentGame = "idle";
      botStatus = "ready";
      break;
    case WStype_TEXT:
      //Wanneer er tekst is ontvangen:
      //Als er succesvol is ingelogd, stuur een bericht naar de serial monitor
      Serial.print("[SERVER] ");
      Serial.println(recievedMsg);
      if (loggedin) {
        Serial.println("[WS] Robot has logged in to the websocket");
      }
      //Als er een action is ontvangen, kijk wat die action is en print het naar de serial monitor
      if (action != "null") {
        bool validGame = false;
        for (int i = 0; i < 3; i++) {
          if (games[i] == game) {
            validGame = true;
          }
        }
        if (validGame == true) {
          if (action == "prepare") {
            if (currentGame == "idle") {
              Serial.print("[SERVER] prepare game: ");
              Serial.println(game);
              botStatus = "preparing_game";

              //Stuur naar de websocket wanneer de arduino klaar is om het spel te starten
              webSocket.sendTXT("{\"status\": true,\"game\": \"" + game + "\"}");
              isPrepared = true;
              botStatus = "ready";
            }
          } else if (action == "start") {
            if (isPrepared == true) {
              Serial.print("[SERVER] start game: ");
              Serial.println(game);
              botStatus = "in_game";
              currentGame = game;
            } else {
              Serial.println("[ERROR] Cannot start " + game + ": not prepared");
              webSocket.sendTXT("{\"error\": \"GAME_NOT_PREPARED\"}");
            }
          } else if (action == "ended") {
            if (currentGame != "idle") {
              if (currentGame == game) {
                Serial.print("[SERVER] ended game: ");
                Serial.println(currentGame);
                botStatus = "finished";
                currentGame = "idle";
                isPrepared = false;
                botStatus = "ready";
              }
            } else {
              Serial.println("[ERROR] Er is geen spel gaande");
            }
          }
        } else {
          //Invalid game
          Serial.println("[ERROR] Invalid game");
          webSocket.sendTXT("{\"error\": \"GAME_NOT_FOUND\"}");
        }
      }
      break;
    case WStype_PING:
          Serial.println("[SERVER] ping");
    case WStype_PONG:
          Serial.println("[BOT] pong");
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
  }
}
