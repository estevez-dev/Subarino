#include <XBee.h>

#include <BH1750FVI.h>

#define  HL_INTERVAL  1000UL
BH1750FVI LightSensor;
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();
char parkingLights = '0';
char lowBeamLights = '0';

void setup()
{
  pinMode(7, OUTPUT); //parking lights
  pinMode(8, OUTPUT); //lowbeam lights
  Serial.begin(9600);
  xbee.begin(Serial);
  LightSensor.begin();
  LightSensor.SetAddress(Device_Address_H);
  LightSensor.SetMode(Continuous_H_resolution_Mode);
}

void loop() {
  xbee.readPacket();
  if (xbee.getResponse().isAvailable()) {
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      xbee.getResponse().getZBRxResponse(rx);
      char comT, comH, comL, comP;
      comT = char(rx.getData()[0]);
      comH = char(rx.getData()[1]);
      if ((comT == 'A') && (comH == 'L')) {
        comL = char(rx.getData()[2]);
        comP = char(rx.getData()[3]);
        if (comL == 'P') {
          switchParkingLights(comP);
        } else if (comL == 'L') {
          switchLowBeamLights(comP);
        }
      }
    }
  }
  
  //check light sensor
  if (lowBeamLights == '2') {
    uint16_t lux = LightSensor.GetLightIntensity();
    static unsigned long previousMillis = 0;
    if(millis() - previousMillis > HL_INTERVAL) {
      previousMillis = millis();
      if (lux < 100) switchLowBeamLights('1'); else if (lux > 105) switchLowBeamLights('0');
    }
  }
}

void switchParkingLights(char state) {
  if (state != parkingLights) {
    if (state == '1')
      digitalWrite(7, HIGH);
    else
      digitalWrite(7, LOW);
    parkingLights = state;
    uint8_t cmd[] = {'S','L','P',state};
    xbeeBroadcast(cmd, sizeof(cmd));
  }
}

void switchLowBeamLights(char state) {
  if (state != lowBeamLights) {
    if (state == '1') {
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
    } else if (state == '0') {
      digitalWrite(8, LOW);
      if (parkingLights == '0') digitalWrite(7, LOW); //switch parking lights off if low beam was turned on without it
    }
    lowBeamLights = state;
    uint8_t cmd[] = {'S','L','L',state};
    xbeeBroadcast(cmd, sizeof(cmd));
  }
}

void xbeeBroadcast(uint8_t* command, int cmdSize) {
  XBeeAddress64 addr64 = XBeeAddress64(0x0000, 0xffff);
  ZBTxRequest zbTx = ZBTxRequest(addr64, command, cmdSize);
  xbee.send(zbTx);
}

