#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <PNGdec.h>
#include "Box.h"
#include "Screen.h"
#include "vector"
#include <ScreenManager.h>

// Include the TFT library https://github.com/Bodmer/TFT_eSPI
#include "SPI.h"
#include <TFT_eSPI.h> // Hardware-specific library


#define FileSys LittleFS


#define MOSI_PIN 32
#define MISO_PIN 39
#define CLK_PIN 25
#define CS_PIN 33

#include <XPT2046_Bitbang.h>

XPT2046_Bitbang ts(MOSI_PIN, MISO_PIN, CLK_PIN, CS_PIN);

void printPressure();
void printTouchToSerial(TouchPoint);
void checkTouchedBox(int, int);

void printScreen(fs::SDFS &, const char *);
void pngDraw(PNGDRAW *);
int32_t pngSeek(PNGFILE *, int32_t);
int32_t pngRead(PNGFILE *, uint8_t *, int32_t);
void pngClose(void *);
void * pngOpen(const char *, int32_t *);

void writeSettings(fs::FS &);
void readFile(fs::FS &, const char *);
void readAirSuspensionData(fs::FS &);
String getValue(String, char, int);
void getPressure();
void sendSettings();

// Define the box coordinates
std::vector<Box> mainScreenButtons = {
  {0, 0, 70, 120, "Front Up", true, false},
  {0, 120, 70, 120, "Front Down", true, false},
  {263, 0, 70, 120, "Back Up", true, false},
  {263, 120, 70, 120, "Back Down", true, false},
  {132, 174, 48, 42, "Settings1", false, true},
  {84, 174, 48, 42, "Ride", false, false},
  {186, 174, 48, 42, "Park", false, false}
};

std::vector<Box> settings1Buttons = {
  {20,20, 50,50, "mainScreen", false, true },
  {245, 20, 50,50, "save", false, true},
  {120, 80, 30, 30, "RideFD", false, true},
  {205, 80, 30, 30, "RideFU", false, true},
  {120, 113, 30,30, "RideBD", false, true},
  {205, 113, 30, 30, "RideBU", false, true},
  {120, 158, 30,30, "ParkFD", false, true},
  {205, 158, 30,30, "ParkFU", false, true},
  {120, 190, 30,30, "ParkBD", false, true},
  {205, 190, 30,30, "ParkBU", false, true},
  {260,190,30,30, "Settings2", false, true}
};

std::vector<Box> settings2Buttons = {
  {20,20, 50,50, "mainScreen", false, true },
  {245, 20, 50,50, "save", false, true},
  {120, 80, 30, 30, "MaxFD", false, true},
  {205, 80, 30, 30, "MaxFU", false, true},
  {120, 113, 30,30, "MaxBD", false, true},
  {205, 113, 30, 30, "MaxBU", false, true},
  {120, 158, 30,30, "MinFD", false, true},
  {205, 158, 30,30, "MinFU", false, true},
  {120, 190, 30,30, "MinBD", false, true},
  {205, 190, 30,30, "MinBU", false, true},
  {20,190,30,30, "Settings1", false, true}
};


TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
File pngfile;
PNG png;

  SPIClass spiSD = SPIClass(VSPI);

double frontMin = 0;
double frontMax = 0;
double backMin = 0;
double backMax = 0;
double rideFront = 0;
double rideBack = 0;
double parkFront = 0;
double parkBack = 0;

double front = 0;
double back = 0;


void printSettings(bool);
bool buttonHandler(String);

Screen mainScreen("mainScreen", "/MainScreen.png");
Screen settings1("Settings1", "/Settings1.png");
Screen settings2("Settings2", "/Settings2.png");

ScreenManager Manager(&mainScreen);

String incoming = "";

//====================================================================================
//                                    Setup
//====================================================================================
void setup()
{
  Serial.begin(9600, SERIAL_8N1);
  Serial2.begin(9600, SERIAL_8N1, 27, 22);
  Serial.setTimeout(20);

  ts.begin();

  if (!SD.begin(SS, spiSD, 80000000))
  {
    
    return;
  }
 

  // Initialise the TFT
  tft.init();
  tft.setRotation(1);

  tft.fillScreen(TFT_BLACK);
  
  mainScreen.addButtons(mainScreenButtons);
  settings1.addButtons(settings1Buttons);
  settings2.addButtons(settings2Buttons);
  mainScreen.activate(true);
  printScreen(SD, mainScreen.getPath());
  Manager.addScreen(&mainScreen);
  Manager.addScreen(&settings1);
  Manager.addScreen(&settings2);
  
  // testing
  
  //

  readAirSuspensionData(SD);
  delay(2000);
  sendSettings();
  printPressure();
}

//====================================================================================
//                                    Loop
//====================================================================================
void loop()
{
  TouchPoint touch = ts.getTouch();
  if(Manager.getActiveScreen()->getName() == "mainScreen")
  {
    getPressure();
    printPressure();
  }
  if (touch.zRaw != 0)
  {
    //printTouchToSerial(touch);
    String button = Manager.getActiveScreen()->handleTouch(touch.x, touch.y);
    if(!buttonHandler(button)){
      if(button == "save"){
        writeSettings(SD);
        sendSettings();
        Manager.changeActiveScreen("mainScreen");
      }else{
        Manager.changeActiveScreen(button);
      }
    }else{
      if(Manager.getActiveScreen()->getName() == "mainScreen"){
        printPressure();
      }else if(Manager.getActiveScreen()->getName() == "Settings1"){
        printSettings(true);
      }else if(Manager.getActiveScreen()->getName() == "Settings2"){
        printSettings(false);
      }
    }
  }else
  {
    Manager.getActiveScreen()->releaseButton();

  }
  
  if(Manager.getChange())
  {
    Manager.Change(false);
    printScreen(SD, Manager.getActiveScreen()->getPath());

    if(Manager.getActiveScreen()->getName() == "mainScreen"){
        printPressure();
      }else if(Manager.getActiveScreen()->getName() == "Settings1"){
        readAirSuspensionData(SD);
        printSettings(true);
      }else if(Manager.getActiveScreen()->getName() == "Settings2"){
        printSettings(false);
      }
  }
  delay(100);

}

void sendSettings(){
  Serial2.print("settings/" + String(frontMin) + 
"/" + frontMax + 
"/" +  backMin +
"/" +  backMax +
"/" +  rideFront +
"/" +  rideBack +
"/" +  parkFront +
"/" +  parkBack + "/");
}

void getPressure(){
  while(Serial2.available()){
    delay(2);
    char c = Serial2.read();
    incoming += c;
  }
  if(incoming != "")
  {
    Serial.println(incoming);
    try
    {
      front = getValue(incoming, '/', 0).toDouble();
      back = getValue(incoming, '/', 1).toDouble();
      Serial.print(front);
      Serial.println(back);
    }
    catch(const std::exception& e)
    {
      
    }
    incoming = "";
  }
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


bool buttonHandler(String button){
  
  if(button.startsWith("Ride")){
    if(button.endsWith("FU")){
      rideFront += 0.1;
    }else if(button.endsWith("FD")){
      rideFront -= 0.1;
    }else if(button.endsWith("BU")){
      rideBack += 0.1;
    }else if(button.endsWith("BD")){
      rideBack -= 0.1;
    }
  }else if(button.startsWith("Park")){
    if(button.endsWith("FU")){
      parkFront += 0.1;
    }else if(button.endsWith("FD")){
      parkFront -= 0.1;
    }else if(button.endsWith("BU")){
      parkBack += 0.1;
    }else if(button.endsWith("BD")){
      parkBack -= 0.1;
    }
  }else if(button.startsWith("Max")){
    if(button.endsWith("FU")){
      frontMax += 0.1;
    }else if(button.endsWith("FD")){
      frontMax -= 0.1;
    }else if(button.endsWith("BU")){
      backMax += 0.1;
    }else if(button.endsWith("BD")){
      backMax -= 0.1;
    }
  }else if(button.startsWith("Min")){
    if(button.endsWith("FU")){
      frontMin += 0.1;
    }else if(button.endsWith("FD")){
      frontMin -= 0.1;
    }else if(button.endsWith("BU")){
      backMin += 0.1;
    }else if(button.endsWith("BD")){
      backMin -= 0.1;
    }
  }else {
    return false;
    }
  return true;
}

void printTouchToSerial(TouchPoint p)
{
  Serial.print("Pressure = ");
  Serial.print(p.zRaw);
  Serial.print(", x = ");
  Serial.print(p.x);
  Serial.print(", y = ");
  Serial.print(p.y);
  Serial.println();
}

void printPressure()
{
  
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    int x = 85;
    int y = 140;
    int fontNum = 4;
    tft.drawString(String(front, 1), x, y, fontNum);

    x = 201;
    y = 140;

    tft.drawString(String(back,1), x, y, fontNum);
  
}

void printSettings(bool settingIndicator)
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
  else
  {
    Box1 = String(frontMax, 1);
    Box2 = String(backMax, 1);
    Box3 = String(frontMin, 1);
    Box4 = String(backMin, 1);
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
}

void writeSettings(fs::FS &fs)
{
  File file = fs.open("/settings.txt", FILE_WRITE);
  if (!file)
  {
    
    return;
  }
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
  else
  {
    
  }
}


void readFile(fs::FS &fs, const char *path)
{
  

  File file = fs.open(path);
  if (!file)
  {
    
    return;
  }

 
  while (file.available())
  {
    Serial.write(file.read());
  }
  file.close();
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
void readAirSuspensionData(fs::FS &fs)
{
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
  else
  {

  }
}


//=========================================v==========================================
//                                      pngDraw
//====================================================================================
// This next function will be called during decoding of the png file to
// render each image line to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
// Callback function to draw pixels to the display

void printScreen(fs::SDFS &fs, const char *path)
{
    
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
    else
    {
      
      return;
    }
  }
  else
  {
  
    return;
  }
}

void pngDraw(PNGDRAW *pDraw)
{
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(0, 0 + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void *pngOpen(const char *filename, int32_t *size)
{
  pngfile = SD.open(filename, "r");
  *size = pngfile.size();
  return &pngfile;
}

void pngClose(void *handle)
{
  File pngfile = *((File *)handle);
  if (pngfile)
    pngfile.close();
}

int32_t pngRead(PNGFILE *page, uint8_t *buffer, int32_t length)
{
  if (!pngfile)
    return 0;
  page = page; // Avoid warning
  return pngfile.read(buffer, length);
}

int32_t pngSeek(PNGFILE *page, int32_t position)
{
  if (!pngfile)
    return 0;
  page = page; // Avoid warning
  return pngfile.seek(position);
}