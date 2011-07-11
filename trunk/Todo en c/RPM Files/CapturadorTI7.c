/*IMPLEMENTATION MODULE CapturadorTI7; */
/*
**     Filename  : CapturadorTI7.C
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

#include "CapturadorTI7.h"
#include "Cap1.h"
#include "stddef.h"
#include "Mydefines.h"

#ifdef RPM
void CapturadorTI7_DefConstructor(void*_self,va_list * args);

const struct CapturadorClass CapturadorTI7={
  CLASS_INITIALIZATION(CapturadorClass,CapturadorTI7,Capturador,CapturadorTI7_DefConstructor,Object_dtor,Object_differ,Object_puto),
  CapturadorTI7_Procesar,
  CapturadorTI7_Comenzar,
  CapturadorTI7_Terminar,
  CapturadorTI7_getMicroSegundos,
  CapturadorTI7_getPulsos 
};
   
void * CapturadorTI7_Instancia=NULL;


/*
** ===================================================================
**     Method      :  CapturadorTI7_DefConstructor
**     Description :  Constructor de la clase
** ===================================================================
*/
void CapturadorTI7_Constructor(void*_self){  
  struct CapturadorTI7 *_cap= (struct CapturadorTI7 *)_self;
  Cap1_Init();   
}
/*
** ===================================================================
**     Method      :  CapturadorTI7_DefConstructor
**     Description :  Constructor por defecto
** ===================================================================
*/
void CapturadorTI7_DefConstructor(void*_self,va_list * args){  
  CapturadorTI7_Constructor(_self);  
}
/*
** ===================================================================
**     Method      :  CapturadorTI7_getInstancia
**     Description :  Obtiene la unica instancia de CapturadorTI7
** ===================================================================
*/
void * CapturadorTI7_getInstancia(void){
  
  if (CapturadorTI7_Instancia==NULL)
    CapturadorTI7_Instancia=_new(&CapturadorTI7);
  
  return CapturadorTI7_Instancia;
}

/*
** ===================================================================
**     Method      :  Capturador_Procesar
**     Description :  Procesa el capturador para que obtenga los 
**     microsegundos y los pulsos.
** ===================================================================
*/
void CapturadorTI7_Procesar(void * _self){
  struct CapturadorTI7 *_cap=(struct CapturadorTI7 *) _self;
  Cap1_GetCapResult(&_cap->ultimoproceso);  
}

/*
** ===================================================================
**     Method      :  Capturador_Comenzar
**     Description :  Comienza la capturacion de pulsos
** ===================================================================
*/
void CapturadorTI7_Comenzar(void * _self){
   Cap1_Enable();  
}
/*
** ===================================================================
**     Method      :  Capturador_Procesar
**     Description :  Termina la capturacion de pulsos
** ===================================================================
*/
void  CapturadorTI7_Terminar(void * _self){
  Cap1_Disable();  
}

/*
** ===================================================================
**     Method      :  Capturador_getMicroSegundos 
**     Description :  Obtiene los microsegundos del capturador
** ===================================================================
*/
ulong CapturadorTI7_getMicroSegundos(void * _self){
  struct CapturadorTI7 *_cap= (struct CapturadorTI7 *)_self;
  return Capture_getMicroSegundos(&_cap->ultimoproceso);
}
/*
** ===================================================================
**     Method      :  Capturador_getPulsos 
**     Description :  Obtiene los pulsos del capturador
** ===================================================================
*/
uint CapturadorTI7_getPulsos(void * _self){
  struct CapturadorTI7 *_cap= (struct CapturadorTI7 *)_self;
  return Capture_getPulsos(&_cap->ultimoproceso);
}

#endif

