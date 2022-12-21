/*
 * CommInterfaceSerial.h
 *
 *  Created on: Feb 4, 2019
 *      Author: simonjewell
 */

#ifndef COMMINTERFACESENSORS_H_
#define COMMINTERFACESENSORS_H_

#include "CommInterface.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

class SensorsInterface : public CommInterface {
public:
	SensorsInterface();
	void process();
    void showConfiguration();
	void showInitInfo();
	void send(const char *buf);
private:
	void TurnOnServo(int n);
	void TurnOffServo(int n);
	void SetLightBrightness(int n, int b);
	void SetLightGroupBrightness(int n, int b);
protected:
     Adafruit_PWMServoDriver servo_pwm = Adafruit_PWMServoDriver();
     Adafruit_PWMServoDriver light1_pwm = Adafruit_PWMServoDriver(0x41, Wire);
     Adafruit_PWMServoDriver light2_pwm = Adafruit_PWMServoDriver(0x42, Wire);
};

#endif /* COMMINTERFACESENSORS_H_ */
