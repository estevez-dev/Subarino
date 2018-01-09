#include <SoftwareSerial.h>
#include <Wire.h>
#include <BH1750FVI.h>

SoftwareSerial BTSerial(3, 2); // RX | TX
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
  char comUnit = 'z';
  char comFunc = '9';

  if (BTSerial.available() == 2) {
    comUnit = BTSerial.read();
    comFunc = BTSerial.read();
    Serial.println("Recaved: "+String(comUnit)+String(comFunc));
    if (comUnit == 'L') {
      if (comFunc == '0') {
        digitalWrite(8, HIGH);
      } else if (comFunc == '1') {
        digitalWrite(8, LOW);
      } else if (comFunc == '2') {
        digitalWrite(9, HIGH);
      } else if (comFunc == '3') {
        digitalWrite(9, LOW);
      } else if (comFunc == '8') {
        hlAuto = true;
      } else if (comFunc == '9') {
        hlAuto = false;
      }    
    }
  }

  if (hlAuto == true) {
    static unsigned long previousMillis = 0;
    if(millis() - previousMillis > HL_INTERVAL) {
      previousMillis = millis();
      Serial.println(String(lux));
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
