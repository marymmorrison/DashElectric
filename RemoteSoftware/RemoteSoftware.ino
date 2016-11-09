#include "CommunicationHandler.h"
#include "HardwareHandler.h"
#include "Logger.h"
#include "Pins.h"

HardwareHandler handler;
CommunicationHandler comm;
Logger logger;

short status = 0;
short battery_counter = 0;
short mode_counter = 0;

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
                if (digitalRead(batteryButton) == HIGH) {
                        if(battery_counter < 4000) {
                                battery_counter++;
                                if(battery_counter > 3000) {
                                        handler.displayBatteryLevel();
                                        Serial.println("Battery pushed!");
                                }
                        }
                } else {
                        if(battery_counter >= 0 ) {
                                battery_counter--;
                        } else {
                                handler.turnOffStatusLEDs();
                        }
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
        }
        comm.sendMessage("SPEED", String(status));
}

void processMessage() {
        String cmd = comm.getCommand();
        if(cmd.equals("STATUS")) {
                Serial.println("Logic board is good!");
        } else {
                Serial.print("Unknown command - ");
                Serial.println(cmd);
        }
}
