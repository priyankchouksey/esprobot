#define CONFIG_MAX_SIZE 4096    /* Sanity limit for config file */

typedef struct {
  String ssid;
  String passphrase;
  String hostname;
  uint8_t ip[4];
  uint8_t netmask[4];
  uint8_t gateway[4];
  bool dhcp;
} config_t;

config_t config;
