#include <Arduino.h>

#define RXD2 16
#define TXD2 17

void setup() 
{
  Serial.begin(115200);


  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  
  Serial.println("test");

}

void loop() { 
  while (Serial2.available()) 
  {
    Serial.print(char(Serial2.read()));
  }
}