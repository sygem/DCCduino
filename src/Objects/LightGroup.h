/**********************************************************************

LightGroup.h
COPYRIGHT (c) 2019 Simon Jewell

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/

#ifndef LightGroup_h
#define LightGroup_h

#include "Arduino.h"
#include <Adafruit_PWMServoDriver.h>

struct LightGroupData {
  int snum;
  unsigned pin1 : 1;
  unsigned pin2 : 1;
};

struct LightGroup {

  static LightGroup *firstLightGroup;
  LightGroupData data;
  LightGroup *nextLightGroup;
  static void load();
  static void store();
  static LightGroup *create(int, int, int=0, int=1);
  static LightGroup* get(int);  
  static void remove(int);  
  static void show();
  static void status();
  static void parse(const char *c);
  static void check();   
  void setBrightness(Adafruit_PWMServoDriver pwm, int brightness);
}; // LightGroup

#endif

