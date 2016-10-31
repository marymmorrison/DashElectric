#include <SPI.h>
#include <RH_NRF24.h>

#include <radio_config_Si4460.h>
#include <RadioHead.h>
#include <RH_ASK.h>
#include <RH_NRF24.h>
#include <RH_NRF51.h>
#include <RH_NRF905.h>
#include <RH_RF22.h>
#include <RH_RF24.h>
#include <RH_RF69.h>
#include <RH_RF95.h>
#include <RH_Serial.h>
#include <RH_TCP.h>
#include <RHCRC.h>
#include <RHDatagram.h>
#include <RHGenericDriver.h>
#include <RHGenericSPI.h>
#include <RHHardwareSPI.h>
#include <RHMesh.h>
#include <RHNRFSPIDriver.h>
#include <RHReliableDatagram.h>
#include <RHRouter.h>
#include <RHSoftwareSPI.h>
#include <RHSPIDriver.h>
#include <RHTcpProtocol.h>

// pin setup
int PinLED1 = 9;
int PinLED2 = 7;
int PinLED3 = 6;
int PinLED4 = 5;
int PinLED5 = A5; // Analog 5 because can't use digital 1 or 0

int BatteryButton = 4;
int ModeButton = 2;
// end of pin setup

// variables to display
int RiderMode = 1;
int BatteryLevel = 4; // 4 = max, 1 = min

// Singleton instance of the radio driver
RH_NRF24 nrf24;

void MODE()
{
  ModeDisplay();
  delay(1000);
  long timev = millis();
  Serial.print("Currenttime:");
  Serial.print(timev);
  Serial.print('\n');
  while ( millis() < timev + (2000)){
          if (digitalRead(ModeButton)==HIGH)
          { 
          Serial.print("YOU DOUBLE PRESSED THE MODE BUTTON");
          Serial.print('\n');
          ModeChange();
          ModeDisplay();
          delay(500);
          }
  }
}


// function to display mode LEDs
void ModeDisplay(){
   switch (RiderMode)
   {
      case 1:
        digitalWrite(PinLED1, LOW);
        digitalWrite(PinLED2, LOW);
        digitalWrite(PinLED3, LOW);
        digitalWrite(PinLED4, HIGH);
        Serial.print("Rider Mode 1");
        Serial.print('\n');
        break;
      case 2:
        digitalWrite(PinLED1, LOW);
        digitalWrite(PinLED2, LOW);
        digitalWrite(PinLED3, HIGH);
        digitalWrite(PinLED4, LOW);
        Serial.print("Rider Mode 2");
        Serial.print('\n');
        break;
      case 3:
        digitalWrite(PinLED1, LOW);
        digitalWrite(PinLED2, HIGH);
        digitalWrite(PinLED3, LOW);
        digitalWrite(PinLED4, LOW);
        Serial.print("Rider Mode 3");
        Serial.print('\n');
        break;
    }
  
}

void ModeChange(){
  
  if (RiderMode == 1)
  {
  RiderMode = 2;
  Serial.print("NEW RIDER MODE:");
  Serial.print(RiderMode);
  Serial.print('\n');

  // send mode
  uint8_t data[] = "$SendMode:2;";
  Serial.println("Sending: ");
  Serial.print((char*)data);
  nrf24.send(data, sizeof(data));
  nrf24.waitPacketSent();
  }
  else if (RiderMode == 2)
  {
  RiderMode = 3;
  Serial.print("NEW RIDER MODE:");
  Serial.print(RiderMode);
  Serial.print('\n');

  // send mode
  uint8_t data[] = "$SendMode:3;";
  Serial.println("Sending: ");
  Serial.print((char*)data);
  nrf24.send(data, sizeof(data));
  nrf24.waitPacketSent();
  }
  else if (RiderMode == 3)
  {
  RiderMode = 1;   
  Serial.print("NEW RIDER MODE:");
  Serial.print(RiderMode);
  Serial.print('\n');  

  // send mode
  uint8_t data[] = "$SendMode:1;";
  Serial.println("Sending: ");
  Serial.print((char*)data);
  nrf24.send(data, sizeof(data));
  nrf24.waitPacketSent();
  } 
  
} // end of ModeChange function

void DispBattery(){
  
    switch (BatteryLevel)
    {
      case 1:
        digitalWrite(PinLED1, HIGH);
        digitalWrite(PinLED2, LOW);
        digitalWrite(PinLED3, LOW);
        digitalWrite(PinLED4, LOW);
        Serial.print("CASE1");
        Serial.print('\n');
        break;
      case 2:
        digitalWrite(PinLED1, HIGH);
        digitalWrite(PinLED2, HIGH);
        digitalWrite(PinLED3, LOW);
        digitalWrite(PinLED4, LOW);
        Serial.print("CASE2");
        Serial.print('\n');
        break;
      case 3:
        digitalWrite(PinLED1, HIGH);
        digitalWrite(PinLED2, HIGH);
        digitalWrite(PinLED3, HIGH);
        digitalWrite(PinLED4, LOW);
        Serial.print("CASE3");
        Serial.print('\n');
        break;
      case 4:
        digitalWrite(PinLED1, HIGH);
        digitalWrite(PinLED2, HIGH);
        digitalWrite(PinLED3, HIGH);
        digitalWrite(PinLED4, HIGH);
        Serial.print("CASE4");
        Serial.print('\n');
        break;
    }
    
}

void GetBattery()
{
  if (nrf24.available())
  {
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    
    if (nrf24.recv(buf, &len))
    {
      // Variables for reading message
      int ind1, ind2, ind3;
      String readString = (char*)buf;

      // get indicies for characters that separate messages
      ind1 = readString.indexOf('$');
      ind2 = readString.indexOf(':');
      ind3 = readString.indexOf(';'); 

      // print out command
      Serial.print(readString.substring(ind1+1, ind2));
      // print out argument: should be between 1 and 4
      Serial.print(readString.substring(ind2+1, ind3));

      // if first message says SendBat then set battery level to the value of argument
      if (readString.substring(ind1+1, ind2) == "SendBat")
      {
        BatteryLevel = readString.substring(ind2+1, ind3).toInt();
      }
      
    } // end of RECV
    
  } // end of NRF24 Available Loop

} // end of GetBattery

void RequestBattery()
{
  uint8_t data[] = "$GetBat:0;";
  Serial.println("Sending: ");
  Serial.print((char*)data);
  nrf24.send(data, sizeof(data));
  nrf24.waitPacketSent();
}

void TurnOff()
{
   digitalWrite(PinLED1, LOW);
   digitalWrite(PinLED2, LOW);
   digitalWrite(PinLED3, LOW);
   digitalWrite(PinLED4, LOW);
   digitalWrite(PinLED5, LOW);
}

void setup() 
{
  Serial.begin(9600);

  // set led pins to output
  pinMode(PinLED1, OUTPUT);
  pinMode(PinLED2, OUTPUT);
  pinMode(PinLED3, OUTPUT);
  pinMode(PinLED4, OUTPUT);
  
  pinMode(PinLED5, OUTPUT); // analog setup

  // input buttons
  pinMode(BatteryButton, INPUT); 
  pinMode(ModeButton, INPUT); 
  
}

void loop()
{
  if (digitalRead(BatteryButton) == HIGH)
  {
    Serial.print("YOU PRESSED THE BATTERY BUTTON");
    Serial.print('\n');
    RequestBattery();
    GetBattery();
    DispBattery();
    //delay(2000);
  }
  if (digitalRead(ModeButton) == HIGH)
  { 
    Serial.print("YOU PRESSED THE MODE BUTTON");
    Serial.print('\n');
    MODE();
  }
  TurnOff(); 
}


