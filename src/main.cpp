#include <Arduino.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
 
const char* ssid = "<YourWiFiName>";
const char* password =  "<YourWiFiNamePassword>";

void notFound(AsyncWebServerRequest * request);
 
AsyncWebServer server(80);
 
void setup(){
  Serial.begin(115200);

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to WiFi Successfully.");
  Serial.println("IP-adress:");
  Serial.println(WiFi.localIP());

  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occured while mounting SPIFFS");
    return;
  }

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Method", "GET, POST, PUT");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");
 
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Test route");
  });

  server.onNotFound(notFound);
 
  server.begin();
}
 
void loop(){}

void notFound(AsyncWebServerRequest * request) {
  if (request->method() == HTTP_OPTIONS) {
    request->send(200);
  } else {
    request->send(404, "application/json", "{\"message\":\"Not found\"}");
  }
}