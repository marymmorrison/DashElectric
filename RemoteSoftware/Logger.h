/*
   Logger.h - Library for logging code correctly.
 */

#ifndef Logger_h
#define Logger_h

#include "Arduino.h"

class Logger
{
public:
	Logger(){
	};
	void init(int baud) {
		Serial.begin(baud);
	}

	void verbose(String message) {
		if (_debugLevel >= VERBOSE) {
			Serial.println("[VERBOSE]\t" + message);
		}
	}

	void info(String message) {
		if (_debugLevel >= INFO) {
			Serial.println("[INFO]\t" + message);
		}
	}

	void warn(String message) {
		if (_debugLevel >= WARN) {
			Serial.println("[WARN]\t" + message);
		}
	}

	void error(String message) {
		if (_debugLevel >= ERROR) {
			Serial.println("[ERROR]\t" + message);
		}
	}

	void setDebugLevel(int level) {
		_debugLevel = level;
	}

	int VERBOSE = 4;
	int INFO = 3;
	int WARN = 2;
	int ERROR = 1;
	int OFF = 0;

private:
	int _debugLevel;
};

#endif
