/**********************************************************************

Light.cpp
COPYRIGHT (c) 2019 Simon Jewell

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/
/**********************************************************************



**********************************************************************/

#include "DCCpp.h"
#include "Light.h"
#include "EEStore.h"
#include <EEPROM.h>
#include "CommInterface.h"
#include "Sensor.h"

///////////////////////////////////////////////////////////////////////////////

void Light::check(){
} // Light::check

///////////////////////////////////////////////////////////////////////////////

Light *Light::create(int snum, int pwm, int pin, int v) {
  CommManager::printf("Create Light\n");
  Light *tt;

  if (firstLight==NULL) {
    firstLight=(Light *)calloc(1,sizeof(Light));
    tt=firstLight;
  } else if ((tt=get(snum))==NULL) {
    tt=firstLight;
    while(tt->nextLight!=NULL) {
      tt=tt->nextLight;
    }
    tt->nextLight=(Light *)calloc(1,sizeof(Light));
    tt=tt->nextLight;
  }

  if (tt==NULL) {       // problem allocating memory
    if (v==1) {
      CommManager::printf("<X Light::create>");
    }
    return(tt);
  }

  tt->data.snum=snum;
  tt->data.pwm=pwm;
  tt->data.pin=pin;
  tt->data.brightness=0;

  if (v==1) {
    CommManager::printf("<O>");
  }
  return(tt);

}

///////////////////////////////////////////////////////////////////////////////

Light* Light::get(int n){
  Light *tt;
  for (tt=firstLight;tt!=NULL && tt->data.snum!=n;tt=tt->nextLight);
  return(tt);
}
///////////////////////////////////////////////////////////////////////////////

void Light::remove(int n){
  Light *tt,*pp;

  for (tt=firstLight;tt!=NULL && tt->data.snum!=n;pp=tt,tt=tt->nextLight);

  if (tt==NULL) {
    CommManager::printf("<X Light::remove>");
    return;
  }

  if (tt==firstLight) {
    firstLight=tt->nextLight;
  } else {
    pp->nextLight=tt->nextLight;
  }

  free(tt);

  CommManager::printf("<O>");
}

///////////////////////////////////////////////////////////////////////////////

void Light::show(){
  Light *tt;

  if (firstLight==NULL) {
    CommManager::printf("<X Light::show>");
    return;
  }

  for (tt=firstLight;tt!=NULL;tt=tt->nextLight) {
    CommManager::printf("<L %d %d %d>", tt->data.snum, tt->data.pin, tt->data.brightness);
  }
}

///////////////////////////////////////////////////////////////////////////////

void Light::status(){
  //Light *tt;

  if (firstLight==NULL) {
    CommManager::printf("<X Light::status>");
    return;
  }

  //for(tt=firstLight;tt!=NULL;tt=tt->nextLight){
  //  CommManager::printf("<%c %d>", tt->active?'Q':'q', tt->data.snum);
  //}
}

///////////////////////////////////////////////////////////////////////////////

void Light::parse(const char *c) {
  //CommManager::printf("<X Light::parse>");
  int n,pin,pwm;
  Light *s;

  switch(sscanf(c,"%d %d %d",&n,&pwm,&pin)) {

    case 3:                     // argument is string with id number of Light followed by a pin number and pullUp indicator (0=LOW/1=HIGH)
      create(n,pwm,pin,0);
    break;

    case 2:
      s = get(n);
      if (s != NULL) {
        CommManager::printf("<I %d %d>", n, pwm);
      } else {
        CommManager::printf("<X Light::setBrightness>");
      }
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

void Light::load(){
  struct LightData data;
  Light *tt;

  for(int i=0;i<EEStore::eeStore->data.nLights;i++){
    EEPROM.get(EEStore::pointer(),data);
    tt=create(data.snum,data.pwm,data.pin,data.brightness);
    EEStore::advance(sizeof(tt->data));
  }
}

///////////////////////////////////////////////////////////////////////////////

void Light::store(){
  Light *tt;

  tt=firstLight;
  EEStore::eeStore->data.nLights=0;

  while(tt!=NULL){
    EEPROM.put(EEStore::pointer(),tt->data);
    EEStore::advance(sizeof(tt->data));
    tt=tt->nextLight;
    EEStore::eeStore->data.nLights++;
  }
}

///////////////////////////////////////////////////////////////////////////////

void Light::setBrightness(Adafruit_PWMServoDriver pwm, int brightness) {
  data.brightness = brightness;
  pwm.setPWM(data.pin,0,brightness);
  #ifdef ENABLE_LCD
  lcdDisplay.clear();
  lcdDisplay.setCursor(0, 0);
  lcdDisplay.print("Light ");
  lcdDisplay.print(data.snum);
  lcdDisplay.print(" Pin ");
  lcdDisplay.print(data.pin);
  lcdDisplay.setCursor(0, 1);
  lcdDisplay.print("Brightness ");
  lcdDisplay.print(data.brightness);
  #endif
  delay(20);
}

Light *Light::firstLight=NULL;
