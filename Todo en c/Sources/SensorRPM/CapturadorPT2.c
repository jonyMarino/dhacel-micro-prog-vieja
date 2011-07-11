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


/*
** ===================================================================
**     Method      :  CapturadorPT2_DefConstructor
**     Description :  Constructor de la clase
** ===================================================================
*/
void CapturadorPT2_Init(){  
  CapPT2_Init();   
}

/*
** ===================================================================
**     Method      :  CapturadorPT2_getInstancia
**     Description :  Obtiene la unica instancia de CapturadorPT2
** ===================================================================
*/
struct CapturadorPT2 * CapturadorPT2_getInstancia(){
    struct CapturadorPT2* CapturadorPT2_Instancia;
 
    CapturadorPT2_Instancia=(struct CapturadorPT2*)malloc(sizeof(struct CapturadorPT2));
  
  if(!CapturadorPT2_Instancia)
    return NULL;
  
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
void CapturadorPT2_Comenzar(){
   CapPT2_Enable();  
}
/*
** ===================================================================
**     Method      :  Capturador_Procesar
**     Description :  Termina la capturacion de pulsos
** ===================================================================
*/
void  CapturadorPT2_Terminar(){
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
  return Capture_getMicroSegundos(_cap->ultimoproceso);
}
/*
** ===================================================================
**     Method      :  Capturador_getPulsos 
**     Description :  Obtiene los pulsos del capturador
** ===================================================================
*/
uint CapturadorPT2_getPulsos(void * _self){
  struct CapturadorPT2 *_cap=(struct CapturadorPT2 *) _self;
  return Capture_getPulsos(_cap->ultimoproceso);
}

#endif
