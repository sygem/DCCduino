/**********************************************************************

LightGroup.cpp
COPYRIGHT (c) 2019 Simon Jewell

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/
/**********************************************************************



**********************************************************************/

#include "DCCpp.h"
#include "LightGroup.h"
#include "EEStore.h"
#include <EEPROM.h>
#include "CommInterface.h"
#include "Sensor.h"

///////////////////////////////////////////////////////////////////////////////

void LightGroup::check(){
} // LightGroup::check

///////////////////////////////////////////////////////////////////////////////

LightGroup *LightGroup::create(int snum, int pin, int brightness, int v) {
  LightGroup *tt;

  if(firstLightGroup==NULL){
    firstLightGroup=(LightGroup *)calloc(1,sizeof(LightGroup));
    tt=firstLightGroup;
  } else if((tt=get(snum))==NULL){
    tt=firstLightGroup;
    while(tt->nextLightGroup!=NULL)
      tt=tt->nextLightGroup;
    tt->nextLightGroup=(LightGroup *)calloc(1,sizeof(LightGroup));
    tt=tt->nextLightGroup;
  }

  if(tt==NULL){       // problem allocating memory
    if(v==1)
      CommManager::printf("<X LightGroup::create>");
    return(tt);
  }

  tt->data.snum=snum;

  if(v==1)
    CommManager::printf("<O>");
  return(tt);

}

///////////////////////////////////////////////////////////////////////////////

LightGroup* LightGroup::get(int n){
  LightGroup *tt;
  for(tt=firstLightGroup;tt!=NULL && tt->data.snum!=n;tt=tt->nextLightGroup);
  return(tt);
}
///////////////////////////////////////////////////////////////////////////////

void LightGroup::remove(int n){
  LightGroup *tt,*pp;

  for(tt=firstLightGroup;tt!=NULL && tt->data.snum!=n;pp=tt,tt=tt->nextLightGroup);

  if(tt==NULL){
    CommManager::printf("<X LightGroup::remove>");
    return;
  }

  if(tt==firstLightGroup)
    firstLightGroup=tt->nextLightGroup;
  else
    pp->nextLightGroup=tt->nextLightGroup;

  free(tt);

  CommManager::printf("<O>");
}

///////////////////////////////////////////////////////////////////////////////

void LightGroup::show(){
  LightGroup *tt;

  if(firstLightGroup==NULL){
    CommManager::printf("<X LightGroup::show>");
    return;
  }

  for(tt=firstLightGroup;tt!=NULL;tt=tt->nextLightGroup){
    //CommManager::printf("<V %d %d %d>", tt->data.snum, tt->data.pin, tt->data.brightness);
  }
}

///////////////////////////////////////////////////////////////////////////////

void LightGroup::status(){
  //LightGroup *tt;

  if(firstLightGroup==NULL){
    CommManager::printf("<X LightGroup::status>");
    return;
  }

  //for(tt=firstLightGroup;tt!=NULL;tt=tt->nextLightGroup){
  //  CommManager::printf("<%c %d>", tt->active?'Q':'q', tt->data.snum);
  //}
}

///////////////////////////////////////////////////////////////////////////////

void LightGroup::parse(const char *c) {
  int n,s;
  //LightGroup *t;

  switch(sscanf(c,"%d %d",&n,&s)){

    case 2:                     // argument is string with id number of LightGroup followed by a pin number and pullUp indicator (0=LOW/1=HIGH)
      create(n,s);
    break;

    case 1:                     // argument is a string with id number only
      remove(n);
    break;

    case -1:                    // no arguments
      show();
    break;

  }
}

///////////////////////////////////////////////////////////////////////////////

void LightGroup::load(){
  /*struct LightGroupData data;
  LightGroup *tt;

  for(int i=0;i<EEStore::eeStore->data.nLightGroups;i++){
    EEPROM.get(EEStore::pointer(),data);
    tt=create(data.snum,data.pin,data.brightness);
    EEStore::advance(sizeof(tt->data));
  }*/
}

///////////////////////////////////////////////////////////////////////////////

void LightGroup::store(){
  /*LightGroup *tt;

  tt=firstLightGroup;
  EEStore::eeStore->data.nLightGroups=0;

  while(tt!=NULL){
    EEPROM.put(EEStore::pointer(),tt->data);
    EEStore::advance(sizeof(tt->data));
    tt=tt->nextLightGroup;
    EEStore::eeStore->data.nLightGroups++;
  }*/
}

///////////////////////////////////////////////////////////////////////////////

void LightGroup::setBrightness(Adafruit_PWMServoDriver pwm, int brightness) {
  /*data.brightness = brightness;
  pwm.setPWM(data.pin,0,brightness);
  #ifdef ENABLE_LCD
  lcdDisplay.clear();
  lcdDisplay.setCursor(0, 0);
  lcdDisplay.print("LightGroup ");
  lcdDisplay.print(data.snum);
  lcdDisplay.print(" Pin ");
  lcdDisplay.print(data.pin);
  lcdDisplay.setCursor(0, 1);
  lcdDisplay.print("Brightness ");
  lcdDisplay.print(data.brightness);
  #endif
  delay(20);*/
}

LightGroup *LightGroup::firstLightGroup=NULL;
