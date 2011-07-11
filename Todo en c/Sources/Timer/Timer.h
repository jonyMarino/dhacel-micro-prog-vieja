#ifndef _TIMER_H
#define _TIMER_H

#include "stdtypes.h"
#include "Object.h"
#include "BaseTimers.h"



#pragma DATA_SEG Timer_DATA                                            
#pragma CODE_SEG Timer_CODE 
#pragma CONST_SEG DEFAULT

typedef enum {
   _timerStop,
   _timerStart
}stateTimer;


/*  Definicion del Timer*/ 

struct Timer{
  void (*onTime)(void*); 
  void * Obj;
  ulong next_cuenta;
  ulong tiempo;
  uchar of;
  stateTimer state;
  
};

void Timer_Init(void* timer,unsigned int NroList,unsigned char NroTime);

void Timer_Delete(struct Timer* t,unsigned int NroList);

void setBaseTimer();

void TimerComp(void* timer,unsigned char NroTime);

uchar Timer_isfinish(struct Timer* t,unsigned int NroList);

ulong Timer_getCuenta(struct Timer* t,unsigned int NroList,unsigned char NroTime);

void Timer_Restart(struct Timer* t,unsigned int NroList,unsigned char NroTime);

void Timer_setTime(struct Timer* t,unsigned int NroList,unsigned char NroTime,ulong _tiempo);

ulong Timer_getTime(struct Timer* t);

void Timer_Stop(struct Timer* t);



#pragma DATA_SEG DEFAULT                                            
#pragma CODE_SEG DEFAULT 


#endif