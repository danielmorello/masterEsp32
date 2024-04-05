#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <WiFi.h>

typedef struct struct_message {
  char json[200];
} struct_message;

struct_message message;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&message, incomingData, sizeof(message));
  JsonDocument doc;
  deserializeJson(doc, message.json);
  serializeJsonPretty(doc, Serial);
  Serial.println();
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
}
 
void loop() {
}