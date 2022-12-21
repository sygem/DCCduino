/**********************************************************************

Servo.h
COPYRIGHT (c) 2019 Simon Jewell

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/

#ifndef Servo_h
#define Servo_h

#include "Arduino.h"
#include <Adafruit_PWMServoDriver.h>

struct ServoData {
  int snum;
  int pin;
  int min;
  int max;
};

struct Servo {
  static const int SERVOMIN = 110;
  static const int SERVOMAX = 515;

  static Servo *firstServo;
  ServoData data;
  unsigned long turnOffAfter;
  Servo *nextServo;
  static void load();
  static void store();
  static Servo *create(int, int, int=SERVOMIN, int=SERVOMAX, int=1);
  static Servo* get(int);  
  static void remove(int);  
  static void show();
  static void show(int);
  static void status();
  static void parse(const char *c);
  static void check();   
  void turnOn(Adafruit_PWMServoDriver pwm);
  void turnOff(Adafruit_PWMServoDriver pwm);
  void switchOff(Adafruit_PWMServoDriver pwm);
}; // Servo

#endif

