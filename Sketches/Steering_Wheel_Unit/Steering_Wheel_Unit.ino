#include <BH1750FVI.h>

#define  HL_INTERVAL  1000UL
BH1750FVI LightSensor;

void setup()
{
  pinMode(7, OUTPUT); //parking lights
  pinMode(8, OUTPUT); //lowbeam lights
  Serial.begin(9600);
  LightSensor.begin();
  LightSensor.SetAddress(Device_Address_H);
  LightSensor.SetMode(Continuous_H_resolution_Mode);
}

void loop()
{
  char parkingLights = '0';
  char lowBeamLights = '0';
  char comH, comL, comP;
  if (Serial.available() >= 3) {
    comH = Serial.read();
    comL = Serial.read();
    comP = Serial.read();
    if (comH == 'L') {
      if (comL == 'P') {
        parkingLights = comP;
        if (comP == '0') parkingLights(false) else parkingLights(true);
      } else if (comL == 'L') {
        lowBeamLights = comP;
        if (comP == '0') lowBeamLights(false) else if (comP == '1') lowBeamLights(true);
      }
    }
  }
  //check light sensor
  if (lowBeamLights == '2') {
    uint16_t lux = LightSensor.GetLightIntensity();
    static unsigned long previousMillis = 0;
    if(millis() - previousMillis > HL_INTERVAL) {
      previousMillis = millis();
      if (lux < 100) lowBeamLights(true) else if (lux > 105) lowBeamLights(false);
    }
  }
}

void parkingLights(boolean on) {
  if (on) digitalWrite(7, HIGH); else digitalWrite(7, LOW);
}

void lowBeamLights(boolean on) {
  if (on) {
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
  } else {
    digitalWrite(8, LOW);
    if (parkingLights == '0') digitalWrite(7, LOW); //switch parking lights off if low beam was turned on without it
  }
}

