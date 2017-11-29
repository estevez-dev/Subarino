#include <Wire.h>
#include <BH1750FVI.h>

BH1750FVI LightSensor;

void setup()
{
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.begin(9600);
  Serial.println("Setting up...");
  LightSensor.begin();

  LightSensor.SetAddress(Device_Address_H);
  
  LightSensor.SetMode(Continuous_H_resolution_Mode);
  Serial.println("Ready");
}

void loop() {
  uint16_t lux = LightSensor.GetLightIntensity();
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  digitalWrite(7, LOW);
  if (lux > 10) digitalWrite(12, HIGH);
  if (lux > 20) digitalWrite(11, HIGH);
  if (lux > 50) digitalWrite(10, HIGH);
  if (lux > 100) digitalWrite(9, HIGH);
  if (lux > 150) digitalWrite(8, HIGH);
  if (lux > 300) digitalWrite(7, HIGH);
  delay(3);
}
