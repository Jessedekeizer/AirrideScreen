#include "TFTStorageHandler.h"
#define MAX_IMAGE_WIDTH 320

TFTStorageHandler &storageHandler = storageHandler.GetInstance();

void pngDraw(PNGDRAW *pDraw)
{
    storageHandler.TpngDraw(pDraw);
}

void *pngOpen(const char *filename, int32_t *size)
{
    return storageHandler.TpngOpen(filename, size);
}

void pngClose(void *handle)
{
    storageHandler.TpngClose(handle);
}

int32_t pngRead(PNGFILE *page, uint8_t *buffer, int32_t length)
{
    return storageHandler.TpngRead(page, buffer, length);
}

int32_t pngSeek(PNGFILE *page, int32_t position)
{
    return storageHandler.TpngSeek(page, position);
}

TFTStorageHandler::TFTStorageHandler()
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

void TFTStorageHandler::PrintPressure(double front, double back)
{

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    int x = 85;
    int y = 140;
    int fontNum = 4;
    tft.drawString(String(front, 1), x, y, fontNum);

    x = 201;
    y = 140;

    tft.drawString(String(back, 1), x, y, fontNum);
}

void TFTStorageHandler::DrawString(String str, int x, int y)
{
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(str, x, y, 2);
}

void TFTStorageHandler::WriteSettings()
{
    fs::FS &fs = SD;
    File file = fs.open("/settings.bin", FILE_WRITE);
    if (!file)
    {
        serialManager.Debug("Cannot open file to write");
        return;
    }

    // Write the entire struct in one go
    size_t bytesWritten = file.write((const uint8_t *)&settings, sizeof(settings));

    if (bytesWritten != sizeof(settings))
    {
        serialManager.Debug("Failed to write settings");
    }
    else
    {
        serialManager.Debug("Settings saved successfully");
    }

    file.close();
}

void TFTStorageHandler::WriteLog(String message)
{
    fs::FS &fs = SD;
    File file = fs.open("/LOG.txt", FILE_APPEND);
    if (file)
    {
        file.println(message);

        // Close the file
        file.close();
    }
}

void TFTStorageHandler::ReadFile(const char *path)
{
    fs::FS &fs = SD;
    File file = fs.open(path);
    if (file)
    {
        while (file.available())
        {
            Serial.write(file.read());
        }
        file.close();
    }
}

void TFTStorageHandler::ReadSettings()
{
    fs::FS &fs = SD;
    File file = fs.open("/settings.bin", FILE_READ);
    if (!file)
    {
        serialManager.Debug("Cannot open file to read");
        return;
    }

    // Read the entire struct in one go
    size_t bytesRead = file.read((uint8_t *)&settings, sizeof(settings));

    if (bytesRead != sizeof(settings))
    {
        serialManager.Debug("Failed to read settings");
    }
    else
    {
        serialManager.Debug("Settings loaded successfully");
    }

    file.close();
}

void TFTStorageHandler::SendSettings()
{
    serialManager.sendMessage("settings/" + String(settings.frontMax) +
                              "/" + settings.backMax +
                              "/" + settings.rideFront +
                              "/" + settings.rideBack +
                              "/" + settings.frontUpX +
                              "/" + settings.frontDownX +
                              "/" + settings.backUpX +
                              "/" + settings.backDownX + "/");
}

//=========================================v==========================================
//                                      pngDraw
//====================================================================================
// This next function will be called during decoding of the png file to
// render each image line to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
// Callback function to draw pixels to the display

void TFTStorageHandler::PrintImage(const char *path, int x, int y)
{
    fs::FS &fs = SD;
    File file = fs.open(path);

    if (file)
    {
        String strname = file.name();
        strname = "/" + strname;

        if (!file.isDirectory() && strname.endsWith(".png"))
        {
            int16_t rc = png.open(strname.c_str(), pngOpen, pngClose, pngRead, pngSeek, pngDraw);
            if (rc == PNG_SUCCESS)
            {
                tft.startWrite();

                imageX = x;
                imageY = y;

                uint32_t dt = millis();
                if (png.getWidth() > MAX_IMAGE_WIDTH)
                {
                }
                else
                {
                    rc = png.decode(NULL, 0);
                    png.close();
                }
                tft.endWrite();
            }
        }
    }
    return;
}

void TFTStorageHandler::TpngDraw(PNGDRAW *pDraw)
{
    uint16_t lineBuffer[MAX_IMAGE_WIDTH];
    png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
    tft.pushImage(imageX, imageY + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void *TFTStorageHandler::TpngOpen(const char *filename, int32_t *size)
{
    pngfile = SD.open(filename, "r");
    *size = pngfile.size();
    return &pngfile;
}

void TFTStorageHandler::TpngClose(void *handle)
{
    File pngfile = *((File *)handle);
    if (pngfile)
        pngfile.close();
}

int32_t TFTStorageHandler::TpngRead(PNGFILE *page, uint8_t *buffer, int32_t length)
{
    if (!pngfile)
        return 0;
    page = page; // Avoid warning
    return pngfile.read(buffer, length);
}

int32_t TFTStorageHandler::TpngSeek(PNGFILE *page, int32_t position)
{
    if (!pngfile)
        return 0;
    page = page; // Avoid warning
    return pngfile.seek(position);
}
