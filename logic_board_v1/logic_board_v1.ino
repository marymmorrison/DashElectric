#include <SPI.h>
#include <RH_NRF24.h>

int PinLED1 = 13;
int PinLED2 = 12;
int PinLED3 = 11;
int PinLED4 = 10;
int PinLED5 = 9;
int BatteryButton = 6;
int ModeButton = 5;
int RiderMode = 1;

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
  }
  else if (RiderMode == 2)
  {
  RiderMode = 3;
  Serial.print("NEW RIDER MODE:");
  Serial.print(RiderMode);
  Serial.print('\n');
  }
  else if (RiderMode == 3)
  {
  Serial.print("NEW RIDER MODE:");
  Serial.print(RiderMode);
  Serial.print('\n');
  RiderMode = 1;
  }
}

void Battery(){
if (Serial.available() > 0) {
    double Power = analogRead(A0);// replace the analog read for the passing of the current power value into the function Battery
    Serial.print("Power: ");
    Serial.print(Power);
    Serial.print('\n');
    double PercentPower = (Power/1023.00)*100.00;
    Serial.print("Percent: ");
    Serial.print(PercentPower);
    Serial.print('\n');
    if (PercentPower <= 5)
    {
        digitalWrite(PinLED1, LOW);
        digitalWrite(PinLED2, LOW);
        digitalWrite(PinLED3, LOW);
        digitalWrite(PinLED4, LOW);
        Serial.print("CASE0");
        Serial.print('\n');

    }
    else if ( 5 < PercentPower && PercentPower <= 25)
    {
        digitalWrite(PinLED1, HIGH);
        digitalWrite(PinLED2, LOW);
        digitalWrite(PinLED3, LOW);
        digitalWrite(PinLED4, LOW);
        Serial.print("CASE1");
        Serial.print('\n');

    }
    else if (25 < PercentPower && PercentPower <= 50 )
    {
        digitalWrite(PinLED1, HIGH);
        digitalWrite(PinLED2, HIGH);
        digitalWrite(PinLED3, LOW);
        digitalWrite(PinLED4, LOW);
        Serial.print("CASE2");
        Serial.print('\n');

    }
    else if (50 < PercentPower && PercentPower <= 75)
    {
        digitalWrite(PinLED1, HIGH);
        digitalWrite(PinLED2, HIGH);
        digitalWrite(PinLED3, HIGH);
        digitalWrite(PinLED4, LOW);
        Serial.print("CASE3");
        Serial.print('\n');

    }
    else if (75 < PercentPower && PercentPower <= 100)
    {
        digitalWrite(PinLED1, HIGH);
        digitalWrite(PinLED2, HIGH);
        digitalWrite(PinLED3, HIGH);
        digitalWrite(PinLED4, HIGH);
        Serial.print("CASE4");
        Serial.print('\n');
    }
}
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
  for (int thisPin = 9; thisPin < 14; thisPin++)
    {
    pinMode(thisPin, OUTPUT);
    }
  for (int thisPin = 5; thisPin < 7; thisPin++)
    {
    pinMode(thisPin, INPUT);
    }
}

void loop()
{
  if (digitalRead(BatteryButton)==HIGH)
  {
    Serial.print("YOU PRESSED THE BATTERY BUTTON");
    Serial.print('\n');
    Battery();
    delay(2000);
  }
  if (digitalRead(ModeButton)==HIGH)
  {
    Serial.print("YOU PRESSED THE MODE BUTTON");
    Serial.print('\n');
    MODE();
  }
  TurnOff();
}
