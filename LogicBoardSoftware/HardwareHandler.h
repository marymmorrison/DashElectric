#ifndef HardwareHandler_h
#define HardwareHandler_h

#include "Arduino.h"
#include "Pins.h"
#include <Servo.h>

class HardwareHandler
{
public:
	HardwareHandler(){
	};

	void init() {
    servo.attach(9); // pin the the Servo is connected to
    servo.write(99);
    delay(6000); // ESC needs a few seconds to initialize
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
			//logger.error("Out of range exception!");
			//logger.error("\t Rider mode is " + value);
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

 void setSpeed(short s) {
    speed = s;
 }

  void monitorSpeed() {
    int potentioval = map(speed, 0, 100, 90, 140);
        if (riderMode == 1) {
                if (potentioval > 115) {
                        potentioval = 115;
                }
                desired = potentioval;
                if (abs(desired - currentspeed) > 3) {
                        if ((desired - currentspeed) < 0) {
                                accel = -.5; //for decceleration
                        }
                        else {
                                accel = .5;
                        }
                }
                else {
                        accel = desired - currentspeed;
                }
                currentspeed = currentspeed + accel;
                servo.write(currentspeed);
        }

        if (riderMode == 2) {
                if (potentioval > 125)
                {
                        potentioval = 125;
                }
                desired = potentioval;
                if (abs(desired - currentspeed) > 6) {
                        if ((desired - currentspeed) < 0) {
                                accel = -4; //for decceleration
                        }
                        else {
                                accel = 4;
                        }
                }
                else{
                        accel = desired - currentspeed;
                }
                currentspeed = currentspeed + accel;
                servo.write(currentspeed);
        }

        if (riderMode == 3) {
                if (potentioval > 140)
                {
                        potentioval = 140;
                }
                desired = potentioval;
                if (abs(desired - currentspeed) > 10) {
                        if ((desired - currentspeed) < 0) {
                                accel = -10; //for deceleration
                        }
                        else {
                                accel = 10;
                        }
                } else {
                        accel = desired - currentspeed;
                }
                currentspeed = currentspeed + accel;
                servo.write(currentspeed);
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
  short speed = 0;
  int potentioval = 0;
  float accel = 0;
  float currentspeed = 90;
  int desired = 0;
  short riderMode = 1;
  Servo servo;

	Logger logger;
};

#endif
