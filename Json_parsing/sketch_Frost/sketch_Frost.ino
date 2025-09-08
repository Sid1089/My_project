#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

const char* ssid     = "YOUR_WIFI";
const char* password = "YOUR_PASS";

// NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); // 19800 = +5:30 IST offset

void setup() {
  Serial.begin(115200);

  // Connect WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // Init NTP
  timeClient.begin();
  timeClient.update();

  Serial.println("Current time: " + timeClient.getFormattedTime());

  // Example JSON (from your sheet)
  const char* jsonData = R"rawliteral(
  {
    "reminders": [
      {
        "ReminderID": "medication_1",
        "TimeSlots": ["08:00","14:00","20:00"],
        "TextMessage": "Take Medication A"
      },
      {
        "ReminderID": "hydration",
        "StartTime": "3HRS",
        "TextMessage": "Time to hydrate!"
      }
    ]
  })rawliteral";

  StaticJsonDocument<1024> doc;
  deserializeJson(doc, jsonData);

  JsonArray reminders = doc["reminders"];

  // Loop through all reminders
  for (JsonObject r : reminders) {
    const char* id = r["ReminderID"];
    const char* msg = r["TextMessage"];

    Serial.print("\nReminder: ");
    Serial.println(id);
    Serial.print("Message: ");
    Serial.println(msg);

    // Case 1: TimeSlots
    if (r.containsKey("TimeSlots")) {
      JsonArray slots = r["TimeSlots"];
      for (const char* slot : slots) {
        Serial.print("Scheduled at: ");
        Serial.println(slot);

        // Convert "HH:MM" to seconds
        int hr, min;
        sscanf(slot, "%d:%d", &hr, &min);
        int nowHr = timeClient.getHours();
        int nowMin = timeClient.getMinutes();

        int nowSec = nowHr * 3600 + nowMin * 60;
        int targetSec = hr * 3600 + min * 60;
        int delaySec = targetSec - nowSec;

        if (delaySec < 0) delaySec += 24 * 3600; // next day

        Serial.print("Trigger in (sec): ");
        Serial.println(delaySec);
      }
    }

    // Case 2: StartTime (relative)
    if (r.containsKey("StartTime")) {
      unsigned long ms = 0;
      String t = r["StartTime"].as<String>();
      if (t.endsWith("HRS")) ms = t.toInt() * 3600000UL;
      if (t.endsWith("m"))   ms = t.toInt() * 60000UL;

      Serial.print("Relative Start: ");
      Serial.println(ms / 1000);
    }
  }
}

void loop() {
  // Nothing yet — you’d add alarms/timers here
}
