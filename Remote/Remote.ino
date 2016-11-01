#include "CommunicationHandler.h"
#include "HardwareHandler.h"
#include "Logger.h"
#include "Pins.h"

HardwareHandler handler;
Logger logger;

void setup()
{
        logger.init(9600);
        handler.init(logger);
        // input buttons
        pinMode(batteryButton, INPUT);
        pinMode(riderModeButton, INPUT);
}

void loop()
{
        if (digitalRead(batteryButton) == HIGH)
        {
                logger.info("Battery button pressed.");
        }
        if (digitalRead(riderModeButton) == HIGH)
        {
                logger.info("Rider mode button pressed.");
        }
}


/*void MODE()
   {
        ModeDisplay();
        delay(1000);
        long timev = millis();
        Serial.print("Currenttime:");
        Serial.print(timev);
        Serial.print('\n');
        while ( millis() < timev + (2000)) {
                if (digitalRead(ModeButton)==HIGH)
                {
                        Serial.print("YOU DOUBLE PRESSED THE MODE BUTTON");
                        Serial.print('\n');
                        ModeChange();
                        ModeDisplay();
                        delay(500);
                }
        }
   }*/


   //void RequestBattery() "$GetBat:0;";
