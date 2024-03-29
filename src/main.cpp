#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <WiFi.h>

const char* ssid = "REDRE";
const char* password = "SENHA";

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  char json[200];
} struct_message;

// Create a struct_message called message
struct_message message;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&message, incomingData, sizeof(message));
  JsonDocument doc;
  deserializeJson(doc, message.json);
  serializeJsonPretty(doc, Serial);
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_AP_STA);

  // WiFi.begin(ssid, password);
  // Serial.print("Conectando ao Wi-Fi");
  // while (WiFi.status() != WL_CONNECTED) {
  //   Serial.print(".");
  //   delay(1000);
  // }
  // Serial.println("\nConectado ao Wi-Fi");
  // Serial.println(WiFi.channel());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Serial.println("ESPNOW OK");

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
}