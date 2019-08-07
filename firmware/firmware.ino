#include "pins.h"
#include "configuration.h"
#include "firmware.h"
#include <SPIFFS.h>

#if defined(ESP32)
  #include <WiFi.h>
  #define LED_PIN 2
#else
  #include <ESP8266WiFi.h>
  #define LED_PIN 4
#endif
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

AsyncWebServer  web(80); // Web Server
AsyncWebSocket  ws("/ws"); //Websocket

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  SPIFFS.begin();
  WiFi.mode(WIFI_AP);
  String ssid = "ESPBoardTest";
  WiFi.softAP(ssid.c_str());
  initWeb();

}

void loop() {
  // put your main code here, to run repeatedly:

}

void initWeb() {
  ws.onEvent(wsEvent);
  web.addHandler(&ws);
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), "*");
  web.serveStatic("/", SPIFFS, "/www/").setDefaultFile("index.html");
  web.begin();
}

void wsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
        AwsEventType type, void * arg, uint8_t *data, size_t len) {
          char* temp = reinterpret_cast<char*>(data);
          Serial.println(temp);
  switch (type) {
    case WS_EVT_DATA:
      handleWsData(data);
      break;
    case WS_EVT_CONNECT:
      Serial.print(F("* WS Connect - "));
      Serial.println(client->id());
      break;
    case WS_EVT_DISCONNECT:
      Serial.print(F("* WS Disconnect - "));
      Serial.println(client->id());
      break;
    case WS_EVT_PONG:
      Serial.println(F("* WS PONG *"));
      break;
    case WS_EVT_ERROR:
      Serial.println(F("** WS ERROR **"));
      break;
  }
}

void handleWsData(uint8_t *data){
  DynamicJsonDocument jsonDoc(1024);
  auto error = deserializeJson(jsonDoc, reinterpret_cast<char*>(data));
  if (error){
    Serial.println(F("* Not a Valid JsonData *"));
    return;
  }
  JsonObject json = jsonDoc.to<JsonObject>();
  uint16_t pin = json["pin"].as<int>();
  uint16_t val = json["value"].as<int>();
  Serial.print("Received data ");
  Serial.print(val);
  Serial.print(" at pin ");
  Serial.println(pin);
  digitalWrite(pin, val);
}

// Load configugration JSON file
void loadConfig() {
    // Zeroize Config struct
    memset(&config, 0, sizeof(config));

    // Load CONFIG_FILE json. Create and init with defaults if not found
    File file = SPIFFS.open(CONFIG_FILE, "r");
    if (!file) {
        Serial.println(F("- No configuration file found."));
        config.ssid = "";
        config.passphrase = "";
        saveConfig();
    } else {
        // Parse CONFIG_FILE json
        size_t size = file.size();
        if (size > CONFIG_MAX_SIZE) {
            Serial.println(F("*** Configuration File too large ***"));
            return;
        }

        std::unique_ptr<char[]> buf(new char[size]);
        file.readBytes(buf.get(), size);

        DynamicJsonDocument jsonDoc(1024);
        auto error = deserializeJson(jsonDoc, buf.get());
        if (error) {
            Serial.println(F("*** Configuration File Format Error ***"));
            return;
        }
        JsonObject json = jsonDoc.to<JsonObject>();
        //read network configuration
        if (json.containsKey("network")) {
          JsonObject networkJson = json["network"];
          config.ssid = networkJson["ssid"].as<String>();
          config.passphrase = networkJson["passphrase"].as<String>();
          for (int i = 0; i < 4; i++) {
            config.ip[i] = networkJson["ip"][i];
            config.netmask[i] = networkJson["netmask"][i];
            config.gateway[i] = networkJson["gateway"][i];
          }
          config.dhcp = networkJson["dhcp"];
          config.hostname = networkJson["hostname"].as<String>();
        }
        Serial.println(F("- Configuration loaded."));
    }
}
void saveConfig() {

}
