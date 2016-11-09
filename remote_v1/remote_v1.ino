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
int ModeButton = 3;
// end of pin setup

// variables to display
int RiderMode = 1;
int BatteryLevel = 4; // 4 = max, 1 = min

// Singleton instance of the radio driver
RH_NRF24 nrf24;

void MODE()
{
  ModeDisplay();
  delay(1000); // can you help with getting rid of delays @ MELLO
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
  Serial.println("Change mode");
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
    Serial.println();
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
<<<<<<< HEAD
  if (nrf24.waitAvailableTimeout(500)) // if the other transceiver doesnt reply fast enough, try   if (nrf24.waitAvailableTimeout(200)) 
=======
  if (nrf24.available())
>>>>>>> 7a4a761731b09bbe2cff43b410dc2156aecda4dd
  {
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    
    if (nrf24.recv(buf, &len))
    {
      // Variables for reading message
      int ind1, ind2, ind3;
      String readString = (char*)buf;

      Serial.println("Recv");
      Serial.print(readString);

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
    else
  {
    Serial.println("didn't recv");
  }
    
  } // end of NRF24 Available Loop

} // end of GetBattery

void RequestBattery()
{
  Serial.println("Requesting Battery");
  uint8_t data[] = "$GetBat:0;";
  Serial.println("Sending: ");
  Serial.print((char*)data);
  nrf24.send(data, sizeof(data));
  nrf24.waitPacketSent();
  Serial.println("Should have sent");
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

  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");  
  
  // input buttons
  pinMode(BatteryButton, INPUT); 
  pinMode(ModeButton, INPUT); 
  
}

int batButtonState;             // the current reading from the input pin
int lastBatButtonState = LOW;   // the previous reading from the input pin

long lastBatDebounceTime = 0;  // the last time the output pin was toggled
long batDebounceDelay = 50;    // the debounce time; increase if the output flickers

////////////////////////////////

int modeButtonState;             // the current reading from the input pin
int lastModeButtonState = LOW;   // the previous reading from the input pin

long lastModeDebounceTime = 0;  // the last time the output pin was toggled
long modeDebounceDelay = 50;    // the debounce time; increase if the output flickers

void loop()
<<<<<<< HEAD
{ 
  // button debounce for battery
  int reading = digitalRead(BatteryButton);
  
  if (reading != lastBatButtonState) {
    // reset the debouncing timer
    lastBatDebounceTime = millis();
  }

  if ((millis() - lastBatDebounceTime) > batDebounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != batButtonState) {
      batButtonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (batButtonState==HIGH)
      {
        Serial.println("BATTERY BUTTON\n");
        RequestBattery();
        GetBattery();
        DispBattery();
        delay(1500); // change this to use millis. Idk how to use millis yet.
      }
    }
    
  } // end of millis
  lastBatButtonState = reading;

  // end of button debounce for battery

  // *******************************
  // button debounce for mode
  // *******************************
  
  int reading_mode = digitalRead(ModeButton);
  
  if (reading_mode != lastModeButtonState) {
    // reset the debouncing timer
    lastModeDebounceTime = millis();
=======
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
>>>>>>> 7a4a761731b09bbe2cff43b410dc2156aecda4dd
  }

  if ((millis() - lastModeDebounceTime) > modeDebounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading_mode != modeButtonState) {
      modeButtonState = reading_mode;

      // only toggle the LED if the new button state is HIGH
      if (modeButtonState==HIGH)
      {
        Serial.println("Mode BUTTON\n");
        MODE();
        delay(1500); // change this to use millis. Idk how to use millis yet.
      }
    }
    
  } // end of millis
  lastModeButtonState = reading_mode;

  // END OF MODE BUTTON STATE
  
  TurnOff(); 
}


