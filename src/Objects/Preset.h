/**********************************************************************

Preset.h
COPYRIGHT (c) 2019 Simon Jewell

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/

#ifndef Preset_h
#define Preset_h

#include "Arduino.h"

struct Preset {

  static void parse(const char *c);
  static void ListPresets();
  static void ApplyPreset(int p);

}; // Preset

#endif

