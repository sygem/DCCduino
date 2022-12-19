/**********************************************************************

Servo.cpp
COPYRIGHT (c) 2019 Simon Jewell

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/
/**********************************************************************



**********************************************************************/

#include "DCCpp.h"
#include "Servo.h"
#include "EEStore.h"
#include <EEPROM.h>
#include "CommInterface.h"
#include "Sensor.h"

///////////////////////////////////////////////////////////////////////////////

void Servo::check(){
} // Servo::check

///////////////////////////////////////////////////////////////////////////////

Servo *Servo::create(int snum, int pin, int min, int max, int v) {
  Servo *tt;

  if(firstServo==NULL){
    firstServo=(Servo *)calloc(1,sizeof(Servo));
    tt=firstServo;
  } else if((tt=get(snum))==NULL){
    tt=firstServo;
    while(tt->nextServo!=NULL)
      tt=tt->nextServo;
    tt->nextServo=(Servo *)calloc(1,sizeof(Servo));
    tt=tt->nextServo;
  }

  if(tt==NULL){       // problem allocating memory
    if(v==1)
      CommManager::printf("<X Servo::create>");
    return(tt);
  }

  tt->data.snum=snum;
  tt->data.pin=pin;
  tt->data.min=min;
  tt->data.max=max;

  if(v==1)
    CommManager::printf("<O>");
  return(tt);

}

///////////////////////////////////////////////////////////////////////////////

Servo* Servo::get(int n){
  Servo *tt;
  for(tt=firstServo;tt!=NULL && tt->data.snum!=n;tt=tt->nextServo);
  return(tt);
}
///////////////////////////////////////////////////////////////////////////////

void Servo::remove(int n){
  Servo *tt,*pp;

  for(tt=firstServo;tt!=NULL && tt->data.snum!=n;pp=tt,tt=tt->nextServo);

  if(tt==NULL){
    CommManager::printf("<X Servo::remove>");
    return;
  }

  if(tt==firstServo)
    firstServo=tt->nextServo;
  else
    pp->nextServo=tt->nextServo;

  free(tt);

  CommManager::printf("<O>");
}

///////////////////////////////////////////////////////////////////////////////

void Servo::show(){
  Servo *tt;

  if(firstServo==NULL){
    CommManager::printf("<X Servo::show>");
    return;
  }

  for(tt=firstServo;tt!=NULL;tt=tt->nextServo){
    CommManager::printf("<V %d %d %d %d>", tt->data.snum, tt->data.pin, tt->data.min, tt->data.max);
  }
}

///////////////////////////////////////////////////////////////////////////////

void Servo::status(){
  //Servo *tt;

  if(firstServo==NULL){
    CommManager::printf("<X Servo::status>");
    return;
  }

  //for(tt=firstServo;tt!=NULL;tt=tt->nextServo){
  //  CommManager::printf("<%c %d>", tt->active?'Q':'q', tt->data.snum);
  //}
}

///////////////////////////////////////////////////////////////////////////////

void Servo::parse(const char *c) {
  int n,s,min,max;
  //Servo *t;

  switch(sscanf(c,"%d %d %d %d",&n,&s,&min,&max)){

    case 4:                     // argument is string with id number of Servo followed by a pin number and pullUp indicator (0=LOW/1=HIGH)
      create(n,s,min,max,1);
    break;

    case 1:                     // argument is a string with id number only
      remove(n);
    break;

    case -1:                    // no arguments
      show();
    break;

    case 2:                     // invalid number of arguments
      CommManager::printf("<X Servo::parse>");
      break;
  }
}

///////////////////////////////////////////////////////////////////////////////

void Servo::load(){
  struct ServoData data;
  Servo *tt;

  for(int i=0;i<EEStore::eeStore->data.nServos;i++){
    EEPROM.get(EEStore::pointer(),data);
    tt=create(data.snum,data.pin,data.min,data.max);
    EEStore::advance(sizeof(tt->data));
  }
}

///////////////////////////////////////////////////////////////////////////////

void Servo::store(){
  Servo *tt;

  tt=firstServo;
  EEStore::eeStore->data.nServos=0;

  while(tt!=NULL){
    EEPROM.put(EEStore::pointer(),tt->data);
    EEStore::advance(sizeof(tt->data));
    tt=tt->nextServo;
    EEStore::eeStore->data.nServos++;
  }
}

///////////////////////////////////////////////////////////////////////////////

void Servo::turnOn(Adafruit_PWMServoDriver pwm) {
  pwm.setPWM(data.pin,0,data.max);
  #ifdef ENABLE_LCD
  #if LCD_LINES > 2
  int y = 2;
  #else
  int y = 0;
  #endif
  lcdDisplay.clear();
  lcdDisplay.setCursor(0, y);
  lcdDisplay.print("Servo ");
  lcdDisplay.print(data.snum);
  lcdDisplay.print(" Pin ");
  lcdDisplay.print(data.pin);
  lcdDisplay.setCursor(0, y+1);
  lcdDisplay.print("Turned ON");
  #endif
  turnOffAfter = millis() + 200;
}

void Servo::turnOff(Adafruit_PWMServoDriver pwm) {
  pwm.setPWM(data.pin,0,data.min);
  #ifdef ENABLE_LCD
  #if LCD_LINES > 2
  int y = 2;
  #else
  int y = 0;
  #endif
  lcdDisplay.clear();
  lcdDisplay.setCursor(0, y);
  lcdDisplay.print("Servo ");
  lcdDisplay.print(data.snum);
  lcdDisplay.print(" Pin ");
  lcdDisplay.print(data.pin);
  lcdDisplay.setCursor(0, y+1);
  lcdDisplay.print("Turned OFF");
  #endif
  turnOffAfter = millis() + 200;
}

void Servo::switchOff(Adafruit_PWMServoDriver pwm) {
  pwm.setPWM(data.pin,0,4096);
  turnOffAfter = -1;
}

Servo *Servo::firstServo=NULL;
