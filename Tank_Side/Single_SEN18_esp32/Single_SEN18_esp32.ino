int sen_val = 0;  // Initialize value
const int s_pin = 34; // sensor GPIO pin used
  
void setup() { 
 
  // start the serial console
  Serial.begin(9600);
} 
  
void loop() { 
   
  sen_val = analogRead(s_pin); //Read data from analog pin and store it to sen_val variable
   
  if (sen_val<=100){
    Serial.print("Water Level: Empty");
    }
    else if (sen_val>100 && sen_val<=300){
      Serial.print("Water Level: Low"); 
      } 
      else if (sen_val>300 && sen_val<=330){
        Serial.print("Water Level: Medium");
        }
else if (sen_val>330){ 
    Serial.print("Water Level: High"); 
  }
  delay(1000); 
}
