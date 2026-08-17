#include "Arduino.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <WiFi.h>

// change the clock speed to reduce power

int ledpin = 3; 
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  10        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;  
int awakeTime = 10000; //awake for 10 total seconds (including serial loading time)

void setup() {
  WiFi.mode(WIFI_OFF); // not using wifi
  //btStop(); // not using bluetooth
  //setCpuFrequencyMhz(10);

  // remove serial debugging in final code to reduce power
  Serial.begin(115200); 
  delay(3000); // time to open serial monitor 
  Serial.println("im awake");

  pinMode(ledpin, OUTPUT);
  digitalWrite(ledpin, HIGH);  // LED ON while awake

  ++bootCount; //increment number of times it has booted up
  Serial.print("Boot count = "); //print number of times it has booted up
  Serial.println(bootCount);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR); //wakes up board after specified time
}


void loop() {
  //compare current time (time since last reset) to time it needs to be awake 
  int currentTime = millis();
  if(currentTime >= awakeTime)
  {
    digitalWrite(ledpin, LOW);
    Serial.println("Going to sleep");
    Serial.flush(); //remove memory
    esp_deep_sleep_start(); //go to sleep

  }
}