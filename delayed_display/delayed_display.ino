#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Include image headers
#include "IMG_0007.h" 
#include "IMG_0008.h"
#include "IMG_0043.h"
#include "IMG_0044.h"
#include "IMG_0046.h"  

// Pointer array to images
const unsigned char* imageList[] = {
  IMG_0007,
  IMG_0008,
  IMG_0043,
  IMG_0044,
  IMG_0046  
};

const int imageCount = sizeof(imageList) / sizeof(imageList[0]);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // I2C address
  display.clearDisplay();
  display.display();
}

void loop() {
  for (int i = 0; i < imageCount; i++) {
    display.clearDisplay();
    display.drawBitmap(0, 0, imageList[i], 128, 64, WHITE);
    display.display();
    delay(3000);  
  }

  while (true);  // stop after one cycle (remove to loop endlessly)
}
