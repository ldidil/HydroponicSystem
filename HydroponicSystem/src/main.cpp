#include <WiFi.h>
#include <HTTPClient.h>
#include <BH1750.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include "DHT.h"

#define APIKEY  "iQfahEflD2I1PwUS3Jsf6QxY1iyDIxta"                        //replace API key
#define DEVICE_DEV_ID "esp32@ldidil"      //replace device developer id
#define DHTTYPE DHT11


const int DHTPIN  = 4;                               // Output Pin GPIO19
DHT dht(DHTPIN, DHTTYPE);
HTTPClient http;


BH1750 lightMeter;

const int tdsPIN = 32;

//water level meter RDSM01
const int phPIN = 33;

//water level meter RDSM01
const int potPin = 34;

#define TdsSensorPin 32
#define VREF 3.3              // analog reference voltage(Volt) of the ADC
#define SCOUNT  30            // sum of sample point

float analogBufferForTDS[SCOUNT];     // store the analog value in the array, read from ADC
float analogBufferForPH[SCOUNT];     // store the analog value in the array, read from ADC
float analogBufferTemp[SCOUNT];
float analogBufferTemp2[SCOUNT];
int copyIndex = 0;

const char ssid[] = "modem";                 //replace wifi SSID 
const char password[] = "Admin4317";         //replace wifi password 

float temperature = 0;
float humidity = 0;

long previousMillis = 0;
int interval = 10000; // 30 seconds

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

int getMedianNum(float array[], int filterLen) {
  float tempArray[filterLen];
  for (int i = 0; i<filterLen; i++)
  tempArray[i] = array[i];
  int i, j, tempValue;
  for (j = 0; j < filterLen - 1; j++) {
    for (i = 0; i < filterLen - j - 1; i++) {
      if (tempArray[i] > tempArray[i + 1]) {
        tempValue = tempArray[i];
        tempArray[i] = tempArray[i + 1];
        tempArray[i + 1] = tempValue;
      }
    }
  }
  if ((filterLen & 1) > 0){
    tempValue = tempArray[(filterLen - 1) / 2];
  }  else {
    tempValue = (tempArray[filterLen / 2] + tempArray[filterLen / 2 - 1]) / 2;
  }
  return tempValue;
}

float readWaterLvl(){
  return 100-map(analogRead(potPin), 0, 4096, 0, 100);
}

int getTDS(){
  
    for(int analogBufferIndex=0; analogBufferIndex<SCOUNT; analogBufferIndex++){
      analogBufferForTDS[analogBufferIndex] = analogRead(TdsSensorPin);    //read the analog value and store into the buffer
      delay(100);
    }

    for(copyIndex=0;copyIndex<SCOUNT;copyIndex++){
        analogBufferTemp[copyIndex]= analogBufferForTDS[copyIndex];
    }
      float averageTDSVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 4096.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
      float EC=(133.42*averageTDSVoltage*averageTDSVoltage*averageTDSVoltage - 255.86*averageTDSVoltage*averageTDSVoltage + 857.39*averageTDSVoltage)*1.8;
      float ecValue25  =  EC / (1.0+0.02*(temperature-25.0));  //temperature compensation
	    int tdsValue = ecValue25 * 0.5;
      return tdsValue;
}

float getPH(){
  
    for(int analogBufferIndex=0; analogBufferIndex<SCOUNT; analogBufferIndex++){
      analogBufferForPH[analogBufferIndex] = analogRead(phPIN);    //read the analog value and store into the buffer
      delay(100);
    }

    for(copyIndex=0;copyIndex<SCOUNT;copyIndex++){
        analogBufferTemp2[copyIndex]= analogBufferForPH[copyIndex];
        Serial.println(-0.00478 * analogBufferTemp2[copyIndex] + 22.18);
    }
    
      float averagePHVoltage = getMedianNum(analogBufferTemp2,SCOUNT);
      return floor(100*(-0.00478 * averagePHVoltage + 22.18))/100;
}

void setup()
{
  Serial.begin(921600);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  //dht
  dht.begin();
  //swiatlo
  Wire.begin();
  lightMeter.begin();
}

void loop()
{
  
  if (millis() - previousMillis > interval) {
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
  
    float lightLvl = lightMeter.readLightLevel();
    int waterLvlValue = readWaterLvl();
    float phValue = getPH();
    int tdsValue = getTDS();
    
    StaticJsonDocument<200> doc;
  
    JsonObject root = doc.to<JsonObject>(); // Json Object refer to { }
    root["device_developer_id"] = DEVICE_DEV_ID;
  
    JsonObject data = root.createNestedObject("data");
    data["temperature"] = (int)temperature;
    data["humidity"] = (int)humidity;
    data["lightLvl"] = (int)lightLvl;
    data["waterLvl"] = (int)waterLvlValue;
    data["tdsValue"] = (int)tdsValue;
    data["phValue"] = phValue;
    String body;

    serializeJson(root, body);
    Serial.println(body);

  
    sendData(body);

    previousMillis = millis();
  }
}