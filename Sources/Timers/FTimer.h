#ifndef F_TIMER_H
#define F_TIMER_H

#include "stdtypes.h"
#include "Timer.h"
#include "Object.h"

typedef void(*pfunction)(void);
/*  Definicion del Timer*/

struct FTimer{
  struct Timer _base;
//Protected:
  pfunction pf;
};

extern const struct TimerClass FTimer;

/*
** ===================================================================
**     Method      :  FTimer_ChangeFunction 
**    Description : Cambia la funcion a llamar cuando se llega al tiempo
** ===================================================================
*/
void FTimer_ChangeFunction(void * _self,pfunction pf);

/*
** ===================================================================
**     Method      :  FTimer_DefConstruct 
**    Description : Metodo por defecto para setear los 
**                  valores de configuración del Timer
** ===================================================================
*/
void FTimer_DefConstruct(void * _self,va_list *args);


/*
** ===================================================================
**     Method      :  _FTimer_getFunction 
**    Description : Retorna la funcion a llamar por el Objeto tipo FTimer
** ===================================================================
*/
#define _FTimer_getFunction(timer) ((struct FTimer*)timer)->pf

#endif 