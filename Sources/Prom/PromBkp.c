#include <limits.h>
#include <stdlib.h>
#include "stddef.h"
//#include "IFsh10.h"
#include "PromBkp.h"
#include "PromBkpProtected.h"
#include "Timer.h"

void PromBkp_ctor(void * _self,va_list * args);

const struct Class PromBkpClass={
  CLASS_INITIALIZATION(Class,PromBkpClass,Class,Object_ctor,Object_dtor,Object_differ,Object_puto)  
};
const struct PromBkpClass PromBkp={
  CLASS_INITIALIZATION(PromBkpClass,PromBkp,Object,PromBkp_ctor,Object_dtor,Object_differ,Object_puto)  
};


void PromBkp_ctor(void * _self,va_list * args){
  struct PromBkp * self = (struct PromBkp *)_self;
  self->aGrabar = FALSE;
  self->timer = NULL;
  self->disableCont=0;
  
}

/*
** ===================================================================
**    Function    :  PromBkp_deshabilitar 
**    Description : 
** ===================================================================
*/
void PromBkp_deshabilitar(void* self){
  struct PromBkp * _p = (struct PromBkp *)self; 
  _p->disableCont++;
  if(!_p->disableCont) //pego la vuelta?
    _p->disableCont=UINT_MAX;   
}

/*
** ===================================================================
**    Function    :  PromBkp_habilitar 
**    Description : 
** ===================================================================
*/
void PromBkp_habilitar(void * self){
  struct PromBkp * _p = (struct PromBkp *)self;
  _p->disableCont--;
  if(_p->disableCont==UINT_MAX) //pego la vuelta?
    _p->disableCont=0;   
}


/*
** ===================================================================
**    Function    :  PromBkp_demorarGrabado 
**    Description : 
** ===================================================================
*/
void PromBkp_demorarGrabado(void * self,word time){
  struct PromBkp * _p = (struct PromBkp *)self;
  if(!_p->timer)
    _p->timer= (struct Timer *) _new(&Timer,(ulong)time);
  else
    Timer_setTime(_p->timer,(ulong)time);
}
/*
** ===================================================================
**    Function    :  PromBkp_listoParaGrabar 
**    Description : 
** ===================================================================
*/
bool PromBkp_listoParaGrabarOBorrar(void * self){
  struct PromBkp * _p = (struct PromBkp *)self;
  bool delayFlag;
  
  delayFlag=(_p->timer)?Timer_isfinish(_p->timer):TRUE;
  if(_p->timer && delayFlag)
    deleteAndNil(&_p->timer);
  
  return (PromBkp_grabacionOBorradoEnEspera(self)
          && delayFlag
          && _p->disableCont==0);
}
/*
** ===================================================================
**    Function    :  PromBkp_grabacionOBorradoEnEspera 
**    Description : 
** ===================================================================
*/
bool PromBkp_grabacionOBorradoEnEspera(void * self){
  struct PromBkp * _p = (struct PromBkp *)self;
  
  return ( _p->aGrabar || ! _p->listaDireccionesABorrar.isEmpty()); 
}
/*
** ===================================================================
**    Function    :  PromBkp_borrarPagina 
**    Description : 
** ===================================================================
*/
byte PromBkp_borrarPagina(void * self,void * Addr){
  struct PromBkp * _p = (struct PromBkp *)self;
  if(Addr==NULL)
    return 1;
  return _p->listaDireccionesABorrar.add(Addr);
}
 
/*
** ===================================================================
**    Function    :  PromBkp_grabarOBorrar 
**    Description :  template method
** ===================================================================
*/
byte PromBkp_grabarOBorrar(void * self){
  struct PromBkp * _p = (struct PromBkp *)self;
  void* direccion;
  direccion=_p->listaDireccionesABorrar.eliminarPrimero();
  if(direccion!=NULL){     //borrar?
    //PromBkp_callPreEraseListeners(self,direccion);
    return _PromBkp_borrarProm(self,direccion); 
    //PromBkp_callPreEraseListeners(self,direccion);

  } else if(_p->aGrabar){
    direccion=_PromBkp_grabarProm(self);   
    //PromBkp_callPostWriteListeners(self,direccion);
    return direccion!=NULL;
  } 
  return EXIT_FAILURE;
  
}

/*
** ===================================================================
**    Function    :  PromBkp_grabarOBorrar 
**    Description :  template method
** ===================================================================
*/
void PromBkp_setAGrabar(void * self,bool valor){
  struct PromBkp * _p = (struct PromBkp *)self;
  _p->aGrabar=valor;    
}


/*
** ===================================================================
**    Function    :  PromBkp_getAGrabar 
**    Description :  template method
** ===================================================================
*/
bool PromBkp_getAGrabar(void * self){
  struct PromBkp * _p = (struct PromBkp *)self;
  return _p->aGrabar;    
}
