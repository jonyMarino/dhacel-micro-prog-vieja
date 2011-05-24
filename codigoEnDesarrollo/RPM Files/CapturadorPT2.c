/*IMPLEMENTATION MODULE CapturadorPT2; */
/*
**     Filename  : CapturadorPT2.C
**     Project   : RPM
**     Processor : MC9S12GC32CFU
**     Version   : 0.0.1
**     Compiler  : Metrowerks HC12 C Compiler
**     Date/Time : 14/04/2008, 13:13
**     Abstract  :
**      Implementacion de un tipo capturador para el timer
**     numero 2 con el modulo CapPT2.  
**      Esta clase es de tipo Singleton (solo puede tener una instancia)
**     obviamente porque utiliza un pin que no puede ser compartido
*/

#include "CapturadorPT2.h"
#include "CapPT2.h"
#include "stddef.h"
#include "Mydefines.h"

#pragma CONST_SEG DEFAULT

#ifdef RPM

void CapturadorPT2_DefConstructor(void*_self,va_list * args);

const struct CapturadorClass CapturadorPT2={
  
  CLASS_INITIALIZATION(CapturadorClass,CapturadorPT2,Capturador,CapturadorPT2_DefConstructor,Object_dtor,Object_differ,Object_puto),
  CapturadorPT2_Procesar,
  CapturadorPT2_Comenzar,
  CapturadorPT2_Terminar,
  CapturadorPT2_getMicroSegundos,
  CapturadorPT2_getPulsos 
};

   
void * CapturadorPT2_Instancia=NULL;


/*
** ===================================================================
**     Method      :  CapturadorPT2_DefConstructor
**     Description :  Constructor de la clase
** ===================================================================
*/
void CapturadorPT2_Constructor(void*_self){  
  struct CapturadorPT2 *_cap= (struct CapturadorPT2 *)_self;
  CapPT2_Init();   
}
/*
** ===================================================================
**     Method      :  CapturadorPT2_DefConstructor
**     Description :  Constructor por defecto
** ===================================================================
*/
void CapturadorPT2_DefConstructor(void*_self,va_list * args){  
  CapturadorPT2_Constructor(_self);  
}
/*
** ===================================================================
**     Method      :  CapturadorPT2_getInstancia
**     Description :  Obtiene la unica instancia de CapturadorPT2
** ===================================================================
*/
void * CapturadorPT2_getInstancia(void){
  
  if (CapturadorPT2_Instancia==NULL)
    CapturadorPT2_Instancia=_new(&CapturadorPT2);
  
  return CapturadorPT2_Instancia;
}

/*
** ===================================================================
**     Method      :  Capturador_Procesar
**     Description :  Procesa el capturador para que obtenga los 
**     microsegundos y los pulsos.
** ===================================================================
*/
void CapturadorPT2_Procesar(void * _self){
  struct CapturadorPT2 *_cap= (struct CapturadorPT2 *)_self;
  CapPT2_GetCapResult(&_cap->ultimoproceso);  
}

/*
** ===================================================================
**     Method      :  Capturador_Comenzar
**     Description :  Comienza la capturacion de pulsos
** ===================================================================
*/
void CapturadorPT2_Comenzar(void * _self){
   CapPT2_Enable();  
}
/*
** ===================================================================
**     Method      :  Capturador_Procesar
**     Description :  Termina la capturacion de pulsos
** ===================================================================
*/
void  CapturadorPT2_Terminar(void * _self){
  CapPT2_Disable();  
}

/*
** ===================================================================
**     Method      :  Capturador_getMicroSegundos 
**     Description :  Obtiene los microsegundos del capturador
** ===================================================================
*/
ulong CapturadorPT2_getMicroSegundos(void * _self){
  struct CapturadorPT2 *_cap= (struct CapturadorPT2 *)_self;
  return Capture_getMicroSegundos(&_cap->ultimoproceso);
}
/*
** ===================================================================
**     Method      :  Capturador_getPulsos 
**     Description :  Obtiene los pulsos del capturador
** ===================================================================
*/
uint CapturadorPT2_getPulsos(void * _self){
  struct CapturadorPT2 *_cap=(struct CapturadorPT2 *) _self;
  return Capture_getPulsos(&_cap->ultimoproceso);
}

#endif

