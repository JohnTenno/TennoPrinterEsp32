#include "printer_uart.h"

String serialBuffer = "";

void setupPrinterUART() {
    Serial2.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
}

void pollPrinterUART() {
    while (Serial2.available()) {
        char c = Serial2.read();
        serialBuffer += c;
    }
}
