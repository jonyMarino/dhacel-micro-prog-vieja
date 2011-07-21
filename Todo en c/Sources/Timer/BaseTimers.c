#include "BaseTimers.h"
#include "MethodTimer.h"
#include "Timer.h"

#define MAX_TIMER 10

unsigned long cuenta[MAX_TIMER]={0};
unsigned char nroTimerActual;


void BaseTimers_incrementar(unsigned int ms,unsigned char NroTime){
  cuenta[NroTime]+=ms;
}

bool BaseTimers_moveOut(void * timer,unsigned int NroList){
  byte err;
  BaseTimers_1ms_40ms_lockInc();
  err = LinkedList_moveOut(timer,NroList);
  BaseTimers_1ms_40ms_unlockInc();
  
  
  return err;
}


void BaseTimers_actualizarTimers(struct Timer* t){
    
    TimerComp(t,t->argTimer.NroTime);
         
  
}

unsigned long BaseTimers_getCuenta(unsigned char NroTime){
  unsigned long cuentatmp;
  
  cuentatmp = cuenta[NroTime];
    
  return cuentatmp;
}



unsigned char getUltimoNroTimer(){
   return  nroTimerActual;
}


void setNroTimer(unsigned char val){
    nroTimerActual=val;
}
