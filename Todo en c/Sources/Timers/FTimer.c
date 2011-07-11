/*MODULE FTimer*/


#include <stdarg.h>
#include <stdlib.h>

#include "stddef.h"
#include "stdtypes.h"
#include "Timer.h"
#include "FTimer.h"

 
#pragma CONST_SEG DEFAULT

void FTimer_OnTime(struct FTimer * self);
void FTimer_DefConstruct(struct FTimer * self,va_list *);


const struct TimerClass FTimer={
  TIMER_CLASS_INITIALIZATION(TimerClass,FTimer,Timer,FTimer_DefConstruct,Timer_Destruct,NULL,NULL,FTimer_OnTime)
};


/*
** ===================================================================
**     Method      :  FTimer_Construct 
**    Description : Metodo para setear los 
**                  valores de configuración del Timer
** ===================================================================
*/
void FTimer_Construct(struct FTimer * self,ulong tiempo, pfunction pf){
  Timer_Construct((struct Timer *) self,tiempo);
  self->pf=pf;    
}

/*
** ===================================================================
**     Method      :  FTimer_DefConstruct 
**    Description : Metodo por defecto para setear los 
**                  valores de configuración del Timer
** ===================================================================
*/
void FTimer_DefConstruct(struct FTimer * self,va_list *args){
  FTimer_Construct(self,va_arg(*args,ulong),va_arg(*args,pfunction));
}

/*
** ===================================================================
**     Method      :  FTimer_OnTime 
**    Description : funcion a llamar cuando se llega al tiempo
** ===================================================================
*/
void FTimer_OnTime(struct FTimer * self){
  (*(self->pf))();
}
/*
** ===================================================================
**     Method      :  FTimer_ChangeFunction 
**    Description : Cambia la funcion a llamar cuando se llega al tiempo
** ===================================================================
*/
void FTimer_ChangeFunction(void * _self,pfunction pf){
  struct FTimer * self = (struct FTimer *)_self;
  self->pf=pf;  
}

