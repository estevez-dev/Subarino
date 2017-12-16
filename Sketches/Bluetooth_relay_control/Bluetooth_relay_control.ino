#include <SoftwareSerial.h>

SoftwareSerial BTSerial(3, 2); // RX | TX
int d = 1000;
String reader;
int currentPin = 8;

void setup()
{
  pinMode(8, OUTPUT);
  Serial.begin(9600);
  Serial.println("Ready:");
  BTSerial.begin(9600);  // HC-05 default speed in AT command more
}

void loop()
{
  reader="";
  while (BTSerial.available()) {
    delay(3);
    if (BTSerial.available()>0) {
      char c = BTSerial.read();
      reader += c;
        
    }
  }
  if (reader.length() > 0) {
    Serial.println("Trying to change dalay to "+reader+"");
    d = reader.toInt();    
  }
 
  if (d == 11) {
    digitalWrite(8, HIGH);
  
  } else if (d == 12) {
    digitalWrite(8, LOW);
  }
  
}
