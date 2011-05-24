#include <stdarg.h>
#include <stdlib.h>
#include <string.h> 


#include "stddef.h"
#include "stdtypes.h"
#include "Cpu.h"
#include "Timer.h"
#include "BaseTimers_1ms_40ms.hpp"

#pragma DATA_SEG Timer_DATA                                            
#pragma CODE_SEG Timer_CODE 
#pragma CONST_SEG DEFAULT

void Timer_DefConstruct(void * _self,va_list *);
void Timer_OnTime(void * _self);
//void EnterTimer(struct Timer * timer);

const struct Class TimerClass={
  CLASS_INITIALIZATION(Class,TimerClass,Class,Object_ctor,Object_dtor,Object_differ,Object_puto)  
};

const struct TimerClass Timer={
  TIMER_CLASS_INITIALIZATION(TimerClass,Timer,Object,Timer_DefConstruct,Timer_Destruct,NULL,NULL,Timer_OnTime) 
};

/*
** ===================================================================
**     Method      :  Timer_Construct 
**    Description : Metodo para setear los 
**                  valores de configuración del contador
** ===================================================================
*/
void Timer_Construct(void * _self,ulong tiempo){
  struct Timer * self = (struct Timer*) _self;
  BaseTimers * baseTimer = BaseTimers_1ms_40ms::getInstance(); 
  ulong cuenta = baseTimer->getCuenta();
  self->baseTimer = baseTimer;
  self->tiempo= tiempo;
  self->next_cuenta=cuenta+tiempo;
  
  if(cuenta >= self->next_cuenta)
    self->of=TRUE;  
  else
    self->of=FALSE;
    
  baseTimer->add(self);

}

/*
** ===================================================================
**     Method      :  Timer_DefConstruct 
**    Description : Constructor por defecto
** ===================================================================
*/
void Timer_DefConstruct(void * self,va_list *args){
  Timer_Construct(self,va_arg(*args,ulong));
}

/*
** ===================================================================
**     Method      :  Timer_Destruct 
**    Description : Metodo para destruir el Timer
** ===================================================================
*/
void Timer_Destruct(void * self){
  Timer_Stop(self);
}

/*
** ===================================================================
**     Method      :  setBaseTimer 
**    Description : setea la entrada de ticks del timer
** ===================================================================
*/
void setBaseTimer(void * _self,BaseTimers * base){
  struct Timer * self = (struct Timer*)_self;  
  self->baseTimer = base;
}
/*
** ===================================================================
**     Method      :  Timer_OnTime 
**    Description : Evento al llegar al tiempo fijado
** ===================================================================
*/
void Timer_OnTime(void * self){
  Timer_Stop(self);
}

/*
** ===================================================================
**     Method      :  IncTimer 
**    Description : Incrementa el valor de la cuenta del Timer
** ===================================================================
*/
void TimerComp(void * _self){  
  struct Timer * self = (struct Timer*)_self;
  bool comp;
  BaseTimers * baseTimer = self->baseTimer;  
  ulong cuentaTmp = baseTimer->getCuenta();
  ulong nextCuentaTmp = self->next_cuenta;
  ulong tiempoTmp = self->tiempo;
  
  if(self->of)
    comp= cuentaTmp <= nextCuentaTmp;
  else
    comp= cuentaTmp >= nextCuentaTmp;
  
  if (comp ){
    int error = cuentaTmp - nextCuentaTmp;
    error = (error<0)?-error:error;
    nextCuentaTmp +=   error + tiempoTmp;
    self->next_cuenta=nextCuentaTmp;    
    if(cuentaTmp >= nextCuentaTmp){
      self->of=TRUE;        
    }
    (**(struct TimerClass**)self).onTime(self);
  }
}

/*
** ===================================================================
**     Method      :  Timer_getCuenta 
**    Description : Regresa la cuenta actual que se resetea al llegar al tiempo
** ===================================================================
*/
ulong Timer_getCuenta(void * _self){
  struct Timer * self = (struct Timer*)_self;
  ulong cuentaTmp;
  ulong nextCuentaTmp;
  ulong tiempoTmp;
  
  if(Timer_isfinish(self) )
    return 0;
  cuentaTmp = self->baseTimer->getCuenta();

  Cpu_DisableInt();
    nextCuentaTmp = self->next_cuenta;
    tiempoTmp = self->tiempo;
  Cpu_EnableInt();
   
  return cuentaTmp - (nextCuentaTmp - tiempoTmp);
}

/*
** ===================================================================
**     Method      :  Timer_isfinish 
**    Description : Indica si ya termino la cuenta
** ===================================================================
*/
uchar Timer_isfinish(void * _self){
  struct Timer * self = (struct Timer*)_self;
  if(self->baseTimer)
    return !self->baseTimer->contains(self);
  return TRUE;
}

/*
** ===================================================================
**     Method      :  Timer_Restart 
**    Description : Reinicia la cuenta del tiempo
** ===================================================================
*/
void Timer_Restart(void * _self){
  struct Timer * self = (struct Timer*)_self;
  ulong cuenta = self->baseTimer->getCuenta();
  self->next_cuenta= cuenta + self->tiempo ;
  if(cuenta >= self->next_cuenta)
    self->of=TRUE;  
  else
    self->of=FALSE;
  if(Timer_isfinish(self))
    self->baseTimer->add(self);
}

/*
** ===================================================================
**     Method      :  Timer_setTime 
**    Description : Setea un tiempo nuevo y reinicia la cuenta del tiempo
** ===================================================================
*/
void Timer_setTime(void * _self,ulong tiempo){					 
  struct Timer * self = (struct Timer*)_self;
  self->tiempo=tiempo;
  Timer_Restart(self);
}

/*
** ===================================================================
**     Method      :  Timer_getTime 
**    Description : Obtiene el tiempo a alcanzar
** ===================================================================
*/
ulong Timer_getTime(void * _self){
  struct Timer * self = (struct Timer*)_self;
  return self->tiempo;
}

/*
** ===================================================================
**     Method      :  Timer_Stop 
**    Description : Detiene la cuenta del timer
** ===================================================================
*/
void Timer_Stop(void * _self){  
  struct Timer * self = (struct Timer*)_self;
  self->baseTimer->moveOut(self);
}



#pragma CODE_SEG Timer_CODE


