#include <SPI.h>
#include <RH_NRF24.h>

int RiderMode = 1;
int powerPin = A0; // for the potentiometer

// Singleton instance of the radio driver
RH_NRF24 nrf24;

void setup() 
{
  Serial.begin(9600);   //sets data rate in bits/second
  while (!Serial); // wait for serial port to connect. Needed for Leonardo only ... ! signifies "not"
  
  if (!nrf24.init())      //waits for radio driver (declared by code above aka receiver) to initialize
    {Serial.println("init failed");}  //if radio driver fails, prints error message
  
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1))
    {Serial.println("setChannel failed");}
  
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    {Serial.println("setRF failed");}
}

void StoreMode(String mode)
{
  // convert mode from string to int
  // set equal to global mode 
  RiderMode = mode.toInt();
}

int ReadBattery()
{
////  if (Serial.available() > 0) {
    // replace the analog read for the passing of the current power value into the function Battery
    // check if A0 is the battery pin
    double Power = analogRead(powerPin);
    
    double PercentPower = map(Power, 0, 675, 0, 100);
    
    Serial.println("\nPower: "); 
    Serial.print(Power);
    Serial.print('\n');
    //double PercentPower = (Power/1023.00)*100.00;
    Serial.println("Percent: "); 
    Serial.print(PercentPower);
    Serial.print('\n');

    // declare integer for battery case
        // 0 = default (error)
        // 1 is <= 25%
        // 2 is btwn 25 and 50%
        // 3 is btwn 50 and 75%
        // 4 is btwn 75 and 100%
        
    int batCase = 0;
    
    if (PercentPower <= 25)
    {
        batCase = 1;
        Serial.print("Battery Power: Case 1");
        Serial.print('\n');
        
    }
    else if (25 < PercentPower && PercentPower <= 50 )
    {
        batCase = 2;
        Serial.print("Battery Power: Case 2");
        Serial.print('\n');
        
    }
    else if (50 < PercentPower && PercentPower <= 75)
    {
        batCase = 3;
        Serial.print("Battery Power: Case 3");
        Serial.print('\n');
      
    }
    else if (75 < PercentPower && PercentPower <= 100)
    {
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
    
}


void SendBattery()
{ 
  // read battery level
  int batteryCase = ReadBattery();

  // send message based off of what was read
  switch (batteryCase){
    case 0:
    {
      // this is an error case, means the battery percent is a weird number
      uint8_t data[] = "$SendBat:0;";
      Serial.println("Sending: ");
      Serial.print((char*)data);
      nrf24.send(data, sizeof(data));
      nrf24.waitPacketSent();
      break;
    }
    case 1:
    {
      uint8_t data[] = "$SendBat:1;";
      Serial.println("Sending: ");
      Serial.print((char*)data);
      nrf24.send(data, sizeof(data));
      nrf24.waitPacketSent();
      break;
    }
    case 2:
    {
      uint8_t data[] = "$SendBat:2;";
      Serial.println("Sending: ");
      Serial.print((char*)data);
      nrf24.send(data, sizeof(data));
      nrf24.waitPacketSent();
      break;
    }
    case 3:
    {
      uint8_t data[] = "$SendBat:3;";
      Serial.println("Sending: ");
      Serial.print((char*)data);
      nrf24.send(data, sizeof(data));
      nrf24.waitPacketSent();
      break;
    }
    case 4:
    {
      uint8_t data[] = "$SendBat:4;";
      Serial.println("Sending: ");
      Serial.print((char*)data);
      nrf24.send(data, sizeof(data));
      nrf24.waitPacketSent();
      break;
    }
 
  } // end of switch case

}

// reads a message if it is available
// sends code to either send battery or store mode
void getMessage()
{
  
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
  
  }
  
}

void loop()
{
  getMessage();
}


