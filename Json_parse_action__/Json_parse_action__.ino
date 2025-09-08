#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <time.h>

// ===== OLED Settings =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ===== IR Sensor =====
#define IR_PIN 34

// ===== BLE UUIDs =====
#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-ba0987654321"

// ===== Frost JSON =====
const char *jsonData = R"rawliteral(
{
  "reminders": [
    {
      "ReminderID": "hydration",
      "Type": "DrinkWater",
      "Enabled": true,
      "StartTime": "3HRS",
      "RelativeTrigger": "if user skipped drinkwater1 ,trigger 2(skipped) ,trigger3",
      "definedByUser": true,
      "DisplayImage": null,
      "TextMessage": null,
      "VoiceMessageID": null,
      "Priority": null,
      "DisplayDurationSec": null,
      "AudioFileID": null
    },
    {
      "ReminderID": "drink_water_1",
      "Type": "DrinkWater",
      "Enabled": true,
      "RelativeTrigger": "if user responded for trigger 1 skip 2,3,reset the countdown",
      "DisplayImage": "Image1",
      "TextMessage": "Time to hydrate!",
      "VoiceMessageID": "msg1",
      "Priority": 3,
      "DisplayDurationSec": 60,
      "AudioFileID": "beep1"
    },
    {
      "ReminderID": "drink_water_2",
      "Type": "DrinkWater",
      "Enabled": true,
      "RelativeTrigger": "if user responded for trigger 2, skip 3 reset the countdown",
      "DisplayImage": "Image2",
      "TextMessage": "Sip break!",
      "VoiceMessageID": "msg2",
      "Priority": 3,
      "DisplayDurationSec": 60,
      "AudioFileID": "beep1"
    },
    {
      "ReminderID": "drink_water_3",
      "Type": "DrinkWater",
      "Enabled": true,
      "RelativeTrigger": "if user responded for trigger 3, reset countdown",
      "DisplayImage": "Image3",
      "TextMessage": "Please drink!",
      "VoiceMessageID": "msg3",
      "Priority": 3,
      "DisplayDurationSec": 60,
      "AudioFileID": "beep1"
    },
    {
      "ReminderID": "pomodoro",
      "Type": "Pomodoro",
      "Enabled": true,
      "StartTime": "20m",
      "RelativeTrigger": "within the phases 1,2,3",
      "definedByUser": true,
      "TimeSlots": ["09:00-11:00", "12:00-13:15", "15:00-16:00"],
      "Constraints": "phase1=a-b, phase2=c-d, phase3=e-f",
      "DisplayImage": "Iclock with progressive bar",
      "TextMessage": "Focus session started",
      "VoiceMessageID": "msg4",
      "Priority": 4,
      "AudioFileID": "beep2"
    },
    {
      "ReminderID": "place_bottle",
      "Type": "PlaceBottle",
      "Enabled": true,
      "RelativeTrigger": "quick response if hydration enabled",
      "DisplayImage": "Place bottle on dock",
      "TextMessage": "Place the bottle on the dock",
      "VoiceMessageID": "msg5",
      "Priority": 5,
      "AudioFileID": "beep3"
    },
    {
      "ReminderID": "clean_bottle_1",
      "Type": "CleanBottle",
      "Enabled": true,
      "RelativeTrigger": "after setup of device, every 2 days once",
      "DisplayImage": "Image5",
      "TextMessage": "Cleaning recommended",
      "VoiceMessageID": "msg6",
      "Priority": 2,
      "DisplayDurationSec": 60,
      "AudioFileID": "beep4"
    },
    {
      "ReminderID": "clean_bottle_2",
      "Type": "CleanBottle",
      "Enabled": true,
      "RelativeTrigger": "after setup of device, every 3 days once",
      "DisplayImage": "Image6",
      "TextMessage": "Must clean the bottle",
      "VoiceMessageID": "msg7",
      "Priority": 2,
      "AudioFileID": "beep4"
    },
    {
      "ReminderID": "healing",
      "Type": "Healing",
      "Enabled": true,
      "RelativeTrigger": "after setup of device, 30 min",
      "DisplayImage": "Image7",
      "TextMessage": "Water is getting energised",
      "VoiceMessageID": "msg8",
      "Priority": 1,
      "DisplayDurationSec": 20,
      "AudioFileID": "no_beep"
    },
    {
      "ReminderID": "dnd_block",
      "Type": "DND",
      "Enabled": true,
      "StartTime": "23:00",
      "RelativeTrigger": "if user activates",
      "TimeInterval": "7hrs",
      "SpecialWindowFrom": "19:00",
      "SpecialWindowTo": "07:00",
      "Priority": 0
    },
    {
      "ReminderID": "medication_1",
      "Type": "Medication",
      "Enabled": true,
      "TimeSlots": ["08:00", "14:00", "20:00"],
      "DisplayImage": "Image8",
      "TextMessage": "Take Medication A",
      "VoiceMessageID": "msg10",
      "Priority": 1,
      "DisplayDurationSec": 100,
      "AudioFileID": "beep5"
    },
    {
      "ReminderID": "medication_2",
      "Type": "Medication",
      "Enabled": true,
      "TimeSlots": ["08:00", "14:00", "20:00"],
      "DisplayImage": "Image8",
      "TextMessage": "Take Medication B",
      "VoiceMessageID": "msg11",
      "Priority": 1,
      "DisplayDurationSec": 100,
      "AudioFileID": "beep5"
    },
    {
      "ReminderID": "medication_3",
      "Type": "Medication",
      "Enabled": true,
      "TimeSlots": ["08:00", "14:00", "20:00"],
      "DisplayImage": "Image8",
      "TextMessage": "Take Medication C",
      "VoiceMessageID": "msg12",
      "Priority": 1,
      "DisplayDurationSec": 100,
      "AudioFileID": "beep5"
    },
    {
      "ReminderID": "medication_4",
      "Type": "Medication",
      "Enabled": true,
      "TimeSlots": ["08:00", "14:00", "20:00"],
      "DisplayImage": "Image8",
      "TextMessage": "Take Medication D",
      "VoiceMessageID": "msg13",
      "Priority": 1,
      "DisplayDurationSec": 100,
      "AudioFileID": "beep5"
    },
    {
      "ReminderID": "medication_5",
      "Type": "Medication",
      "Enabled": true,
      "TimeSlots": ["08:00", "14:00", "20:00"],
      "DisplayImage": "Image8",
      "TextMessage": "Take Medication E",
      "VoiceMessageID": "msg14",
      "Priority": 1,
      "DisplayDurationSec": 100,
      "AudioFileID": "beep5"
    },
    {
      "ReminderID": "stretch",
      "Type": "Exercise",
      "Enabled": true,
      "RelativeTrigger": "after activation, every 45 min",
      "DisplayImage": "Image9",
      "TextMessage": "Time to relax and stretch",
      "DisplayDurationSec": 120,
      "AudioFileID": "beep6"
    },
    {
      "ReminderID": "short_break",
      "Type": "Break",
      "Enabled": true,
      "RelativeTrigger": "if pomodoro enabled, every 20 min",
      "TextMessage": "Time to take a break",
      "DisplayDurationSec": 300,
      "AudioFileID": "beep2"
    },
    {
      "ReminderID": "meditation",
      "Type": "Meditation",
      "Enabled": true,
      "RelativeTrigger": "after activation, daily",
      "TimeSlots": ["06:00-07:00"],
      "DisplayImage": "Image10",
      "TextMessage": "Time to improve mental health",
      "DisplayDurationSec": 3600,
      "AudioFileID": "beep7"
    },
    {
      "ReminderID": "short_walk",
      "Type": "Exercise",
      "Enabled": true,
      "RelativeTrigger": "after activation, every 2 hrs",
      "DisplayImage": "Image11",
      "TextMessage": "Time to get up and walk",
      "DisplayDurationSec": 90,
      "AudioFileID": "beep8"
    }
  ]
}
)rawliteral";

// JSON buffer (tuned for large Frost JSON)
DynamicJsonDocument doc(24576);  
JsonArray reminders;
int currentReminder = 0;   // Tracks active reminder
bool timeSynced = false;

// ===== BLE Callback to Receive Time =====
class TimeCallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String value = pCharacteristic->getValue();   // Arduino String
    if (value.length() > 0) {
      Serial.print("Received Time via BLE: ");
      Serial.println(value);

      // Expect format: "HH:MM:SS"
      int hh, mm, ss;
      if (sscanf(value.c_str(), "%d:%d:%d", &hh, &mm, &ss) == 3) {
        struct tm tm_time = {};
        tm_time.tm_hour = hh;
        tm_time.tm_min  = mm;
        tm_time.tm_sec  = ss;
        tm_time.tm_mday = 1;
        tm_time.tm_mon  = 0;
        tm_time.tm_year = 120; // 2020 baseline (dummy)

        time_t t = mktime(&tm_time);
        struct timeval now = { .tv_sec = t };
        settimeofday(&now, NULL);

        timeSynced = true;
        Serial.println("System time updated!");
      }
    }
  }
};

void setup() {
  Serial.begin(115200);
  pinMode(IR_PIN, INPUT);

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Parse JSON
  DeserializationError error = deserializeJson(doc, jsonData);
  if (error) {
    Serial.println(F("Failed to parse JSON"));
    return;
  }
  reminders = doc["reminders"].as<JsonArray>();

  // ===== Start BLE =====
  BLEDevice::init("ESP32_TimeSync");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_WRITE
  );
  pCharacteristic->setCallbacks(new TimeCallback());
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  BLEDevice::startAdvertising();

  Serial.println("BLE Time Sync Ready. Use phone to send HH:MM:SS");

  // Show first reminder
  showReminder(currentReminder);
}

void loop() {
  int irValue = digitalRead(IR_PIN);

  if (irValue == LOW) {  // IR triggered
    Serial.println("IR Triggered!");

    // Execute current reminder
    executeReminder(currentReminder);

    // Move to next reminder (wrap at end)
    currentReminder++;
    if (currentReminder >= reminders.size()) {
      currentReminder = 0;
    }

    delay(1000);  // debounce
    showReminder(currentReminder);
  }

  // Show clock if time synced
  if (timeSynced) {
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    char buf[16];
    strftime(buf, sizeof(buf), "%H:%M:%S", &timeinfo);

    display.setCursor(0, 50);
    display.println(buf);
    display.display();
  }

  delay(500);
}

// ===== Display next reminder info =====
void showReminder(int index) {
  display.clearDisplay();
  if (index < reminders.size()) {
    JsonObject reminder = reminders[index];
    String id   = reminder["ReminderID"] | "Unknown";
    String type = reminder["Type"] | "Unknown";
    String text = reminder["TextMessage"] | "No Msg";

    display.setCursor(0, 0);
    display.println("Next Reminder:");
    display.println(type);
    display.println(text);
    display.display();

    Serial.print("Upcoming: ");
    Serial.print(id);
    Serial.print(" - ");
    Serial.println(text);
  }
}

// ===== Execute reminder =====
void executeReminder(int index) {
  if (index < reminders.size()) {
    JsonObject reminder = reminders[index];
    String id   = reminder["ReminderID"] | "Unknown";
    String type = reminder["Type"] | "Unknown";
    String text = reminder["TextMessage"] | "No Msg";

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Executing:");
    display.println(type);
    display.println(text);
    display.display();

    Serial.print("Executing: ");
    Serial.print(id);
    Serial.print(" - ");
    Serial.println(text);
  }
}
