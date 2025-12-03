#include <Arduino.h>
#include <WebServer.h>

WebServer server(80);

#include "wifi_config.h"
#include "printer_uart.h"
#include "sd_manager.h"
#include "web_handlers.h"

void setup() {
    Serial.begin(115200);

    setupPrinterUART();
    setupWiFi();
    setupSD();

    registerWebHandlers();
    server.begin();

    Serial.println("TennoPrinter listo!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    server.handleClient();
    pollPrinterUART();
}
