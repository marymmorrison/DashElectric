/*
   Logger.h - Library for logging code correctly.
 */

#ifndef Logger_h
#define Logger_h

#include "Arduino.h"

class Logger
{
public:
	Logger(){};
	void init(int baud) {
		Serial.begin(baud);
	}

	void verbose(String message) {
		if (_debugLevel >= VERBOSE) {
			Serial.println(message);
		}
	}

	void info(String message) {
		if (_debugLevel >= INFO) {
			Serial.println(message);
		}
	}

	void warn(String message) {
		if (_debugLevel >= WARN) {
			Serial.println(message);
		}
	}

	void error(String message) {
		if (_debugLevel >= ERROR) {
			Serial.println(message);
		}
	}

	void setDebugLevel(int level) {
		_debugLevel = level;
	}

	int OFF = 0;
	int VERBOSE = 1;
	int INFO = 2;
	int WARN = 3;
	int ERROR = 4;
private:
	int _debugLevel;
};

#endif
