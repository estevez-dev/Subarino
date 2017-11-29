#include <SoftwareSerial.h>

SoftwareSerial BTSerial(3, 2); // RX | TX

void setup()
{
  Serial.begin(9600);
  Serial.println("Ready:");
  BTSerial.begin(38400);  // HC-05 default speed in AT command more
}

void loop()
{
  if (BTSerial.available())
    Serial.write(BTSerial.read());

  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available())
    BTSerial.write(Serial.read());
}
