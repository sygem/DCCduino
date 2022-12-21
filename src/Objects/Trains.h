/**********************************************************************

Train.h
COPYRIGHT (c) 2019 Simon Jewell

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/

#ifndef Train_h
#define Train_h

#include "Arduino.h"

struct TrainData { // TrainData is persisted across sessions
  int tnum;
  int id;
  uint8_t f0;
};

struct TrainState { // TrainState is not persisted
  uint8_t speed;
  uint8_t direction;
  uint8_t lights;
};

struct Train {

  static Train *firstTrain;
  TrainData data;
  TrainState state;
  Train *nextTrain;
  static void load();
  static void store();
  static Train *create(int, int, uint8_t=0, int=1);
  static Train* get(int);  
  static void remove(int);  
  static void show();
  static void status();
  static void parse(const char *c);
  static void check();   
  static void setThrottle(const char *c);
  static void setFunction(const char *c);
  String getName();
  void setName(String name);

}; // Servo

#endif

