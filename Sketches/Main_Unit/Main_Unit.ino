#include <SD.h>

#include <SoftwareSerial.h>
#include <Wire.h>
#include <BH1750FVI.h>

#define  HL_INTERVAL  1000UL

SoftwareSerial BTSerial(3, 2); // RX | TX
SoftwareSerial Radio(6, 5);
BH1750FVI LightSensor;
char lowbeamAuto = '0';
char parkingLights = '0';
char lowbeamLights = '0';

void setup()
{
  Serial.begin(9600);

  Serial.print("Bluetooth initialization...");
  BTSerial.begin(9600);
  Serial.println("Done.");
  Serial.print("Radio initialization...");
  Radio.begin(9600);
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
  char comH = 'z';
  char comL = '9';
  char comF = '9';
  File settings;

  BTSerial.listen();
  if (BTSerial.available() == 3) {
    comH = BTSerial.read();
    comL = BTSerial.read();
    comF = BTSerial.read();
    Serial.println("Recaved: "+String(comH)+String(comL)+String(comF));
    if (comH == 'L') {
      if (comL == 'P') {
        parkingLights = comF;
        radioSend("LP"+String(comF));
      } else if (comL == 'L') {
        lowbeamLights = comF;
        radioSend("LL"+String(comF));
      } else if (comL == 'A') {
        lowbeamAuto = comF;  
      } else if (comL == 'S') {
        BTSerial.print(String(parkingLights) + String(lowbeamLights) + String(lowbeamAuto));  
      }
    }
    SD.remove("settings.sub");
    settings = SD.open("settings.sub", FILE_WRITE);
    settings.print(String(parkingLights) + String(lowbeamLights) + String(lowbeamAuto));
    settings.close();
  }

  if (lowbeamAuto == '1') {
    static unsigned long previousMillis = 0;
    if(millis() - previousMillis > HL_INTERVAL) {
      previousMillis = millis();
      if (lux < 100) {
        lowbeamLights = '1';
        parkingLights = '1';
        radioSend("LP1");
        radioSend("LL1");
      } else if (lux > 105) {
        lowbeamLights = '0';
        parkingLights = '0';
        radioSend("LP0");
        radioSend("LL0");
      }
      BTSerial.print(String(parkingLights) + String(lowbeamLights) + String('1'));
    }
  }

  
}

void radioSend(String comand){
  Radio.print(comand);
}
