#ifndef CommunicationHandler_h
#define CommunicationHandler_h

#include <NRF24.h>
#include <RH_NRF24.h>
#include "Arduino.h"
#include "Logger.h"
#include "Pins.h"


class CommunicationHandler
{
public:
	CommunicationHandler();
	bool init();
	void sendMessage(String command, String argument);
	bool available(){
	};
	void processMessage(String message);
	bool receive();
 String getCommand();
 String getArgument();

private:
	RH_NRF24 nrf24; // Singleton instance of the radio driver
	String command;
	String argument;
};

#endif
