#pragma once

#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include <TFT_eSPI.h> // Hardware-specific library
#include <PNGdec.h>
#include <memory>
#include "SerialManager.h"

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

    void PrintImage(const char *, int = 0, int = 0);
    void WriteSettings();
    void WriteLog(String message);
    void ReadFile(const char *);
    void ReadAirSuspensionData();
    void sendSettings();
    void PrintPressure(double front, double back);
    void TpngDraw(PNGDRAW *);
    int32_t TpngSeek(PNGFILE *, int32_t);
    int32_t TpngRead(PNGFILE *, uint8_t *, int32_t);
    void TpngClose(void *);
    void *TpngOpen(const char *, int32_t *);
    void DrawString(String str, int x = 0, int y = 0);
    void DrawRect(int x, int y, int width, int height, uint32_t color = TFT_BLACK);

    double frontMax = 0;
    double backMax = 0;
    double rideFront = 0;
    double rideBack = 0;
    double frontUpX = 0;
    double frontDownX = 0;
    double backUpX = 0;
    double backDownX = 0;
    int imageX = 0;
    int imageY = 0;

private:
    TFT_eSPI tft;
    File pngfile;
    PNG png;

    SPIClass spiSD = SPIClass(VSPI);

    TFTStorageHandler();
};

extern TFTStorageHandler &storageHandler;
