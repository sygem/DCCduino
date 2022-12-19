/*
 * CommInterfaceSensors.cpp
 *
 *  Created on: Feb 4, 2019
 *      Author: simonjewell
 */

#include <Arduino.h>
#include "CommInterfaceSensors.h"
#include "Sensor.h"
#include "DCCpp.h"
#include "Objects/Servo.h"
#include "EEStore.h"

SensorsInterface::SensorsInterface() {
#ifndef ARDUINO_AVR_UNO

  servo_pwm.begin();
  servo_pwm.setPWMFreq(60);
  delay(10);

#endif

}

void SensorsInterface::process() {
  Servo *tt;
  for(tt=Servo::firstServo;tt!=NULL;tt=tt->nextServo){
    if (tt->turnOffAfter != -1 && millis() > tt->turnOffAfter) {
      tt->switchOff(servo_pwm);
    }
  }
}

void SensorsInterface::showConfiguration() {
	Serial.println("Sensors:");
	Sensor::status();
  Serial.println("Servos:");
  Servo::status();
}

void SensorsInterface::showInitInfo() {
	CommManager::printf("<N0:SENSORS>");
}

void SensorsInterface::send(const char *buf) {
    String buffer(buf);
    if (buffer.startsWith("<") && buffer.endsWith(">")) {
        buffer = buffer.substring(1,buffer.length()-1);
        int n,min,max;
        switch(buffer[0]){

            case 'Q':
            switch(sscanf(buffer.c_str()+1,"%d",&n)){

              case 1:                     // argument is a string with id number only
                TurnOnServo(n);
                CommManager::printf("<O>");
              break;
              default:
                CommManager::printf("<X Sensors::send 1>");
                break;
            }
            break;
            case 'q':
            switch(sscanf(buffer.c_str()+1,"%d",&n)){

              case 1:                     // argument is a string with id number only
                TurnOffServo(n);
                CommManager::printf("<O>");
              break;
              default:
                CommManager::printf("<X Sensors::send 2>");
                break;
            }
            break;
            case 'K': {
              switch(sscanf(buffer.c_str()+1,"%d %d %d",&n,&min,&max)) {
                case 3:
                  {
                    Servo* s = Servo::get(n);
                    if (min < Servo::SERVOMIN) min = Servo::SERVOMIN;
                    if (max > Servo::SERVOMAX) max = Servo::SERVOMAX;
                    if (min > max) min = max;
                    s->data.min = min;
                    s->data.max = max;
                  }
                  {
                    Sensor* ss = Sensor::get(n);
                    if (ss->active) {
                      TurnOnServo(n);
                    } else {
                      TurnOffServo(n);
                    }
                    CommManager::printf("<O %d %d>",min,max);
                  }
                  EEStore::store();
                break;
                default:
                  CommManager::printf("<X Sensors::send 3>");
                  break;
              }
            }
            break;
        }

    }
}

void SensorsInterface::TurnOnServo(int n) {
  Servo* s = Servo::get(n);
  if (s != NULL) {
    s->turnOn(servo_pwm);
  } else {
    //CommManager::printf("<X Sensors::TurnOnServo>");
  }
}

void SensorsInterface::TurnOffServo(int n) {
  Servo* s = Servo::get(n);
  if (s != NULL) {
    s->turnOff(servo_pwm);
  } else {
    //CommManager::printf("<X Sensors::TurnOffServo>");
  }
}