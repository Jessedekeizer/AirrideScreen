#pragma once

#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include <TFT_eSPI.h> // Hardware-specific library
#include <PNGdec.h>
#include <memory>
#include "SerialManager.h"
#include "SettingsDevice.h"

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
    void ReadSettings();
    void SendSettings();
    void PrintPressure(double front, double back);
    void PrintSettingBool(bool value, int x, int y);
    void TpngDraw(PNGDRAW *);
    int32_t TpngSeek(PNGFILE *, int32_t);
    int32_t TpngRead(PNGFILE *, uint8_t *, int32_t);
    void TpngClose(void *);
    void *TpngOpen(const char *, int32_t *);
    void DrawString(String str, int = 0, int = 0);
    SettingsDevice &getSettings() { return settings; }

    int imageX = 0;
    int imageY = 0;

private:
    TFT_eSPI tft;
    File pngfile;
    PNG png;
    SettingsDevice settings;
    SPIClass spiSD = SPIClass(VSPI);

    TFTStorageHandler();
};

extern TFTStorageHandler &storageHandler;
