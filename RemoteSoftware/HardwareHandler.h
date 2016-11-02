#ifndef HardwareHandler_h
#define HardwareHandler_h

#include "Arduino.h"
#include "Pins.h"
#include "Logger.h"

class HardwareHandler
{
public:
	HardwareHandler(){
	};

	void init(Logger l) {
		// set led pins to output
		pinMode(statusLED1, OUTPUT);
		pinMode(statusLED2, OUTPUT);
		pinMode(statusLED3, OUTPUT);
		pinMode(statusLED4, OUTPUT);

		logger = l;
	}

// Displays rider mode on the LEDs
	void displayRiderMode(int riderMode){
		digitalWrite(statusLED1, riderMode == 1);
		digitalWrite(statusLED2, riderMode == 2);
		digitalWrite(statusLED3, riderMode == 3);
		digitalWrite(statusLED4, riderMode == 4);

		logger.verbose("Rider mode is " + riderMode);
	}

// Displays battery level on the LEDs
	void displayBatteryLevel() {
		int value = batteryLevel / 25; // NOT CORRECT - need to figure proper battery percentage algorithm

		digitalWrite(statusLED1, value > 0);
		digitalWrite(statusLED2, value > 1);
		digitalWrite(statusLED3, value > 2);
		digitalWrite(statusLED4, value > 3);

		logger.verbose("Battery level: " + value);
	}

// Sets rider mode
	void setRiderMode(int value){
		if(value >= 1 && value <= 3)
		{
			riderMode = value;
			logger.info("Rider mode changed: " + value); \
		}
		else
		{
			logger.error("Out of range exception!");
			logger.error("\t Rider mode is " + value);
		}
		//"$SendMode:" + String.valueOf(RiderMode) + ";"
	}

// Sets battery level
	void setBatteryLevel(int value){
		if(value >= 0 && value <= 100)
		{
			batteryLevel = value;
			logger.info("Battery level changed: " + value);
		}
		else
		{
			logger.error("Out of range exception!");
			logger.error("\t Battery level is " + value);
		}
	}

	// Turns off all status LEDs
	void turnOffStatusLEDs()
	{
		digitalWrite(statusLED1, LOW);
		digitalWrite(statusLED2, LOW);
		digitalWrite(statusLED3, LOW);
		digitalWrite(statusLED4, LOW);
	}

private:
	int batteryLevel;
	int riderMode;

	Logger logger;
};

#endif
