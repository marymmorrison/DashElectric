#include "CommunicationHandler.h"

CommunicationHandler::CommunicationHandler (){
};


void CommunicationHandler::sendMessage(String command, String argument) {
        String str = "$" + command + ":" + argument + ";";
        uint8_t data[sizeof(str)];
        str.toCharArray((char*)data, sizeof(data));
        Serial.println("Sending: ");
        Serial.print((char*)data);
        nrf24.send(data, sizeof(data));
        nrf24.waitPacketSent();
}


void CommunicationHandler::receive() {
        char cmd[6];
        char arg[6];
        uint8_t buf[64];
        uint8_t len = sizeof(buf);
        if (nrf24.recv(buf, &len)) {
                for(int i = 0; i < sizeof(buf); ++i) {
                        if(buf[i] == 36) { // $
                                for(int j = 0; buf[i] != 36 || buf[i] != 58; ++i) {
                                        cmd[j] = buf[i];
                                        j++;
                                }
                        }
                        if(buf[i] == 58) { // :
                                for(int j = 0; buf[i] != 58 || buf[i] != 59; ++i) {
                                        arg[j] = buf[i];
                                        j++;
                                }
                        }
                }
                logger.info(String((char*)buf) + "received");
                logger.info("\tCommand: " + String((char*)cmd));
                logger.info("\tArgument: " + String((char*)arg));
        }
        //logger.warn("Null message received on wireless communication ISR.");
        logger.verbose("No data to be received");
}

void CommunicationHandler::processMessage(String message)
{
        int start = message.lastIndexOf('$');
        int seperator = message.indexOf(':', start);
        int end = message.indexOf(';', start);

        String cmd = message.substring(start + 1, seperator);
        String arg = message.substring(seperator + 1, end);

        logger.verbose("Received: " + message);
        logger.verbose("\t Command:" + cmd + " Argument:" + arg);
}
