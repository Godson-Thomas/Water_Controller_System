#include "WiFi.h"
#include "ESPAsyncWebServer.h"

// Set your access point network credentials
const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const int s_pin1 = 36; // sensor GPIO pin bottom
const int s_pin2 = 39;
const int s_pin3 = 34;
const int s_pin4 = 35;
const int s_pin5 = 32;
const int s_pin6 = 33;
const int s_pin7 = 25;
const int s_pin8 = 26;
const int s_pin9 = 27;
const int s_pin10 = 14; // sensor GPIO pin Top


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
    request->send_P(200, "text/plain", process().c_str());
  });
 
 
  
  // Start server
  server.begin();
}

void loop(){
  
}

String process(){
int a=analogRead(s_pin1);
int b=analogRead(s_pin2);
int c=analogRead(s_pin3);
int d=analogRead(s_pin4);
int e=analogRead(s_pin5);
int f=analogRead(s_pin6);
int g=analogRead(s_pin7);
int h=analogRead(s_pin8);
int i=analogRead(s_pin9);
int j=analogRead(s_pin10);
int avg=(a+b+c+d+e+f+g+h+i+j)/10; // for a single sensor if a >330 ==> fully immersed
                                                     //  if a >300 && a<=300 ==> Partially immersed
                                                     //if a>100 && a<=300 ==>Low   
if( (avg>330)){
  return String("high");
}
 if (avg>100 && avg<=300){

return String("low");
  
}
if (avg>300 && avg<=300){
  return String("med");
}
}
