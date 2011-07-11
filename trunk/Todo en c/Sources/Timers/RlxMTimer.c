/*MODULE RlxMTimer*/


#include <stdarg.h>
#include <stdlib.h>

#include "Cpu.h"
#include "stddef.h"
#include "stdtypes.h"
#include "LinkedList.hpp"
#include "RlxMTimer.h"
#include "Thread.h"
#include "Method.h"

#pragma DATA_SEG RlxMTimer_DATA                                            
#pragma CODE_SEG RlxMTimer_CODE 
#pragma CONST_SEG DEFAULT

void RlxMTimer_DefConstruct(void * _self,va_list * args); 
void RlxMTimer_OnTime(void * _self);
void RlxMTimer_Destruct(void * _self);
void* RlxMTimer_Handler(void * a);

const struct TimerClass RlxMTimer={
   TIMER_CLASS_INITIALIZATION(TimerClass,RlxMTimer,MethodTimer,RlxMTimer_DefConstruct,RlxMTimer_Destruct,NULL,NULL,RlxMTimer_OnTime)
};


LinkedList * _RlxMTCont=NULL;


/*
** ===================================================================
**     Method     : RlxMTimer_Construct
**    Description : Constructor del Objeto
** ===================================================================
*/
void RlxMTimer_Construct(void * _self,ulong tiempo,   void (*pf)(void*),void * Obj ){
void * p_tmp;
struct RlxMTimer * _mt= (struct RlxMTimer *)_self;
void *thread;  
  
  MTimer_Construct(_self,tiempo,pf,Obj);
  _mt->execute=FALSE;
  
  if(!_RlxMTCont){    
    _RlxMTCont=new LinkedList();
    pthread_create(&thread,NULL,RlxMTimer_Handler,NULL);
  }
  
  _RlxMTCont->add(_self);

}

/*
** ===================================================================
**     Method     : RlxMTimer_DefConstruct
**    Description : Constructor por defecto del Objeto
** ===================================================================
*/
void RlxMTimer_DefConstruct(void * _self,va_list * args){
  RlxMTimer_Construct(_self,va_arg(*args,ulong),va_arg(*args,pMethod),va_arg(*args,void*));
}

/*
** ===================================================================
**     Method     : RlxMTimer_DefConstruct
**    Description : Destructor del Objeto
** ===================================================================
*/
void RlxMTimer_Destruct(void * _self){
  _RlxMTCont->moveOut(_self);
  if(_RlxMTCont->isEmpty())
    delete(_RlxMTCont);
  Timer_Destruct(_self);
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
void * RlxMTimer_Handler(void * a){
  if(_RlxMTCont){     
    uint i;
    LinkedList::LinkedListIterator it;
    _RlxMTCont->linkedListIterator(&it);
    
    while(it.hasNext()){
      struct RlxMTimer * _rmt= (struct RlxMTimer *)it.next(); 
      if(_rmt && _rmt->execute){
        _rmt->execute=FALSE;
        (*(_rmt->_base.pf))(_rmt->_base.Obj);  
      }       
    }
  }
  return NULL;
}


