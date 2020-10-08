#include "WiFi.h"
#include "ESPAsyncWebServer.h"

// Set your access point network credentials
const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const int s_pin = 34; // sensor GPIO pin used

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/sen_value", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(analogRead(s_pin)).c_str());
  });
 
 
  
  // Start server
  server.begin();
}

void loop(){
  
}
