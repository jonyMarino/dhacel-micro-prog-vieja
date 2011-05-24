#ifndef _TIMER_H
#define _TIMER_H

#include "stdtypes.h"
#include "Object.h"
//#include "TimerAllocable.h"
#include "BaseTimers.hpp"


#pragma DATA_SEG Timer_DATA                                            
#pragma CODE_SEG Timer_CODE 
#pragma CONST_SEG DEFAULT
/*  Definicion del Timer*/

struct TimerClass{
  struct Class super;
  void (*onTime)(void*); 
};
 

struct Timer{
  struct Object super;
//Private:
  ulong next_cuenta;
  ulong tiempo;
  uchar of;
  BaseTimers * baseTimer;
};

extern const struct Class TimerClass;
extern const struct TimerClass Timer;

void Timer_Construct(void * _self,ulong tiempo);
/*
** ===================================================================
**     Method      :  Timer_Destruct 
**    Description : Metodo para destruir el Timer
** ===================================================================
*/
void Timer_Destruct(void * _self);
/*
** ===================================================================
**     Method      :  setBaseTimer 
**    Description : setea la entrada de ticks del timer
** ===================================================================
*/
void setBaseTimer(void * _self,BaseTimers * base);
/*
** ===================================================================
**     Method      :  TimerComp 
**    Description : Compara el valor del timer contra el ultimo valor de
**    la cuenta
** ===================================================================
*/
void TimerComp(void * _self);
/*
** ===================================================================
**     Method      :  Timer_isfinish 
**    Description : Indica si ya termino la cuenta
** ===================================================================
*/
uchar Timer_isfinish(void * _self);
/*
** ===================================================================
**     Method      :  Timer_getCuenta 
**    Description : Regresa la cuenta actual que se resetea al llegar al tiempo
** ===================================================================
*/
ulong Timer_getCuenta(void * _self);
/*
** ===================================================================
**     Method      :  Timer_Restart 
**    Description : Reinicia la cuenta del tiempo
** ===================================================================
*/
void Timer_Restart(void * _self);
/*
** ===================================================================
**     Method      :  Timer_setTime 
**    Description : Setea un tiempo nuevo y reinicia la cuenta del tiempo
** ===================================================================
*/
void Timer_setTime(void * _self,ulong tiempo);
/*
** ===================================================================
**     Method      :  Timer_getTime 
**    Description : Obtiene el tiempo a alcanzar
** ===================================================================
*/
ulong Timer_getTime(void * _self);
/*
** ===================================================================
**     Method      :  Timer_Stop 
**    Description : Detiene la cuenta del timer
** ===================================================================
*/
void Timer_Stop(void * _self);



#define TIMER_CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto,onTime)\
  CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto),  \
  onTime
 
#pragma DATA_SEG DEFAULT                                            
#pragma CODE_SEG DEFAULT 


#endif