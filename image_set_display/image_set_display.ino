#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BluetoothSerial.h>
#include "place_bottle.h"  // Include your image header
#include "Break.h"
#include "clean_imed.h"
#include "drink_imed.h"
#include "hydrate.h"
#include "wait.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define IR_SENSOR_PIN 19  // <-- Make sure to define this pin!

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_OLED_BT");

  pinMode(IR_SENSOR_PIN, INPUT);  // <-- Set the IR pin mode

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Waiting for CMD...");
  display.display();
}

void loop() {
  bool itemPlaced = digitalRead(IR_SENSOR_PIN) == LOW;  // LOW = object present for most IR sensors

  if (!itemPlaced) {
    display.clearDisplay();
    display.drawBitmap(0, 0, place_bottle, 128, 64, WHITE);  // <-- Image name must match your .h
    display.display();
    delay(2000);
    return;
  }

  if (SerialBT.available()) {
    char cmd = SerialBT.read();
    Serial.print("Received: ");
    Serial.println(cmd);

    display.clearDisplay();

    switch (cmd) {
      case '1':
        display.drawBitmap(0, 0, Break, 128, 64, WHITE);
        break;
      case '2':
        display.drawBitmap(0, 0, drink_imed, 128, 64, WHITE);
        break;
      case '3':
        display.drawBitmap(0, 0, wait, 128, 64, WHITE);
        break;
      case '4':
        display.drawBitmap(0, 0, clean_imed, 128, 64, WHITE);
        break;
      case '5':
        display.drawBitmap(0, 0, hydrate, 128, 64, WHITE);
        break;
      case 'C':
      case 'c':
        display.setCursor(0, 0);
        display.print("Screen Cleared");
        break;
      default:
        display.setCursor(0, 0);
        display.print("Unknown CMD: ");
        display.print(cmd);
        break;
    }

    display.display();
  }
}
