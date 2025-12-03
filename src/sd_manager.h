#ifndef SD_MANAGER_H
#define SD_MANAGER_H

#include <Arduino.h>
#include <FS.h>
#include <SD_MMC.h>

void setupSD();
void handleListFiles();
void handleUpload();
void handleDelete();
void handleRaw();

#endif
