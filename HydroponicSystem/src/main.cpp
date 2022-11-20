#include <WiFi.h>
#include <HTTPClient.h>
#include <BH1750.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <SimpleDHT.h>

#define APIKEY  "Ax3bO9E449RPlqPuRExiRkuygLHWkmT7"                        //replace API key
#define DEVICE_DEV_ID "deviceDefault@ldidil"      //replace device developer id


const int SEN_PIN = 4;                               // Output Pin GPIO19
SimpleDHT11 dht22(SEN_PIN);
HTTPClient http;


BH1750 lightMeter;

//water level meter RDSM01
const int potPin = 34;

const char ssid[] = "modem";                 //replace wifi SSID 
const char password[] = "Admin4317";         //replace wifi password 

float temperature = 0;
float humidity = 0;
long previousMillis = 0;
int interval = 5000; // 5 seconds

void sendData(String body)
{
  http.begin("http://apiv2.favoriot.com/v2/streams");
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Apikey", APIKEY);
  
    int httpCode = http.POST(body);
    if (httpCode > 0) {
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    }
    else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
}


void setup()
{
  Serial.begin(921600);

  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  //swiatlo
  Wire.begin();
  lightMeter.begin();

  //water level
}

void loop()
{
  if (millis() - previousMillis > interval) {
    int err = SimpleDHTErrSuccess;
    if ((err = dht22.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      Serial.print("Read DHT22 failed, err="); Serial.println(err);
      delay(1000);
      return;
    }
  
    float lightLvl = lightMeter.readLightLevel();
    int waterLvlValue = analogRead(potPin);
    Serial.print("Sample OK: ");
    Serial.print((float)temperature);
    Serial.print(" *C, ");
    Serial.print((float)humidity);
    Serial.println(" RH%");
    Serial.print(lightLvl);
    Serial.println(" RH%");
    
    StaticJsonDocument<200> doc;
  
    JsonObject root = doc.to<JsonObject>(); // Json Object refer to { }
    root["device_developer_id"] = DEVICE_DEV_ID;
  
    JsonObject data = root.createNestedObject("data");
    data["temperature"] = (int)temperature;
    data["humidity"] = (int)humidity;
    data["lightLvl"] = (int)lightLvl;
    data["waterLvl"] = (int)waterLvlValue;
  
    String body;

    serializeJson(root, body);
    Serial.println(body);

  
    sendData(body);

    previousMillis = millis();
  }
}