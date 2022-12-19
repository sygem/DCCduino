/**********************************************************************

Train.cpp
COPYRIGHT (c) 2019 Simon Jewell

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/
/**********************************************************************



**********************************************************************/

#include "DCCpp.h"
#include "Objects/Trains.h"
#include "EEStore.h"
#include <EEPROM.h>
#include "CommInterface.h"
#include "SerialCommand.h"

///////////////////////////////////////////////////////////////////////////////
const int LIGHTS_CHECK = 500;
static unsigned long lastTime = 0;


void Train::check(){
  Train *tt;

  if (millis() - lastTime > LIGHTS_CHECK) {
    lastTime = millis();
    if (firstTrain==NULL) {
        //CommManager::printf("<X Train::check>");
        return;
    }

    for (tt=firstTrain;tt!=NULL;tt=tt->nextTrain) {
        if (tt->state.lights >= 1) {
            char c[20];
            sprintf(c,"f %d %d",tt->data.id,144); // what about functions F1-F4?
            SerialCommand::parse(c);
        }
    }

  }
} // Train::check

///////////////////////////////////////////////////////////////////////////////

Train *Train::create(int tnum, int id, uint8_t f0, int v) {
  Train *tt;

  if(firstTrain==NULL){
    firstTrain=(Train *)calloc(1,sizeof(Train));
    tt=firstTrain;
  } else if((tt=get(id))==NULL){
    tt=firstTrain;
    while(tt->nextTrain!=NULL)
      tt=tt->nextTrain;
    tt->nextTrain=(Train *)calloc(1,sizeof(Train));
    tt=tt->nextTrain;
  }

  if(tt==NULL){       // problem allocating memory
    if(v==1)
      CommManager::printf("<X Train::create>");
    return(tt);
  }

  tt->data.tnum=tnum;
  tt->data.id=id;
  tt->data.f0=f0;

  if(v==1)
    CommManager::printf("<O>");
  return(tt);

}

///////////////////////////////////////////////////////////////////////////////

Train* Train::get(int n){
  Train *tt;
  for(tt=firstTrain;tt!=NULL && tt->data.id!=n;tt=tt->nextTrain);
  return(tt);
}
///////////////////////////////////////////////////////////////////////////////

void Train::remove(int n){
  Train *tt,*pp;

  for(tt=firstTrain;tt!=NULL && tt->data.id!=n;pp=tt,tt=tt->nextTrain);

  if(tt==NULL){
    CommManager::printf("<X Train::remove>");
    return;
  }

  if(tt==firstTrain)
    firstTrain=tt->nextTrain;
  else
    pp->nextTrain=tt->nextTrain;

  free(tt);

  CommManager::printf("<O>");
}

///////////////////////////////////////////////////////////////////////////////

void Train::show(){
  Train *tt;

  if(firstTrain==NULL){
    CommManager::printf("<X Train::show>");
    return;
  }

  for(tt=firstTrain;tt!=NULL;tt=tt->nextTrain){
    //CommManager::printf("<Q %d %d %d>", tt->data.snum, tt->data.pin, tt->data.pullUp);
  }
}

///////////////////////////////////////////////////////////////////////////////

void Train::status(){
  //Train *tt;

  if(firstTrain==NULL){
    CommManager::printf("<X Train::status>");
    return;
  }

  //for(tt=firstTrain;tt!=NULL;tt=tt->nextTrain){
  //  CommManager::printf("<%c %d>", tt->active?'Q':'q', tt->data.snum);
  //}
}

///////////////////////////////////////////////////////////////////////////////

void Train::parse(const char *c) {
  int n,s,m;

  switch(sscanf(c,"%d %d %d",&n,&s,&m)){

    case 3:                     // argument is string with id number of Train followed by a DCC ID number and flag indicating lights
      create(n,s,m,1);
    break;

    case 1:                     // argument is a string with id number only
      remove(n);
    break;

    case -1:                    // no arguments
      show();
    break;

    case 2:                     // invalid number of arguments
      CommManager::printf("<X Train::parse>");
      break;
  }
}

void Train::setThrottle(const char *c) {
  int nReg;
  int cab;
  int tSpeed;
  int tDirection;

  if(sscanf(c,"%d %d %d %d",&nReg,&cab,&tSpeed,&tDirection)!=4)
    return;

  Train* train = Train::get(cab);
  if (train == NULL) {
      return;
  }

  train->state.speed = tSpeed;
  train->state.direction = tDirection;

}

void Train::setFunction(const char *c) {
  int cab;
  int fByte, eByte;
  int nParams;

  nParams=sscanf(c,"%d %d %d",&cab,&fByte,&eByte);

  if (nParams<2) {
    return;
  }

  Train* train = Train::get(cab);
  if (train == NULL || train->data.f0 == 0) {
      return;
  }

  if ((fByte & 0b10000000) == 0b1000000) { // 128
      if ((fByte & 0b10000) == 0b10000) {  // 16 (F0)
          // Turned lights on
          train->state.lights = 1;
      } else {
          // Turned lights off
          train->state.lights = 0;
      }
  }
}

///////////////////////////////////////////////////////////////////////////////

void Train::load(){
  struct TrainData data;
  Train *tt;

  for(int i=0;i<EEStore::eeStore->data.nTrains;i++){
    EEPROM.get(EEStore::pointer(),data);
    tt=create(data.tnum,data.id,data.f0);
    EEStore::advance(sizeof(tt->data));
  }
}

///////////////////////////////////////////////////////////////////////////////

void Train::store(){
  Train *tt;

  tt=firstTrain;
  EEStore::eeStore->data.nTrains=0;

  while(tt!=NULL){
    EEPROM.put(EEStore::pointer(),tt->data);
    EEStore::advance(sizeof(tt->data));
    tt=tt->nextTrain;
    EEStore::eeStore->data.nTrains++;
  }
}

///////////////////////////////////////////////////////////////////////////////

String Train::getName() {
  String name(data.name);
  return name;
}

Train *Train::firstTrain=NULL;
