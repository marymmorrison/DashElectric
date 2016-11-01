#include "CommunicationHandler.h"
#include "HardwareHandler.h"
#include "Logger.h"
#include "Pins.h"

HardwareHandler handler;
CommunicationHandler comm;
Logger logger;

String buffer[16]; // Volatile ??
volatile int bufferIndex = 0;

void setup()
{
        logger.init(9600);
        handler.init(logger);
        comm.init();
        // input buttons
        pinMode(batteryButton, INPUT);
        pinMode(riderModeButton, INPUT);

        // setup interrupts
        attachInterrupt(digitalPinToInterrupt(nrf24Interrupt), interruptServiceRoutine, CHANGE);

        logger.setDebugLevel(logger.VERBOSE);
}

void loop()
{
        // checks if any data in the buffer
        if(bufferIndex > 0) {
                String msg = String(buffer[bufferIndex][0]);
                buffer[bufferIndex] = "";
                bufferIndex--;
                comm.processMessage(msg); // processes a command
        }
        if (digitalRead(batteryButton) == HIGH) {
                logger.info("Battery button pressed.");
        }
        if (digitalRead(riderModeButton) == HIGH) {
                logger.info("Rider mode button pressed.");
        }
}

void interruptServiceRoutine() {
        String message = comm.receive();
        //if(message.length() == 0) return;
        if(bufferIndex < sizeof(buffer)) {
                buffer[bufferIndex] = message;
                bufferIndex++;
        } else {
                logger.error("Buffer overflow from communication ISR!");
        }
        logger.info("Message received: " + message);
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
