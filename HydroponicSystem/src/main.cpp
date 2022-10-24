#include <Arduino.h>
#include <WiFiMulti.h>

#define WIFI_SSID ""
#define WIFI_PASS ""

WiFiMulti wiFiMulti;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(921600);
  Serial.println("Hello from the setup");

  wiFiMulti.addAP(WIFI_SSID,WIFI_PASS);
  while (wiFiMulti.run() != WL_CONNECTED){
    delay(500);
  }

  Serial.println("Conected");
}

void loop() {
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
}