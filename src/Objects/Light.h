/**********************************************************************

Light.h
COPYRIGHT (c) 2019 Simon Jewell

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/

#ifndef Light_h
#define Light_h

#include "Arduino.h"
#include <Adafruit_PWMServoDriver.h>

struct LightData {
  int snum;
  int pwm;
  int pin;
  int brightness;
};

struct Light {

  static Light *firstLight;
  LightData data;
  Light *nextLight;
  static void load();
  static void store();
  static Light *create(int, int, int, int=1);
  static Light* get(int);  
  static void remove(int);  
  static void show();
  static void status();
  static void parse(const char *c);
  static void check();   
  void setBrightness(Adafruit_PWMServoDriver pwm, int brightness);
}; // Light

#endif

