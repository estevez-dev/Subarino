#include <mcp_can.h>
#include <SPI.h>
#include <SoftwareSerial.h>

//SoftSerial for bluetooth
//SoftwareSerial BTSerial(3, 2); // RX | TX
//String btReader;
//CAN
const int SPI_CS_PIN = 10; //pin 10 for Subarino CAN-Bus shield (can be 9)
MCP_CAN CAN(SPI_CS_PIN);

void setup() {
  
  //pinMode(12, OUTPUT);
  Serial.begin(38400);
  //Serial.println("<=== Subarino Prototype Alpha ===>");
  //Serial.println("Bluetooth initialization...");
  //BTSerial.begin(9600);
  //Serial.println("..Done");
  
  START_CAN_INIT:
  //Serial.println("CAN-Bus initialization...");
  /*
    Avaiable CAN speeds
    CAN_5KBPS
    CAN_10KBPS
    CAN_20KBPS
    CAN_40KBPS
    CAN_50KBPS
    CAN_80KBPS
    CAN_100KBPS
    CAN_125KBPS
    CAN_200KBPS
    CAN_250KBPS possible
    CAN_500KBPS possible
    CAN_1000KBPS
   */
  if (CAN_OK == CAN.begin(CAN_500KBPS)) {
      //Serial.println("..Done!");
  } else {
      //Serial.println("..Failed. Will retry in 2 seconds");
      delay(2000);
      goto START_CAN_INIT;
  }  
}

void loop() {
  /* Bluetooth commands reader and worker*/
  /*btReader="";
  while (BTSerial.available()) {
    delay(3);
    if (BTSerial.available()>0) {
      char c = BTSerial.read();
      btReader += c;
        
    }
  }
  if (btReader.length() > 0) {
    Serial.println("Command received via bluetooth: "+btReader+" (length="+btReader.length()+")");
    if (btReader == "On") 
      digitalWrite(12, HIGH);
    else
      digitalWrite(12, LOW); 
    BTSerial.write("Maybe...");
  }*/
  
  /* CAN-Bus reader for CANsee app*/
  unsigned char len = 0;
  unsigned char buf[8];

  if(CAN_MSGAVAIL == CAN.checkReceive()){
    CAN.readMsgBuf(&len, buf);
    int can_id = CAN.getCanId();

    Serial.print("<");
    Serial.print(can_id);
    Serial.print(",");

    for(int i = 0; i<len; i++){
      Serial.print(buf[i]);
      Serial.print(",");
    }
    Serial.print(">");
    Serial.println();
  }
  
}
