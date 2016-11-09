#ifndef CommunicationHandler_h
#define CommunicationHandler_h

#include <NRF24.h>
#include "Arduino.h"
#include "Logger.h"
#include <SPI.h>
#include "Pins.h"


class CommunicationHandler
{
public:
	CommunicationHandler(){
	};

	bool init() {
		return nrf24.init();
	}

	void sendMessage(String command, String argument) {
		String str = "$" + command + ":" + argument + ";";
		uint8_t data[sizeof(str)];
		str.toCharArray((char*)data, sizeof(data));
		Serial.println("Sending: ");
		Serial.print((char*)data);
		nrf24.send(data, sizeof(data));
		nrf24.waitPacketSent();
	}

	bool available() {
    return nrf24.available();
	}

	void processMessage(String message)
	{
		int start = message.lastIndexOf('$');
		int seperator = message.indexOf(':', start);
		int end = message.indexOf(';', start);

		String cmd = message.substring(start + 1, seperator);
		String arg = message.substring(seperator + 1, end);

		logger.verbose("Received: " + message);
		logger.verbose("\t Command:" + cmd + " Argument:" + arg);
	}

	String receive() {
		uint8_t buf[128]; //RH_NRF24_MAX_MESSAGE_LEN ???
		uint8_t len = sizeof(buf);
		if (nrf24.recv(buf, &len)) {
      String str = (char*)buf;
			logger.info("Message received: " + str);
			return str;
		}
		logger.warn("Null message received on wireless communication ISR.");
		return "";
	}
private:
	NRF24 nrf24; // Singleton instance of the radio driver
	Logger logger;
};

#endif
