#ifndef TFTSTORAGEHANDLER_H
#define TFTSTORAGEHANDLER_H

#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include <TFT_eSPI.h> // Hardware-specific library
#include <PNGdec.h>

class TFTStorageHandler
{
public:
    static TFTStorageHandler &getInstance()
    {
        static TFTStorageHandler instance;
        return instance;
    }

    void printScreen(const char *);
    void writeSettings();
    void readFile(const char *);
    void readAirSuspensionData();
    void TpngDraw(PNGDRAW *);
    int32_t TpngSeek(PNGFILE *, int32_t);
    int32_t TpngRead(PNGFILE *, uint8_t *, int32_t);
    void TpngClose(void *);
    void *TpngOpen(const char *, int32_t *);

private:
    TFT_eSPI tft;
    File pngfile;
    PNG png;

    SPIClass spiSD = SPIClass(VSPI);

    TFTStorageHandler()
    {
        if (!SD.begin(SS, spiSD, 80000000))
        {
            return;
        }

        tft = TFT_eSPI(); // Invoke custom library

        // Initialise the TFT
        tft.init();
        tft.setRotation(1);

        tft.fillScreen(TFT_BLACK);
    };
};

#endif