#include "CommunicationHandler.h"

CommunicationHandler::CommunicationHandler() {
}

bool CommunicationHandler::init() {
        if (!nrf24.init()) {//waits for radio driver (declared by code above aka receiver) to initialize
                Serial.println("init failed"); //if radio driver fails, prints error message
        }

        if (!nrf24.setChannel(1)) { // Defaults after init are 2.4 GHz (channel 1), 2Mbps, 0dBm
                Serial.println("setChannel failed");
        }

        if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)) {
                Serial.println("setRF failed");
        }
}

bool CommunicationHandler::receive() {
        if (nrf24.available())
        {
                char cmd[6];
                char arg[6];
                uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
                uint8_t len = sizeof(buf);
                if (nrf24.recv(buf, &len)) {
                        Serial.print("received: ");
                        Serial.println((char*)buf);
                        String message = (char*)buf;

                        int start = message.lastIndexOf('$');
                        int seperator = message.indexOf(':', start);
                        int end = message.indexOf(';', start);

                        command = message.substring(start + 1, seperator);
                        argument = message.substring(seperator + 1, end);
                        return message.length() > 0;
                }
                else
                {
                        Serial.println("Receive failed");
                }
        }
        return false;
}



void CommunicationHandler::sendMessage(String command, String argument) {
        String str = "$" + command + ":" + argument + ";";
        uint8_t data[sizeof(str)];
        str.toCharArray((char*)data, sizeof(data));
        //logger.info("Sending: " + String((char*)data));
        nrf24.send(data, sizeof(data));
        nrf24.waitPacketSent();
}

String CommunicationHandler::getCommand() {
  return command;
}

String CommunicationHandler::getArgument() {
  return argument;
}
