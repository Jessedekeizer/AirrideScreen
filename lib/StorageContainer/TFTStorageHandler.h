#pragma once

#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include <TFT_eSPI.h> // Hardware-specific library
#include <PNGdec.h>
#include <memory>

class TFTStorageHandler
{
public:
    static TFTStorageHandler &GetInstance()
    {
        static TFTStorageHandler instance;
        return instance;
    }
    TFTStorageHandler(const TFTStorageHandler &) = delete;
    TFTStorageHandler &operator=(const TFTStorageHandler &) = delete;

    void PrintScreen(const char *);
    void WriteSettings();
    void ReadFile(const char *);
    void ReadAirSuspensionData();
    void sendSettings();
    void PrintSettings(bool settingIndicator);
    void PrintPressure(double front, double back);
    void TpngDraw(PNGDRAW *);
    int32_t TpngSeek(PNGFILE *, int32_t);
    int32_t TpngRead(PNGFILE *, uint8_t *, int32_t);
    void TpngClose(void *);
    void *TpngOpen(const char *, int32_t *);

    double frontMin = 0;
    double frontMax = 0;
    double backMin = 0;
    double backMax = 0;
    double rideFront = 0;
    double rideBack = 0;
    double parkFront = 0;
    double parkBack = 0;

private:
    TFT_eSPI tft;
    File pngfile;
    PNG png;

    SPIClass spiSD = SPIClass(VSPI);

    TFTStorageHandler();
};

extern TFTStorageHandler &storageHandler;
