#include "CommunicationHandler.h"
#include "HardwareHandler.h"
#include "Logger.h"
#include "Pins.h"

HardwareHandler handler;
CommunicationHandler comm;
Logger logger;

short status = 0;
short speed = 0;
short mode_counter = 0;

void processMessage();

void setup()
{
        //logger.init(9600);
        Serial.begin(9600);
        handler.init(logger);
        comm.init();
        // input buttons
        pinMode(batteryButton, INPUT);
        pinMode(riderModeButton, INPUT);

        logger.setDebugLevel(logger.VERBOSE);
        logger.error("Hi!");
}

void loop()
{
        long start = millis();
        while(millis() - start < 150) {
                if(comm.receive()) {
                        processMessage();
                }
                if(handler.checkBatteryButton()){
                  comm.sendMessage("GETBAT", "0");
                }
                if (digitalRead(riderModeButton) == HIGH) {
                        mode_counter++;
                        if(mode_counter > 3000) {
                                Serial.println("Mode pushed!");
                                mode_counter = 0;
                        }
                } else {
                        mode_counter = 0;
                }
                
                double speedIn = analogRead(A0);
                speed = map(speedIn, 0, 1023, 0, 100);
        }
        comm.sendMessage("SPEED", String(speed));
}

void processMessage() {
        String cmd = comm.getCommand();
        if(cmd.equals("STATUS")) {
                Serial.println("Logic board is good!");
        } else if(cmd.equals("ACK")) {
        }  else {
                Serial.print("Unknown command - ");
                Serial.println(cmd);
        }
}
