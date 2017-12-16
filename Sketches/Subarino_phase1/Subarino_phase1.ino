#include <SoftwareSerial.h>
#include <Wire.h>
#include <BH1750FVI.h>

SoftwareSerial BTSerial(3, 2); // RX | TX
String reader;
BH1750FVI LightSensor;
boolean hlAuto = true;
#define  HL_INTERVAL  1000UL

void setup()
{
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  Serial.begin(9600);
  
  BTSerial.begin(9600);  // HC-05 default speed in AT command more
  LightSensor.begin();
  LightSensor.SetAddress(Device_Address_H);
  LightSensor.SetMode(Continuous_H_resolution_Mode);
  Serial.println("Ready:");
}

void loop()
{
  uint16_t lux = LightSensor.GetLightIntensity();
  reader="";
  int command = 0;
  while (BTSerial.available()) {
    delay(3);
    if (BTSerial.available()>0) {
      char c = BTSerial.read();
      reader += c;
    
    }
  }
  if (reader.length() > 0) {
    Serial.println("Command: "+reader+"");
    command = reader.toInt();
    if (command == 11) {
      digitalWrite(8, HIGH);
      digitalWrite(9, HIGH);
    } else if (command == 12) {
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
    } else if (command == 13) {
      hlAuto = true;
    } else if (command == 14) {
      hlAuto = false;
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);  
    }    
  }

  if (hlAuto == true) {
    static unsigned long previousMillis = 0;
    if(millis() - previousMillis > HL_INTERVAL) {
      previousMillis = millis();
      
      if (lux < 100) {
        digitalWrite(8, HIGH);
        digitalWrite(9, HIGH);
      } else if (lux > 105) {
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
      }
    }
  }
 
  
  
}
