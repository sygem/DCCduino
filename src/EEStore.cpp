/**********************************************************************

EEStore.cpp
COPYRIGHT (c) 2013-2016 Gregg E. Berman

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/

#include "DCCpp.h"
#include "EEStore.h"
#include "Accessories.h"
#include "Sensor.h"
#include "Outputs.h"
#include "Objects/Trains.h"
#include "Objects/Controller.h"
#include "Objects/Servo.h"
#include "Objects/Light.h"
#include "Objects/LightGroup.h"
#include <EEPROM.h>

///////////////////////////////////////////////////////////////////////////////

void EEStore::init(){


  eeStore=(EEStore *)calloc(1,sizeof(EEStore));

  EEPROM.get(0,eeStore->data);                                       // get eeStore data

  if (strncmp(eeStore->data.id,EESTORE_ID,sizeof(EESTORE_ID))!=0) {    // check to see that eeStore contains valid DCC++ ID
    sprintf(eeStore->data.id,EESTORE_ID);                           // if not, create blank eeStore structure (no turnouts, no sensors) and save it back to EEPROM
    eeStore->data.nTurnouts=0;
    eeStore->data.nSensors=0;
    eeStore->data.nOutputs=0;
    eeStore->data.nServos=0;
    eeStore->data.nTrains=0;
    eeStore->data.nControllers=0;
    eeStore->data.nLights=0;
    eeStore->data.nLightGroups=0;
    EEPROM.put(0,eeStore->data);
  }

  reset();            // set memory pointer to first free EEPROM space
  Turnout::load();    // load turnout definitions
  Sensor::load();     // load sensor definitions
  Output::load();     // load output definitions
  Servo::load();      // load servo definitions
  Train::load();
  Controller::load();
  Light::load();
  LightGroup::load();
}

///////////////////////////////////////////////////////////////////////////////

void EEStore::clear(){

  sprintf(eeStore->data.id,EESTORE_ID);                           // create blank eeStore structure (no turnouts, no sensors) and save it back to EEPROM
  eeStore->data.nTurnouts=0;
  eeStore->data.nSensors=0;
  eeStore->data.nOutputs=0;
  eeStore->data.nServos=0;
  eeStore->data.nTrains=0;
  eeStore->data.nControllers=0;
  eeStore->data.nLights=0;
  eeStore->data.nLightGroups=0;
  EEPROM.put(0,eeStore->data);

}

///////////////////////////////////////////////////////////////////////////////

void EEStore::store(){
  reset();
  Turnout::store();
  Sensor::store();
  Output::store();
  Servo::store();
  Train::store();
  Light::store();
  LightGroup::store();
  Controller::store();
  EEPROM.put(0,eeStore->data);
}

///////////////////////////////////////////////////////////////////////////////

void EEStore::advance(int n){
  eeAddress+=n;
}

///////////////////////////////////////////////////////////////////////////////

void EEStore::reset(){
  eeAddress=sizeof(EEStore);
}
///////////////////////////////////////////////////////////////////////////////

int EEStore::pointer(){
  return(eeAddress);
}
///////////////////////////////////////////////////////////////////////////////

EEStore *EEStore::eeStore=NULL;
int EEStore::eeAddress=0;
