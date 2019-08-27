#if !defined(CONFIGURATION_)
#define CONFIGURATION_

#include <SPIFFS.h>
#include <ArduinoJson.h>

#define CONFIG_MAX_SIZE 4096    /* Sanity limit for config file */
const char CONFIG_FILE[] = "/config.json";

typedef struct {
  String ssid;
  String passphrase;
  String hostname;
  uint8_t ip[4];
  uint8_t netmask[4];
  uint8_t gateway[4];
  bool dhcp;
} config_t;

void initConfig();
void loadConfig();
void saveConfig();
config_t getConfig();
#endif
