#ifndef CommunicationHandler_h
#define CommunicationHandler_h

#include <NRF24.h>
#include "Arduino.h"
#include "Logger.h"
#include <SPI.h>


class CommunicationHandler
{
public:
	CommunicationHandler();
	void sendMessage(String command, String argument) {
		String str = "$" + command + ":" + argument + ";";
		int8_t data[sizeof(str)];
		str.toCharArray(data, sizeof(data));
		Serial.println("Sending: ");
		Serial.print((char*)data);
		nrf24.send(data, sizeof(data));
		nrf24.waitPacketSent();
	}

	void receive() // THIS NEEDS WORK !
	{
		if (nrf24.available())
		{
			String readString;
			int start = readString.lastIndexOf('$');
			int seperator = readString.indexOf(':', start);
			int end = readString.indexOf(';', start);

			String cmd = readString.substring(start + 1, seperator);
			String arg = readString.substring(seperator + 1, end);

			logger.verbose("Received: " + readString);
			logger.verbose("\t Command:" + cmd + " Argument:" + arg);
		}
	}

	String interruptServiceRoutine() {
		uint8_t buf[128]; //RH_NRF24_MAX_MESSAGE_LEN ???
		uint8_t len = sizeof(buf);
		if (nrf24.recv(buf, &len))
		{
			String readString = (char*)buf;
			logger.info("Message received: " + readString);
			return readString;
		}
		logger.error("Null message received on wireless communication ISR!");
		return "";
	}

private:
	NRF24 nrf24; // Singleton instance of the radio driver
	Logger logger;
};

#endif
