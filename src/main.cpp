#include "main.h"

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&message, incomingData, sizeof(message));
  queue.unshift(message);
  JsonDocument doc;
  deserializeJson(doc, message.json);
  serializeJsonPretty(doc, Serial);
  Serial.println();
}

void startExperiment() {
  digitalWrite(EXPERIMENT_PIN, HIGH);
  delay(highTime);
  digitalWrite(EXPERIMENT_PIN, LOW);
}
 
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");

    return;
  }
  Serial.println("ESPNOW OK");

  esp_now_register_recv_cb(OnDataRecv);
  pinMode(EXPERIMENT_PIN, OUTPUT);
}
 
void loop() {
  delay(interval - highTime);
  startExperiment();
  interval = interval + pace;
  if (interval > maxInterval) {
    interval = minInterval;
  }
}