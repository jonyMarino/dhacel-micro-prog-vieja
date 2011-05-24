#include "BaseTimers_1ms_40ms.hpp"
//#include "timer_interrupt.h"
#include "TimerOld.h"
#include "TI1.h"


BaseTimers_1ms_40ms* BaseTimers_1ms_40ms::instance=NULL;



BaseTimers_1ms_40ms::BaseTimers_1ms_40ms(){
  newAlloced(&on1ms,&Method,inc1,this);
  newAlloced(&on40ms,&Method,inc40,this);
  add1msListener(&on1ms);
  add40msListener(&on40ms);
}

BaseTimers_1ms_40ms *BaseTimers_1ms_40ms::getInstance(void){
  if(!instance)
    instance = new BaseTimers_1ms_40ms();
  return instance;
}

void BaseTimers_1ms_40ms::lockInc(){
  TI1_Disable();
}

void BaseTimers_1ms_40ms::unlockInc(){
  TI1_Enable();
}

void BaseTimers_1ms_40ms::inc1(void * _self){
  BaseTimers_1ms_40ms * self=(BaseTimers_1ms_40ms *)_self;
  self->incrementar(1);
  self->actualizarTimers();
}

void BaseTimers_1ms_40ms::inc40(void * _self){
  BaseTimers_1ms_40ms * self=(BaseTimers_1ms_40ms *)_self;
  self->incrementar(TIEMPO_GRABACION);
  self->actualizarTimers();
}


