#include <XBee.h>

#include <SD.h>

#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial BTSerial(3, 2); // RX | TX
SoftwareSerial XBeeSerial(6, 5);
XBee xbee = XBee();

char lowbeamAuto = '0';
char parkingLights = '0';
char lowbeamLights = '0';

void setup()
{
  Serial.begin(9600);

  Serial.print("Bluetooth initialization...");
  BTSerial.begin(9600);
  Serial.println("Done.");
  Serial.print("XBee initialization...");
  XBeeSerial.begin(9600);
  xbee.setSerial(XBeeSerial);
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
      /*Serial.println("Reading and applying settings...");
      settings = SD.open("settings.sub");
      while (settings.available() > 0) {
        char comH, comL, comP;
        comH = settings.read();
        comL = settings.read();
        comP = settings.read();
        radioSend(String(comH) + String(comL) + String(comP));
        Serial.println(String(comH) + String(comL) + String(comP));
      }
      Serial.println("Done.");
      settings.close();*/
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
  }
  Serial.println("Setup finished.");
}

void loop() {
  char comH, comL ,comP;
  
  BTSerial.listen();
  if (BTSerial.available() >= 3) {
    comH = BTSerial.read();
    comL = BTSerial.read();
    comP = BTSerial.read();
    Serial.println("Recaved from BT: "+String(comH)+String(comL)+String(comP));
    if (comH == 'L') {
        uint8_t cmd[] = {'L',comL,comP};
        xbeeBroadcast(cmd, sizeof(cmd));
    }
  }
  
  /*Radio.listen();
  File settings;
  if (Radio.available() >= 3) {
    comH = Radio.read();
    comL = Radio.read();
    comP = Radio.read();
    Serial.println("Recaved from Radio: "+String(comH)+String(comL)+String(comP));
    SD.remove("settings.sub");
    settings = SD.open("settings.sub", FILE_WRITE);
    settings.print(String(parkingLights) + String(lowbeamLights) + String(lowbeamAuto));
    settings.close();
  }*/
}

void xbeeBroadcast(uint8_t* command, int cmdSize) {
  XBeeAddress64 addr64 = XBeeAddress64(0x0000, 0xffff);
  Serial.print("Broadcasting: ");
  for(int i = 0; i < cmdSize; i++) {
    Serial.print(char(command[i]));
  }
  Serial.println(" ");
  ZBTxRequest zbTx = ZBTxRequest(addr64, command, cmdSize);
  xbee.send(zbTx);
}
