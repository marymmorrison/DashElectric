#ifndef CommunicationHandler_h
#define CommunicationHandler_h

#include <NRF24.h>
#include "Arduino.h"
#include "Logger.h"
#include "Pins.h"


class CommunicationHandler
{
public:
	CommunicationHandler();
	bool init(){};
	void sendMessage(String command, String argument);
	bool available(){};
	void processMessage(String message);
	void receive();

private:
	NRF24 nrf24; // Singleton instance of the radio driver
	Logger logger;
};

#endif
