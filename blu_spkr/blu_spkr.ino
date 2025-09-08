#include <Arduino.h>
#include <AudioTools.h>
#include <BluetoothA2DPSink.h>

// Create I2S output stream
I2SStream i2s;
BluetoothA2DPSink a2dp_sink(i2s);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Bluetooth Speaker...");

  // Configure I2S pins and sample rate
  auto cfg = i2s.defaultConfig();
  cfg.pin_bck  = 27;   // Bit Clock
  cfg.pin_ws   = 14;   // Word Select (LRC)
  cfg.pin_data = 26;   // Data in (DIN)
  cfg.sample_rate = 44100;
  i2s.begin(cfg);

  // Start A2DP Bluetooth sink
  a2dp_sink.set_volume(64);  // Volume range: 0-128
  a2dp_sink.start("ESP32_I2S_BT_SPKR");

  Serial.println("Bluetooth Speaker is Ready!");
}

void loop() {
  // Optional: Debug current volume
  Serial.print("Volume: ");
  Serial.println(a2dp_sink.get_volume());
  delay(2000);
}
