#include <XBee.h>

#include <SD.h>

#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial BTSerial(3, 2); // RX | TX
SoftwareSerial XBeeSerial(6, 5);
XBee xbee = XBee();

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
    File file;
    Serial.println("Done.");
    Serial.println("Reading saved state...");
    delay(8000);
    File root = SD.open("/");
    root.rewindDirectory();
    nextfile:
    file = root.openNextFile();
    if (!file) {
      Serial.println("Done.");
    } else {
      if ((file.name()[0] == 'S') && (file.name()[1] == 'T')) {
        uint8_t com[5] = {'A', file.name()[2], file.name()[3], file.read()};
        xbeeBroadcast(com, sizeof(com));
      }
      file.close();
      goto nextfile;
    }
    root.close();
  }
  Serial.println("Setup finished.");
}

void loop() {
  char comT, comH, comL ,comP;
  File stateFile;
  
  BTSerial.listen();
  if (BTSerial.available() >= 4) {
    comT = BTSerial.read();
    comH = BTSerial.read();
    comL = BTSerial.read();
    comP = BTSerial.read();
    Serial.println("Recaved from BT: "+String(comT)+String(comH)+String(comL)+String(comP));
    if (comT == 'A') {
        uint8_t cmd[] = {'A',comH,comL,comP};
        xbeeBroadcast(cmd, sizeof(cmd));
        char fileName[5] = {'S','T',comH, comL}; 
        Serial.print("Writing to file: ");
        Serial.print(fileName);
        Serial.print("...");
        SD.remove(fileName);
        stateFile = SD.open(fileName, FILE_WRITE);
        if (stateFile) {
          stateFile.print(comP);
          stateFile.close();
          Serial.println("Done.");
        } else {
          Serial.println("Failed.");
        }
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
