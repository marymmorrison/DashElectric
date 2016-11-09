#include <RH_NRF24.h>
#include "CommunicationHandler.h"

// Singleton instance of the radio driver
RH_NRF24 nrf24;
CommunicationHandler handler;

short status = 0;
short riderMode = 1;
short speed = 0;
int powerPin = A0; // for the potentiometer

void processMessage();
int readBattery();

void setup()
{
        Serial.begin(9600);
        handler.init();
}

void loop()
{
        if(handler.receive()) {
                processMessage();
        }
}

void processMessage() {
        String cmd = handler.getCommand();
        if(cmd.equals("STATUS")) {
                handler.sendMessage("ACK", "STATUS");
                handler.sendMessage("STATUS", "0");
        }
        else if(cmd.equals("SPEED")) {
                handler.sendMessage("ACK", "SPEED");
                speed = handler.getArgument().toInt();
        }
        else if(cmd.equals("MODE")) {
                handler.sendMessage("ACK", "MODE");
                riderMode = handler.getArgument().toInt();
        }
        else if(cmd.equals("GETBAT")) {
                handler.sendMessage("ACK", "GETBAT");
                handler.sendMessage("BATLVL", String(readBattery()));
        } else {
          Serial.print("Unknown command - ");
          Serial.println(cmd);
        }
}

int readBattery()
{
        // replace the analog read for the passing of the current power value into the function Battery
        // check if A0 is the battery pin
        double Power = analogRead(powerPin);
        int batCase = map(Power, 0, 675, 1, 4);

        Serial.println("\nPower: ");
        Serial.println(Power);

        Serial.println("Percent: ");
        Serial.println(batCase);

        return batCase;
}
