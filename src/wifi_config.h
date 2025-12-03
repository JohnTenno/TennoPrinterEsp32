#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

extern const char* ssid;
extern const char* password;

extern const char* hostname;

extern bool usarIPFija;
extern IPAddress local_IP;
extern IPAddress gateway;
extern IPAddress subnet;

void setupWiFi();

#endif
