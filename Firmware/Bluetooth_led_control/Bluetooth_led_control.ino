#include <SoftwareSerial.h>

SoftwareSerial BTSerial(3, 2); // RX | TX
int d = 1000;
String reader;
int currentPin = 8;

void setup()
{
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
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
 
  if (d == 0) {
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH);
    delay(600);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);
    digitalWrite(4, LOW);
    digitalWrite(3, LOW);
    delay(300);  
  } else {
    digitalWrite(8, HIGH);
    delay(d);
    digitalWrite(8, LOW);
    digitalWrite(7, HIGH);
    delay(d);
    digitalWrite(7, LOW);
    digitalWrite(6, HIGH);
    delay(d);
    digitalWrite(6, LOW);
    digitalWrite(5, HIGH);
    delay(d);
    digitalWrite(5, LOW);
    digitalWrite(4, HIGH);
    delay(d);
    digitalWrite(4, LOW);
    digitalWrite(3, HIGH);
    delay(d);
    digitalWrite(3, LOW);
  }
  
}
