#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "BluetoothSerial.h"  // Built-in ESP32 Bluetooth support

// OLED Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Bluetooth Serial
BluetoothSerial SerialBT;

// Sample Images (bitmaps)
#include "place_bottle.h"
#include "drink_imed.h"
#include "clean_imed.h"
#include "hydrate.h"
#include "Break.h"
#include "wait.h"
// Add more as needed

// Bluetooth (if needed for UNO/Nano)
//#include <SoftwareSerial.h>
//SoftwareSerial BT(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);        // For debugging / Bluetooth
  // BT.begin(9600);         // Use this if using SoftwareSerial for HC-05

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    while (true);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Waiting for command...");
  display.display();
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();  // or use BT.read() if SoftwareSerial

    display.clearDisplay();

    switch (command) {
      case '1':
        display.drawBitmap(0, 0, place_bottle, 128, 64, WHITE);
        break;

      case '2':
        display.drawBitmap(0, 0, drink_imed, 128, 64, WHITE);
        break;

      case '3':
        display.drawBitmap(0, 0, hydrate, 128, 64, WHITE);
        break;

      case '4':
        display.drawBitmap(0, 0, clean_imed, 128, 64, WHITE);
        break;

      case '5':
        display.drawBitmap(0, 0, Break, 128, 64, WHITE);
        break;

      case 'C':
       display.drawBitmap(0, 0, wait, 128, 64, WHITE);
        break;

      case 'c':
        display.setCursor(0, 0);
        display.println("Screen Cleared");
        break;

      default:
        display.setCursor(0, 0);
        display.println("Unknown command:");
        display.print("Code: ");
        display.print(command);
        break;
    }

    display.display();
  }
}
