#include <Arduino.h>
#include <CircularBuffer.hpp>
#include <ArduinoJson.h>
#include <esp_now.h>
#include <WiFi.h>

#define EXPERIMENT_PIN 25

unsigned long highTime = 600;
unsigned long interval = 5000;
unsigned long pace = 5000;
unsigned long maxInterval = 120000;
unsigned long minInterval = 5000;

typedef struct struct_message {
  char json[200];
} struct_message;

struct_message message;

CircularBuffer<struct_message, 100> queue;