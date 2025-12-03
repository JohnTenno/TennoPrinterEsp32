#include <WiFi.h>
#include <ESPmDNS.h>
#include "wifi_config.h"

const char* ssid = "ssid_example";
const char* password = "password_example";

const char* hostname = "TennoPrinter";

bool usarIPFija = false;
IPAddress local_IP(192,168,1,77);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

void setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(hostname);

    if (usarIPFija) WiFi.config(local_IP, gateway, subnet);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(100);

    if (!MDNS.begin(hostname)) {
        Serial.println("Error iniciando mDNS");
    } else {
        Serial.println("mDNS OK: http://" + String(hostname) + ".local");
    }
}
