#include "sd_manager.h"
#include <WebServer.h>

extern WebServer server;

void setupSD() {
    if (!SD_MMC.begin()) {
        Serial.println("Error iniciando SD_MMC");
    } else {
        Serial.println("SD_MMC iniciado");
    }
}

void handleListFiles() {
    File root = SD_MMC.open("/");
    String json = "[";

    while (true) {
        File entry = root.openNextFile();
        if (!entry) break;

        if (!entry.isDirectory()) {
            json += "{\"name\":\"" + String(entry.name()) + "\",";
            json += "\"size\":" + String(entry.size()) + "},";
        }
        entry.close();
    }

    if (json.endsWith(",")) json.remove(json.length() - 1);
    json += "]";

    server.send(200, "application/json", json);
}

void handleRaw() {
    String fname = "/" + server.arg("file");
    File f = SD_MMC.open(fname);
    if (!f) { server.send(404, "text/plain", "not found"); return; }
    server.streamFile(f, "text/plain");
    f.close();
}

void handleUpload() {
    HTTPUpload& upload = server.upload();
    static File uploadFile;

    if (upload.status == UPLOAD_FILE_START) {
        uploadFile = SD_MMC.open("/" + upload.filename, FILE_WRITE);
    }
    else if (upload.status == UPLOAD_FILE_WRITE) {
        if (uploadFile) uploadFile.write(upload.buf, upload.currentSize);
    }
    else if (upload.status == UPLOAD_FILE_END) {
        if (uploadFile) uploadFile.close();
        server.send(200, "text/plain", "OK");
    }
}

void handleDelete() {
    String name = server.arg("name");

    if (SD_MMC.remove("/" + name)) {
        server.send(200, "text/plain", "deleted");
    } else {
        server.send(400, "text/plain", "error");
    }
}
