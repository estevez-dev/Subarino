#include <SD.h>

#include <SoftwareSerial.h>
#include <Wire.h>
#include <BH1750FVI.h>

//SoftwareSerial Radio(6, 7); // RX | TX

void setup()
{
  pinMode(8, OUTPUT); //parking lights
  pinMode(9, OUTPUT); //lowbeam lights
  Serial.begin(9600);

  //Serial.print("Radio initialization...");
  //Radio.begin(9600);  
  //Serial.println("Done.");
  //Serial.println("Setup finished.");
  digitalWrite(8, LOW);
  delay(300);
  digitalWrite(8, HIGH);
}

void loop()
{
  char parkingLights = '0';
  if (Serial.available()) {
    parkingLights = Serial.read();
    digitalWrite(8, LOW);
    delay(100);
    digitalWrite(8, HIGH);
    delay(100);
        
    
    //char lowbeamLights = Serial.read();
    //char lowbeamAuto = Serial.read();
    //Serial.println("Recaved: "+String(parkingLights)+String(lowbeamLights)+String(lowbeamAuto));
  }
}
