#include <WebServer.h>
#include "web_ui.h"
#include "sd_manager.h"
#include "printer_uart.h"

extern WebServer server;

bool printing = false;
size_t totalLines = 0;
size_t linesSent = 0;

void handlePrint() {
    if (printing) {
        server.send(409, "text/plain", "busy");
        return;
    }

    String filename = "/" + server.arg("file");
    File file = SD_MMC.open(filename);

    if (!file) {
        server.send(404, "text/plain", "not found");
        return;
    }

    totalLines = 0;
    while (file.available()) if (file.read() == '\n') totalLines++;
    file.close();

    linesSent = 0;
    printing = true;

    server.send(200, "text/plain", "printing");

    xTaskCreate([](void*) {
        String fname = "/" + server.arg("file");
        File f = SD_MMC.open(fname);

        while (f.available()) {
            String line = f.readStringUntil('\n');
            line.trim();
            if (line.length() > 0) {
                Serial2.println(line);
                linesSent++;
                delay(5);
            }
        }

        f.close();
        printing = false;
        vTaskDelete(NULL);

    }, "printTask", 8192, NULL, 1, NULL);
}

void handleLogs() {
    extern String serialBuffer;
    server.send(200, "text/plain", serialBuffer);
    serialBuffer = "";
}

void handleStatus() {
    String json = "{";
    json += "\"printing\":" + String(printing ? "true" : "false") + ",";
    json += "\"sent\":" + String(linesSent) + ",";
    json += "\"total\":" + String(totalLines);
    json += "}";
    server.send(200, "application/json", json);
}

void registerWebHandlers() {
    server.on("/", handleRoot);
    server.on("/files", handleListFiles);
    server.on("/raw", handleRaw);
    server.on("/upload", HTTP_POST, [](){}, handleUpload);
    server.on("/delete", handleDelete);
    server.on("/print", handlePrint);
    server.on("/status", handleStatus);
    server.on("/logs", handleLogs);
}
