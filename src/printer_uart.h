#ifndef PRINTER_UART_H
#define PRINTER_UART_H

#include <Arduino.h>

#define RX_PIN 3
#define TX_PIN 1

extern String serialBuffer;

void setupPrinterUART();
void pollPrinterUART();

#endif
