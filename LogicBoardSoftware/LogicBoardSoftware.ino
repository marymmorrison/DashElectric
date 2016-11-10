#include "CommunicationHandler.h"
#include "HardwareHandler.h"

// Singleton instance of the radio driver
CommunicationHandler handler;
HardwareHandler hardware;

short status = 0;

void processMessage();
int readBattery();

void setup()
{
        Serial.begin(9600);     // set up Serial library at 9600 bps
        while(!handler.init());
        hardware.init();
}

void loop()
{
        hardware.monitorSpeed();
        if(handler.receive()) {
                processMessage();
        }    
}

void processMessage() {
        String cmd = handler.getCommand();
        if(cmd.equals("STATUS")) {
                handler.sendMessage("STATUS", "0");
        }
        else if(cmd.equals("SPEED")) {
                handler.sendMessage("ACK", "SPEED");
                hardware.setSpeed(handler.getArgument().toInt());
        }
        else if(cmd.equals("MODE")) {
                handler.sendMessage("ACK", "MODE");
                hardware.setRiderMode(handler.getArgument().toInt());
        }
        else if(cmd.equals("GETBAT")) {
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
        double Power = analogRead(A0);
        int batCase = map(Power, 0, 675, 1, 4);

        Serial.println("\nPower: ");
        Serial.println(Power);

        Serial.println("Percent: ");
        Serial.println(batCase);

        return batCase;
}
