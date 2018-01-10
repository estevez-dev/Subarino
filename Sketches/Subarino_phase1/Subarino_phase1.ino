#include <SD.h>

#include <SoftwareSerial.h>
#include <Wire.h>
#include <BH1750FVI.h>

#define  HL_INTERVAL  1000UL

SoftwareSerial BTSerial(3, 2); // RX | TX
BH1750FVI LightSensor;
boolean lowbeamAuto = false;
char parkingLights = '0';
char lowbeamLights = '0';
//char autoHL;

void setup()
{
  pinMode(8, OUTPUT); //parking lights
  pinMode(9, OUTPUT); //lowbeam lights
  Serial.begin(9600);

  Serial.print("Bluetooth initialization...");
  BTSerial.begin(9600);  // HC-05 default speed in AT command more
  Serial.println("Done.");
  Serial.print("Light Sensor initialization...");
  LightSensor.begin();
  LightSensor.SetAddress(Device_Address_H);
  LightSensor.SetMode(Continuous_H_resolution_Mode);
  Serial.println("Done.");
  Serial.print("SD Card initialization...");
  if (!SD.begin(4)) {
    Serial.println("Failed");
  } else {
    File settings;
    Serial.println("Done.");
    Serial.print("Checking settings file...");
    if (SD.exists("settings.sub")) {
      Serial.println("Ok.");
    } else {
      Serial.print("No settings file. Creating...");
      settings = SD.open("settings.sub", FILE_WRITE);
      settings.close();
      if (SD.exists("settings.sub")) {
        Serial.println("Done.");
      } else {
        Serial.println("Failed.");
      }
    }
    Serial.println("Reading and applying settings...");
    settings = SD.open("settings.sub");
    if (settings.available() > 0) {
      parkingLights = settings.read();
      lowbeamLights = settings.read();
      char autoHL = settings.read();
      Serial.println("parkingLights="+String(parkingLights));
      Serial.println("lowbeamLights="+String(lowbeamLights));
      Serial.println("lowbeamAuto="+String(lowbeamAuto));
      if (parkingLights == '1') digitalWrite(8, HIGH);
      if (lowbeamLights == '1') digitalWrite(9, HIGH);
      if (autoHL == '1') lowbeamAuto = true;
      Serial.println("Done.");        
    } else {
      Serial.println("Failed.");
    }
    settings.close();
  }
  Serial.println("Setup finished.");
}

void loop()
{
  uint16_t lux = LightSensor.GetLightIntensity();
  char comUnit = 'z';
  char comFunc = '9';
  File settings;
  
  if (BTSerial.available() == 2) {
    comUnit = BTSerial.read();
    comFunc = BTSerial.read();
    Serial.println("Recaved: "+String(comUnit)+String(comFunc));
    if (comUnit == 'L') {
      if (comFunc == '0') {
        parkingLights = '1';
        digitalWrite(8, HIGH);
      } else if (comFunc == '1') {
        parkingLights = '0';
        digitalWrite(8, LOW);
      } else if (comFunc == '2') {
        lowbeamLights = '1';
        digitalWrite(9, HIGH);
      } else if (comFunc == '3') {
        lowbeamLights = '0';
        digitalWrite(9, LOW);
      } else if (comFunc == '8') {
        lowbeamAuto = true;
      } else if (comFunc == '9') {
        lowbeamAuto = false;
        digitalWrite(8, LOW);
        parkingLights = '0';
        lowbeamLights = '0';
        digitalWrite(9, LOW);
        BTSerial.print(String(parkingLights) + String(lowbeamLights) + String('0'));
      }    
    } else if (comUnit == 'S') {
      BTSerial.print(String(parkingLights) + String(lowbeamLights));
      if (lowbeamAuto) BTSerial.print('1'); else  BTSerial.print('0');
    }
    SD.remove("settings.sub");
    settings = SD.open("settings.sub", FILE_WRITE);
    settings.print(String(parkingLights) + String(lowbeamLights));
    if (lowbeamAuto) settings.print('1'); else  settings.print('0');
    settings.close();
  }

  if (lowbeamAuto == true) {
    static unsigned long previousMillis = 0;
    if(millis() - previousMillis > HL_INTERVAL) {
      previousMillis = millis();
      //Serial.println(String(lux));
      if (lux < 100) {
        lowbeamLights = '1';
        parkingLights = '1';
        digitalWrite(8, HIGH);
        digitalWrite(9, HIGH);
        BTSerial.print(String(parkingLights) + String(lowbeamLights) + String('1'));
      } else if (lux > 105) {
        lowbeamLights = '0';
        parkingLights = '0';
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
        BTSerial.print(String(parkingLights) + String(lowbeamLights) + String('1'));
      }
    }
  }
}
