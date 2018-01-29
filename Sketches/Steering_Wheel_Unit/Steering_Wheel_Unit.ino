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
      char comH, comL, comP;
      comH = char(rx.getData()[0]);
      if (comH == 'L') {
        comL = char(rx.getData()[1]);
        comP = char(rx.getData()[2]);
        if (comL == 'P') {
          parkingLights = comP;
          if (comP == '1')
            switchParkingLights(true);
          else if (comP == '0')
            switchParkingLights(false);
        } else if (comL == 'L') {
          lowBeamLights = comP;
          if (comP == '1')
            switchLowBeamLights(true);
          else if (comP == '0')
            switchLowBeamLights(false);
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
      if (lux < 100) switchLowBeamLights(true); else if (lux > 105) switchLowBeamLights(false);
    }
  }
}

void switchParkingLights(boolean on) {
  if (on) digitalWrite(7, HIGH); else digitalWrite(7, LOW);
}

void switchLowBeamLights(boolean on) {
  if (on) {
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
  } else {
    digitalWrite(8, LOW);
    if (parkingLights == '0') digitalWrite(7, LOW); //switch parking lights off if low beam was turned on without it
  }
}

