/*IMPLEMENTATION MODULE CapturadorSimulado; */
/*
**     Filename  : CapturadorSimulado.C
**     Project   : RPM
**     Processor : MC9S12GC32CFU
**     Version   : 0.0.1
**     Compiler  : Metrowerks HC12 C Compiler
**     Date/Time : 04/04/2008, 10:16
**     Abstract  :
**      Implementacion de un tipo capturador para el timer
**     numero 7 con el modulo Cap1.  
**      Esta clase es de tipo Singleton (solo puede tener una instancia)
**     obviamente porque utiliza un pin que no puede ser compartido
*/

#include "CapturadorSimulado.h"
#include "stddef.h"
#include "Mydefines.h"

#ifdef RPM
void CapturadorSimulado_DefConstructor(void*_self,va_list * args);

const struct CapturadorClass CapturadorSimulado={
  
  CLASS_INITIALIZATION(CapturadorClass,CapturadorSimulado,Capturador,CapturadorSimulado_DefConstructor,Object_dtor,Object_differ,Object_puto),
  CapturadorSimulado_Procesar,
  CapturadorSimulado_Comenzar,
  CapturadorSimulado_Terminar,
  CapturadorSimulado_getMicroSegundos,
  CapturadorSimulado_getPulsos 
};

  
void * CapturadorSimulado_Instancia=NULL;


/*
** ===================================================================
**     Method      :  CapturadorSimulado_DefConstructor
**     Description :  Constructor de la clase
** ===================================================================
*/
void CapturadorSimulado_Constructor(void*_self){  
  struct CapturadorSimulado *_cap= (struct CapturadorSimulado *)_self;   
}
/*
** ===================================================================
**     Method      :  CapturadorSimulado_DefConstructor
**     Description :  Constructor por defecto
** ===================================================================
*/
void CapturadorSimulado_DefConstructor(void*_self,va_list * args){  
  CapturadorSimulado_Constructor(_self);  
}
/*
** ===================================================================
**     Method      :  CapturadorSimulado_getInstancia
**     Description :  Obtiene la unica instancia de CapturadorSimulado
** ===================================================================
*/
void * CapturadorSimulado_getInstancia(void){
  
  if (CapturadorSimulado_Instancia==NULL)
    CapturadorSimulado_Instancia=_new(&CapturadorSimulado);
  
  return CapturadorSimulado_Instancia;
}

/*
** ===================================================================
**     Method      :  Capturador_Procesar
**     Description :  Procesa el capturador para que obtenga los 
**     microsegundos y los pulsos.
** ===================================================================
*/
void CapturadorSimulado_Procesar(void * _self){
  struct CapturadorSimulado *_cap= (struct CapturadorSimulado *)_self;
 
}

/*
** ===================================================================
**     Method      :  Capturador_Comenzar
**     Description :  Comienza la capturacion de pulsos
** ===================================================================
*/
void CapturadorSimulado_Comenzar(void * _self){

}
/*
** ===================================================================
**     Method      :  Capturador_Procesar
**     Description :  Termina la capturacion de pulsos
** ===================================================================
*/
void  CapturadorSimulado_Terminar(void * _self){
   
}

/*
** ===================================================================
**     Method      :  Capturador_getMicroSegundos 
**     Description :  Obtiene los microsegundos del capturador
** ===================================================================
*/
ulong CapturadorSimulado_getMicroSegundos(void * _self){
  struct CapturadorSimulado *_cap=(struct CapturadorSimulado *) _self;
  return _cap->microsegundos;
}
/*
** ===================================================================
**     Method      :  Capturador_getPulsos 
**     Description :  Obtiene los pulsos del capturador
** ===================================================================
*/
uint CapturadorSimulado_getPulsos(void * _self){
  struct CapturadorSimulado *_cap= (struct CapturadorSimulado *)_self;
  return _cap->pulsos;
}

/*
** ===================================================================
**     Method      :  Capturador_getMicroSegundos 
**     Description :  Setea los microsegundos del capturador
** ===================================================================
*/
void CapturadorSimulado_setMicroSegundos(void * _self,ulong val){
  struct CapturadorSimulado *_cap= (struct CapturadorSimulado *)_self;
  _cap->microsegundos=val;
}

/*
** ===================================================================
**     Method      :  Capturador_getPulsos 
**     Description :  Setea los pulsos del capturador
** ===================================================================
*/
void CapturadorSimulado_setPulsos(void * _self,uint val){
  struct CapturadorSimulado *_cap= (struct CapturadorSimulado *)_self;
  _cap->pulsos=val;
}

#endif

