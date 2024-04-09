#include <Arduino.h>
#include <CircularBuffer.hpp>
#include <ArduinoJson.h>
#include <esp_now.h>
#include <WiFi.h>

#define EXPERIMENT_PIN 25

unsigned long highTime = 600;
unsigned long interval = 5000;
unsigned long pace = 5000;
unsigned long maxInterval = 300000;
unsigned long minInterval = 5000;

const int numPeers = 4;

uint8_t slaveAddresses[numPeers][6] = {
  { 0xCC, 0x50, 0xE3, 0x96, 0x26, 0x08 },
  { 0x24, 0x6F, 0x28, 0x79, 0xD7, 0xD8 },
  { 0xAC, 0x67, 0xB2, 0x36, 0x1D, 0x64 },
  { 0xAC, 0x67, 0xB2, 0x36, 0x1F, 0xDC }
};
esp_now_peer_info_t peerInfo;

typedef struct MasterMessage {
  bool restart;
} MasterMessage;

MasterMessage masterMessage;

typedef struct Message {
  int origin;
  uint8_t macOrigin[6];
  unsigned long timeMicros;
  float coreTemp;
  int type;
  unsigned long experiment;
  int position;
} Message;

Message message;

CircularBuffer<Message, 100> queue;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);