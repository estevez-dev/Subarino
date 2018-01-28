
#include <SoftwareSerial.h>

SoftwareSerial Radio(6, 5); // RX - xbee OUT | TX - xbee IN

void setup()
{
  Serial.begin(9600);
  Radio.begin(9600);
  pinMode(8, OUTPUT);
  Serial.write("Ready");
  
}

void loop()
{
  digitalWrite(8,HIGH);
  delay(2000);
  Radio.write("Ho!");
  digitalWrite(8,LOW);
  delay(2000);
  if (Radio.available()) {
    Serial.write(Radio.read());
  }
  }

  

