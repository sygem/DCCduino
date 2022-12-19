/**********************************************************************

Controller.cpp
COPYRIGHT (c) 2019 Simon Jewell

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/
/**********************************************************************



**********************************************************************/

#include "DCCpp.h"
#include "Controller.h"
#include "EEStore.h"
#include <EEPROM.h>
#include "CommInterface.h"

///////////////////////////////////////////////////////////////////////////////

void Controller::check(){
} // Controller::check

///////////////////////////////////////////////////////////////////////////////

Controller *Controller::create(int snum, int v) {
  Controller *tt;

  if(firstController==NULL){
    firstController=(Controller *)calloc(1,sizeof(Controller));
    tt=firstController;
  } else if((tt=get(snum))==NULL){
    tt=firstController;
    while(tt->nextController!=NULL)
      tt=tt->nextController;
    tt->nextController=(Controller *)calloc(1,sizeof(Controller));
    tt=tt->nextController;
  }

  if(tt==NULL){       // problem allocating memory
    if(v==1)
      CommManager::printf("<X Controller::create>");
    return(tt);
  }

  tt->data.snum=snum;
  //tt->data.pin=pin;
  //tt->data.min=min;
  //tt->data.max=max;

  if(v==1)
    CommManager::printf("<O>");
  return(tt);

}

///////////////////////////////////////////////////////////////////////////////

Controller* Controller::get(int n){
  Controller *tt;
  for(tt=firstController;tt!=NULL && tt->data.snum!=n;tt=tt->nextController);
  return(tt);
}
///////////////////////////////////////////////////////////////////////////////

void Controller::remove(int n){
  Controller *tt,*pp;

  for(tt=firstController;tt!=NULL && tt->data.snum!=n;pp=tt,tt=tt->nextController);

  if(tt==NULL){
    CommManager::printf("<X Controller::remove>");
    return;
  }

  if(tt==firstController)
    firstController=tt->nextController;
  else
    pp->nextController=tt->nextController;

  free(tt);

  CommManager::printf("<O>");
}

///////////////////////////////////////////////////////////////////////////////

void Controller::show(){
  Controller *tt;

  if(firstController==NULL){
    CommManager::printf("<X Controller::show>");
    return;
  }

  for(tt=firstController;tt!=NULL;tt=tt->nextController){
    //CommManager::printf("<Q %d %d %d>", tt->data.snum, tt->data.pin, tt->data.pullUp);
  }
}

///////////////////////////////////////////////////////////////////////////////

void Controller::status(){
  //Controller *tt;

  if(firstController==NULL){
    CommManager::printf("<X Controller::status>");
    return;
  }

  //for(tt=firstServo;tt!=NULL;tt=tt->nextServo){
  //  CommManager::printf("<%c %d>", tt->active?'Q':'q', tt->data.snum);
  //}
}

///////////////////////////////////////////////////////////////////////////////

void Controller::parse(const char *c) {
  int n,s,m;
  //Controller *t;

  switch(sscanf(c,"%d %d %d",&n,&s,&m)){

    case 2:                     // argument is string with id number of Servo followed by a pin number and pullUp indicator (0=LOW/1=HIGH)
      create(n,1);
    break;

    case 1:                     // argument is a string with id number only
      remove(n);
    break;

    case -1:                    // no arguments
      show();
    break;

    case 3:                     // invalid number of arguments
      CommManager::printf("<X Controller::parse>");
      break;
  }
}

///////////////////////////////////////////////////////////////////////////////

void Controller::load(){
  struct ControllerData data;
  Controller *tt;

  for(int i=0;i<EEStore::eeStore->data.nControllers;i++){
    EEPROM.get(EEStore::pointer(),data);
    tt=create(data.snum);
    EEStore::advance(sizeof(tt->data));
  }
}

///////////////////////////////////////////////////////////////////////////////

void Controller::store(){
  Controller *tt;

  tt=firstController;
  EEStore::eeStore->data.nControllers=0;

  while(tt!=NULL){
    EEPROM.put(EEStore::pointer(),tt->data);
    EEStore::advance(sizeof(tt->data));
    tt=tt->nextController;
    EEStore::eeStore->data.nControllers++;
  }
}

///////////////////////////////////////////////////////////////////////////////

Controller *Controller::firstController=NULL;
