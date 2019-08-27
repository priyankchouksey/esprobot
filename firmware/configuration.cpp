#include "configuration.h"
config_t config;

void initConfig(){
  SPIFFS.begin();
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
config_t getConfig(){
  return config;
}
