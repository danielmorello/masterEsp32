#include <Arduino.h>
#include <CircularBuffer.hpp>
#include <ArduinoJson.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>

#define EXPERIMENT_PIN 23 // pino do pulso para o início/término da contagem

#define SDA_PIN 18 // pino para I2C comm
#define SCL_PIN 19 // pino para I2C comm
#define SLAVE_ADDR 8 // Endereço I2C do escravo

unsigned long highTime = 600; // tempo em alto do pulso para início/término da contagem
unsigned long interval = 5000; // primeiro intervalo de tempo entre início e fim da contagem
unsigned long pace = 5000; // paço de incremento do intervalo
unsigned long maxInterval = 300000; // tempo máximo do intervalo entre início e término da contagem

uint8_t slaveAddresses[][6] = {
  // { 0x48, 0xE7, 0x29, 0xC9, 0x2B, 0x58 }, // 1
  // { 0xA8, 0x42, 0xE3, 0x59, 0x83, 0x38 }, // 2
  { 0x48, 0xE7, 0x29, 0xC9, 0xED, 0x00 }, // 3
  { 0x48, 0xE7, 0x29, 0xCA, 0x15, 0xD0 }, // 4
  { 0xC8, 0xF0, 0x9E, 0xF5, 0x17, 0x94 }, // 5
  { 0xC8, 0xF0, 0x9E, 0xF8, 0x72, 0x70 }, // 6
  { 0xC8, 0xF0, 0x9E, 0xF7, 0x61, 0x98 }, // 7
  { 0x7C, 0x9E, 0xBD, 0x3A, 0x01, 0x04 }, // 8
  { 0xC8, 0xF0, 0x9E, 0xF8, 0x6A, 0x98 }, // 9
  { 0xC8, 0xF0, 0x9E, 0xF7, 0x69, 0x98 } // 10
};

const int numPeers = sizeof(slaveAddresses) / sizeof(slaveAddresses[0]);

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