/**********************************************************************

Preset.cpp
COPYRIGHT (c) 2019 Simon Jewell

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/
/**********************************************************************



**********************************************************************/

#include "Preset.h"

///////////////////////////////////////////////////////////////////////////////

void Preset::parse(const char *c) {
  int n;

  switch(sscanf(c,"%d",&n)){

    case 1:                     // argument is a string with id number only
      ApplyPreset(n);
    break;

    case -1:                    // no arguments
      ListPresets();
    break;

  }
}

void Preset::ListPresets() {

}

void Preset::ApplyPreset(int p) {
    switch (p) {
        case 0:
        break;
    }
}

