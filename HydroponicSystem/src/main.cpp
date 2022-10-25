#include <Arduino.h>
#include <WiFiMulti.h>
#include <WebSocketsClient.h>

#define WIFI_SSID ""
#define WIFI_PASS ""

WiFiMulti wiFiMulti;
WebSocketsClient wsClient;

#define WS_HOST ""
#define WS_PORT 443
#define  WS_URL ""

void diodBlink(){
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
}

void handleMessage(uint8_t * payload){

}
void onWSEvent(WStype_t type, uint8_t * payload, size_t length){
  switch (type){
  case WStype_CONNECTED:
    Serial.println("WS conntected");
    break;
    case WStype_DISCONNECTED:
   Serial.println("WS disconntected");
    break;
    case WStype_TEXT: //client give us msg from server
    Serial.printf("WS msg: %s\n",payload);
    handleMessage(payload);


    break;
  default:
    break;
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(921600);
  Serial.println("Hello from the setup");

  wiFiMulti.addAP(WIFI_SSID,WIFI_PASS);
  while (wiFiMulti.run() != WL_CONNECTED){
    delay(500);
  }
  Serial.println("Conected");
  wsClient.beginSSL(WS_HOST, WS_PORT, WS_URL, "", "wss");
  wsClient.onEvent(onWSEvent);
}

void loop() {
  diodBlink();
  wsClient.loop(); //keep the connection alive

}