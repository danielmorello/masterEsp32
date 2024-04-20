#include "main.h"

void printDataOnSerial()
{
  Serial.print("Origin: ");
  Serial.println(message.origin);
  Serial.print("MAC: ");
  for (int i = 0; i < 6; i++)
  {
    Serial.printf("%02X:", message.macOrigin[i]);
  }
  Serial.println();
  Serial.print("Micros: ");
  Serial.println(message.timeMicros);
  Serial.print("Temp: ");
  Serial.println(message.coreTemp);
  Serial.print("Type: ");
  Serial.println(message.type);
  Serial.print("Experiment: ");
  Serial.println(message.experiment);
  Serial.print("Position: ");
  Serial.println(message.position);
  Serial.println("=========================");
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Delivery Success");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(5);
    digitalWrite(LED_BUILTIN, LOW);
  } else
  {
    Serial.println("Delivery Fail");
  }
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&message, incomingData, sizeof(message));
  printDataOnSerial();

  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write((byte *)&message, sizeof(message));
  Wire.endTransmission();
}

void startExperiment()
{
  digitalWrite(EXPERIMENT_PIN, HIGH);
  delay(highTime);
  digitalWrite(EXPERIMENT_PIN, LOW);
}

void setup() {
  pinMode(EXPERIMENT_PIN, OUTPUT);

  Wire.begin(SDA_PIN, SCL_PIN, 100000);
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");

    return;
  }
  Serial.println("ESPNOW OK");

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println(numPeers);

  for (size_t i = 0; i < numPeers; i++) {
    memcpy(peerInfo.peer_addr, slaveAddresses[i], 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false; 
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      Serial.println("Failed to add peer");
      return;
    }
    delay(200);
  }

}
 
void loop() {
  delay(10000);
  masterMessage.restart = true;
  for (size_t i = 0; i < numPeers; i++) {
    esp_err_t result = esp_now_send(slaveAddresses[i], (uint8_t *) &masterMessage, sizeof(masterMessage));
    delay(200);
  }
  delay(10000);

  Serial.println("==================================");
  Serial.println("=========START EXPERIMENT=========");
  Serial.println("==================================");

  while (interval <= maxInterval) {
    delay(1000);
    Serial.println("==================================");
    Serial.print("=========INTERVAL ");
    Serial.print(interval);
    Serial.println("=============");
    Serial.println("==================================");
    startExperiment();
    delay(interval - highTime);
    startExperiment();
    interval = interval + pace;
  }
  
  Serial.println("==================================");
  Serial.println("===========END EXPERIMENT=========");
  Serial.println("==================================");

  interval = 5000;
}