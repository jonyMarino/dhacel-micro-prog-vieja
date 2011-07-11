/*MODULE RlxMTimer*/


#include <stdarg.h>
#include <stdlib.h>

#include "Cpu.h"
#include "stddef.h"
#include "stdtypes.h"
#include "LinkedList.h"
#include "RlxMTimer.h"
#include "Method.h"

#pragma DATA_SEG RlxMTimer_DATA                                            
#pragma CODE_SEG RlxMTimer_CODE 
#pragma CONST_SEG DEFAULT

 /* un RlxMTimer sirve para ejecutar una funcion cumplido el tiempo seleccionado
esta funcion se ejecuta dentro del loop principal , por lo tanto uso la lista cero
 */

 struct RlxMTimer* _RlxMTimer; // contiene el ultimo RlxMTimer creado
 
/*
** ===================================================================
**     Method     : RlxMTimer_Construct
**    Description : Constructor del Objeto
** ===================================================================
*/
struct RlxMTimer* RlxMTimer_Init(ulong tiempo,void (*pf)(void*),void * Obj ){

  struct RlxMTimer* rlxMtimer = (struct RlxMTimer*)malloc(sizeof(struct RlxMTimer));
 
  struct Method* handlerTimer = (struct Method*)malloc(sizeof(struct Method));
  
  if(!rlxMtimer || !handlerTimer) // tengo memoria??
    return;
  
  _RlxMTimer=rlxMtimer;
 
  rlxMtimer->handlerTimerLoop=handlerTimer;
  
  rlxMtimer->execute=FALSE;
  rlxMtimer->_base=MTimer_Init(tiempo,RlxMTimer_OnTime,rlxMtimer);
  
  rlxMtimer->onTime=pf;
  rlxMtimer->Obj=Obj;
  
  handlerTimer->pmethod= RlxMTimer_Handler;
  handlerTimer->Obj= rlxMtimer;
  
  MethodContainer_add(handlerTimer,ListaPrincipal);
  
  return rlxMtimer;

}


/*
** ===================================================================
**     Method     : RlxMTimer_DefConstruct
**    Description : Destructor del Objeto
** ===================================================================
*/
void RlxMTimer_Delete(struct RlxMTimer * _self){
  
  LinkedList_moveOut(_self->handlerTimerLoop,ListaPrincipal);  //saco el metodo del loop principal
  Timer_Delete(&(_self->_base->_base),_self->_base->argBase.NroList); //saco el timer de la interrupt 1ms
}

/*
** ===================================================================
**     Method      :  RlxMTimer_OnTime 
**    Description : funcion a llamar cuando se llega al tiempo
** ===================================================================
*/
void RlxMTimer_OnTime(void * _self){
  struct RlxMTimer *self = (struct RlxMTimer *)_self;
  self->execute=TRUE;
}

/*
** ===================================================================
**    Event       :  RlxMTimer_Handler 
**    Type        :  Static
**    Description :  Manejador a llamar para ejecutar las funciones
** en caso de que se borre algun timer al llamar a su funcion pf,
** el siguiente timer no se ejecutara hasta la siguiente llamada al Handler  
** ===================================================================
*/
void  RlxMTimer_Handler(void * a){
   struct RlxMTimer* tmpRt=(struct RlxMTimer*)a;
   
   if(!tmpRt) //tengo un timer agregado??
    return;
   
   if(tmpRt->execute) // se cumplio el tiempo??
    {
      tmpRt->execute=FALSE;  
      (*(tmpRt->onTime))(tmpRt->Obj); 
    }
}


/**/
 struct RlxMTimer* getUltimoRlxMTimer (){
  return _RlxMTimer;
 }

/**/
void RlxTimer_setTime(struct RlxMTimer*t, ulong _tiempo){
  Timer_setTime(&(t->_base->_base),ListaInt1ms,CuntaPrincipal,_tiempo);
}
 
 
/**/ 
ulong RlxTimer_getTime(struct RlxMTimer* t){
  return Timer_getTime(&(t->_base->_base));
}
 
/**/
void RlxTimer_Stop(struct RlxMTimer*t){
  Timer_Stop(&(t->_base->_base));
}

/**/
uchar RlxTimer_isfinish(struct RlxMTimer*t){
  return Timer_isfinish(&(t->_base->_base),ListaInt1ms);
}

/**/
void RlxTimer_Restart(struct RlxMTimer*t){
   Timer_Restart(&(t->_base->_base),ListaInt1ms,CuntaPrincipal);    
}