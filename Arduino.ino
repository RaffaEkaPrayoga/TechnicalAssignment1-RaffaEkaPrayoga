#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>
#define DHT11PIN 23

WiFiClient wifi;
DHT dht(DHT11PIN, DHT11);

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin("Hospot Ranz", "Ranz1245");  // Sesuaikan SSID dan Password WiFi

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi is Connected!");
}

void loop() {
  HTTPClient http;
  http.begin("http://192.168.65.26:5000/sensor/data");  // Ganti dengan alamat IP lokal Anda
  http.addHeader("Content-Type", "application/json");

  float hum = dht.readHumidity();
  float temp = dht.readTemperature();
  long timestamp = millis();

  String data = "{"
                "\"humidity\": " + String(hum, 2) + ","
                "\"temperature\": " + String(temp, 2) + ","
                "\"timestamp\": " + String(timestamp) +
                "}";

  int httpCode = http.POST(data);

  Serial.println(data);
  if (httpCode > 0) { 
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("Data successfully sent");
    } else {
      Serial.println("Unexpected HTTP code: " + String(httpCode));
    }
  } else {
    Serial.println("Error on sending POST: " + String(http.errorToString(httpCode).c_str()));
  }

  http.end();
  delay(5000);
}
