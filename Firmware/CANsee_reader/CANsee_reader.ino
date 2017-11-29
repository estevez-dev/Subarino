//This sketch requires the Seeed CAN-BUS Shield libraries
//https://github.com/yexiaobo-seeedstudio/CAN_BUS_Shield
#include <SPI.h>
#include "mcp_can.h"

const int SPI_CS_PIN = 10; //9 or 10
MCP_CAN CAN(SPI_CS_PIN);


void setup(){
  Serial.begin(38400);

  START_INIT:

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

  if(CAN_OK == CAN.begin(CAN_500KBPS)){
      Serial.println("CAN BUS Shield init ok!");
  }else{
      Serial.println("CAN BUS Shield init fail");
      Serial.println("Init CAN BUS Shield again");
      delay(2000);
      goto START_INIT;
  }
}//end setup


void loop(){
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
}//end loop
