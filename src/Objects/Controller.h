/**********************************************************************

Controller.h
COPYRIGHT (c) 2019 Simon Jewell

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/

#ifndef Controller_h
#define Controller_h

#include "Arduino.h"

struct ControllerData { 
    int snum;
    int trainA;
    int trainB;
    int style;
};

struct Controller {

  static Controller *firstController;
  ControllerData data;
  Controller *nextController;
  static void load();
  static void store();
  static Controller *create(int, int=1);
  static Controller* get(int);  
  static void remove(int);  
  static void show();
  static void status();
  static void parse(const char *c);
  static void check();   

}; // Controller

#endif

