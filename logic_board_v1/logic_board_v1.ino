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

<<<<<<< HEAD
int ReadBattery()
{
//  if (Serial.available() > 0) {
    // replace the analog read for the passing of the current power value into the function Battery
    // check if A0 is the battery pin
    double Power = analogRead(powerPin);
    
    double PercentPower = map(Power, 0, 675, 0, 100);
    
    Serial.println("\nPower: "); 
=======
void Battery(){
if (Serial.available() > 0) {
    double Power = analogRead(A0);// replace the analog read for the passing of the current power value into the function Battery
    Serial.print("Power: "); 
>>>>>>> 7a4a761731b09bbe2cff43b410dc2156aecda4dd
    Serial.print(Power);
    Serial.print('\n');
    //double PercentPower = (Power/1023.00)*100.00;
    Serial.println("Percent: "); 
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
<<<<<<< HEAD
        batCase = 4;
        Serial.print("Battery Power: Case 4");
        Serial.print('\n');
    }

    // Saif note to himself/Hector: I think a more elegant way to do the above is:
    // I will change this later after testing - Saif
    /*
     * int case = PercentPower%25 + 1;
     */
     
    return batCase;
//  }
//  else
//  {
//    Serial.println("Serial not Available");
//  }
    
=======
        digitalWrite(PinLED1, HIGH);
        digitalWrite(PinLED2, HIGH);
        digitalWrite(PinLED3, HIGH);
        digitalWrite(PinLED4, HIGH);
        Serial.print("CASE4");
        Serial.print('\n');
    }
}
>>>>>>> 7a4a761731b09bbe2cff43b410dc2156aecda4dd
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
<<<<<<< HEAD
  
  if (nrf24.waitAvailableTimeout(500))
  {
    // Should be a message for us now   
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (nrf24.recv(buf, &len))
    {
      // print request received
      Serial.print("Got Request: ");
      String readString = (char*)buf;
      Serial.println(readString);

      int ind1, ind2, ind3;

      // get indicies for characters that separate messages
      // Message should look like: 
          // $command:argument;
          
      ind1 = readString.indexOf('$');
      ind2 = readString.indexOf(':');
      ind3 = readString.indexOf(';'); 

      // store what the message says
      String command, argument;
      command = readString.substring(ind1+1, ind2);
      argument = readString.substring(ind2+1, ind3);
      Serial.println("Command and argument received: ");
      Serial.print(command);
      Serial.print(", ");
      Serial.print(argument);

      // run specific commands
      
        if (command == "GetBat"){
          SendBattery();
        }
        else if(command == "SendMode")
        {
          StoreMode(argument);
        }
        else {
          Serial.println("Command Given: ");
          Serial.print(command);
          Serial.println("Error: Command does not match known commands."); 
        }
        
      // end of running specific commands
      
    }
  
=======
  if (digitalRead(BatteryButton)==HIGH)
  {
    Serial.print("YOU PRESSED THE BATTERY BUTTON");
    Serial.print('\n');
    Battery();
    delay(2000);
>>>>>>> 7a4a761731b09bbe2cff43b410dc2156aecda4dd
  }
  if (digitalRead(ModeButton)==HIGH)
  { 
    Serial.print("YOU PRESSED THE MODE BUTTON");
    Serial.print('\n');
    MODE();
  }
  TurnOff(); 
}


