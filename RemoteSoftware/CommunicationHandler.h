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

	bool init() {
		// setup interrupts
		attachInterrupt(digitalPinToInterrupt(nrf24Interrupt), interruptServiceRoutine, HIGH);
		return nrf24.init();
	}

	void sendMessage(String command, String argument) {
		String str = "$" + command + ":" + argument + ";";
		int8_t data[sizeof(str)];
		str.toCharArray(data, sizeof(data));
		Serial.println("Sending: ");
		Serial.print((char*)data);
		nrf24.send(data, sizeof(data));
		nrf24.waitPacketSent();
	}

	bool available() {
		if (!nrf24.available())
		{
			logger.warn("Wireless communication not available")
			return false;
		}
		return bufferIndex > 0;
	}

	void processCommand()
	{

		String readString = buffer[bufferIndex];
		buffer[bufferIndex] = "";
		bufferIndex--;

		int start = readString.lastIndexOf('$');
		int seperator = readString.indexOf(':', start);
		int end = readString.indexOf(';', start);

		String cmd = readString.substring(start + 1, seperator);
		String arg = readString.substring(seperator + 1, end);

		logger.verbose("Received: " + readString);
		logger.verbose("\t Command:" + cmd + " Argument:" + arg);
	}


private:
	NRF24 nrf24; // Singleton instance of the radio driver
	Logger logger;
	volatile String buffer[16];
	volatile int bufferIndex = 0;

	void interruptServiceRoutine() {
		uint8_t buf[128]; //RH_NRF24_MAX_MESSAGE_LEN ???
		uint8_t len = sizeof(buf);
		if (nrf24.recv(buf, &len))
		{
			String readString = (char*)buf;
			if(bufferIndex < sizeof(buffer))
			{
				buffer[bufferIndex] = readString;
				bufferIndex++;
			}
			else
			{
				logger.error("Buffer overflow from communication ISR!")
			}
			logger.info("Message received: " + readString);
		}
		logger.warn("Null message received on wireless communication ISR.");
	}
};

#endif
