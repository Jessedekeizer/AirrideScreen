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

    ReadAirSuspensionData();

    sendSettings();
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

void TFTStorageHandler::PrintSettings(bool settingIndicator)
{

    String Box1 = "";
    String Box2 = "";
    String Box3 = "";
    String Box4 = "";
    if (settingIndicator)
    {
        Box1 = String(rideFront, 1);
        Box2 = String(rideBack, 1);
        Box3 = String(parkFront, 1);
        Box4 = String(parkBack, 1);
    }
    else
    {
        Box1 = String(frontMax, 1);
        Box2 = String(backMax, 1);
        Box3 = String(frontMin, 1);
        Box4 = String(backMin, 1);
    }
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    int x = 180;
    int y = 82;
    int fontNum = 2;
    tft.drawString(Box1, x, y, fontNum);
    y = 122;

    tft.drawString(Box2, x, y, fontNum);
    y = 167;

    tft.drawString(Box3, x, y, fontNum);
    y = 207;

    tft.drawString(Box4, x, y, fontNum);
}

void TFTStorageHandler::WriteSettings()
{
    fs::FS &fs = SD;
    File file = fs.open("/settings.txt", FILE_WRITE);
    if (file)
    {
        // Write the first row - Min and Max Pressure
        file.print(frontMin);
        file.print('/');
        file.print(frontMax);
        file.print('/');
        file.print(backMin);
        file.print('/');
        file.println(backMax);

        // Write the second row - Ride Pressure
        file.print(rideFront);
        file.print('/');
        file.println(rideBack);

        // Write the third row - Park Pressure
        file.print(parkFront);
        file.print('/');
        file.println(parkBack);

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

//====================================================================================
//                                    FileFormat
//====================================================================================
// First row min and max pressure of the airsuspension
// Front_min / Front_max / Back_min / Back_max  \n
// second row is about the ride pressure
// Front / Back  \n
// Third row is about the park pressure
// Front / Back   \n
void TFTStorageHandler::ReadAirSuspensionData()
{
    fs::FS &fs = SD;
    File file = fs.open("/settings.txt");
    if (file)
    {
        // Read the first row - Min and Max Pressure
        frontMin = file.readStringUntil('/').toDouble();
        frontMax = file.readStringUntil('/').toDouble();
        backMin = file.readStringUntil('/').toDouble();
        backMax = file.readStringUntil('\n').toDouble();

        // Read the second row - Ride Pressure
        rideFront = file.readStringUntil('/').toDouble();
        rideBack = file.readStringUntil('\n').toDouble();

        // Read the third row - Park Pressure
        parkFront = file.readStringUntil('/').toDouble();
        parkBack = file.readStringUntil('\n').toDouble();

        // Close the file
        file.close();
    }
}

void TFTStorageHandler::sendSettings()
{
    Serial2.print("settings/" + String(frontMin) +
                  "/" + frontMax +
                  "/" + backMin +
                  "/" + backMax +
                  "/" + rideFront +
                  "/" + rideBack +
                  "/" + parkFront +
                  "/" + parkBack + "/");
}

//=========================================v==========================================
//                                      pngDraw
//====================================================================================
// This next function will be called during decoding of the png file to
// render each image line to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
// Callback function to draw pixels to the display

void TFTStorageHandler::PrintScreen(const char *path)
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
    tft.pushImage(0, 0 + pDraw->y, pDraw->iWidth, 1, lineBuffer);
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
