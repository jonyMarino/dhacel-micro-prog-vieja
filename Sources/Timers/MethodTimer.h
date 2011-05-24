#ifndef _METHOD_TIMER_H
#define _METHOD_TIMER_H

#include "stdtypes.h"
#include "Timer.h"
#include "Object.h"

#pragma DATA_SEG MethodTimer_DATA                                            
#pragma CODE_SEG MethodTimer_CODE 
#pragma CONST_SEG DEFAULT
/*  Definicion del Timer*/

struct MethodTimer{
  struct Timer _base;
  void (*pf)(void*);
  void * Obj;
};

extern const struct TimerClass MethodTimer;
/*
** ===================================================================
**     Method      :  MTimer_Construct 
**    Description : Metodo para setear los 
**                  valores de configuración del Timer
** ===================================================================
*/
void MTimer_Construct(void * self,ulong tiempo,void (*pf)(void*),void * Obj);
/*
** ===================================================================
**     Method      :  MTimer_DefConstruct 
**    Description : Metodo por defecto para setear los 
**                  valores de configuración del Timer
** ===================================================================
*/
void MTimer_DefConstruct(void * self,va_list *args);

/*
** ===================================================================
**     Method      :  MethodTimer_setMetodo 
**     Description : 
** ===================================================================
*/
void MethodTimer_setMetodo(void * self,void (*pf)(void*),void * Obj);



#define _MethodTimer_getFunction(_timer) ((struct MethodTimer*)_timer)->pf

#define _MethodTimer_getObj(_timer) ((struct MethodTimer*)_timer)->Obj


#pragma DATA_SEG DEFAULT                                            
#pragma CODE_SEG DEFAULT 

#endif