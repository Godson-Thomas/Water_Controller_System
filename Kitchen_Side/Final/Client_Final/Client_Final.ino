#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
// Wiring: SDA pin is connected to GPIO 21 and SCL pin to GPIO 22 .
// Connect to LCD via I2C, default address 0x27 
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,16,2) for 16x2 LCD.

const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

//Your IP address or domain name with URL path
const char* serverNameval = "http://192.168.4.1/sen_value";

String inc_s_value;

unsigned long previousMillis = 0;
const long interval = 5000; 
const int hi = 2;//GPIO2
const int me = 5;// GPIO5
const int lo = 15;//GPIO15
const int rel = 16;// GPIO16
TaskHandle_t Task1;
TaskHandle_t Task2;
void setup() {
  Serial.begin(115200);
   pinMode (hi, OUTPUT);
   pinMode (me, OUTPUT);
   pinMode (lo, OUTPUT);
   pinMode (rel,OUTPUT);



 xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500);

  
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}


//Task1code: Relay
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

 while(inc_s_value != "high"){
    digitalWrite(rel, HIGH);
    delay(700);
  }
}


//Task2code:LED_LCD_Data_Fetch

void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    digitalWrite (hi, LOW);  // turn off the LED
    digitalWrite (me, LOW);  // turn off the LED
    digitalWrite (lo, LOW);  // turn off the LED
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis >= interval) {
     // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED ){ 
      inc_s_value = httpGETRequest(serverNameval);
     
      
      Serial.print(inc_s_value);
      Serial.print("\n");
      

       
      // save the last HTTP GET Request
      previousMillis = currentMillis;

       if (inc_s_value=="high"){
 digitalWrite (hi, HIGH);  // turn on the LED
 lcd.setCursor(0, 0); // Set the cursor on the first column and first row.
 lcd.print("Level: "); // Print the string "
 lcd.setCursor(8,0);
 lcd.print("High");
lcd.clear();
        
      }
       if(inc_s_value=="med"){
 digitalWrite (me, HIGH); 
 lcd.setCursor(0, 0); // Set the cursor on the first column and first row.
 lcd.print("Level: "); // Print the string "
 lcd.setCursor(8,0);
 lcd.print("Medium");
lcd.clear();
      }
      if(inc_s_value=="low")
      {
 digitalWrite (lo, HIGH); 
 lcd.setCursor(0, 0); // Set the cursor on the first column and first row.
 lcd.print("Level: "); // Print the string "
 lcd.setCursor(8,0);
 lcd.print("Low");
lcd.clear();
      }
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }
  }
}



void loop() {
   
}

String httpGETRequest(const char* serverName) {
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
