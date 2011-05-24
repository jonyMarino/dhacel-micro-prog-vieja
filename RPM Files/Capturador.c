/*IMPLEMENTATION MODULE Capturador; */
/*
**     Filename  : Capturador.C
**     Project   : RPM
**     Processor : MC9S12GC32CFU
**     Version   : 0.0.1
**     Compiler  : Metrowerks HC12 C Compiler
**     Date/Time : 04/04/2008, 10:02
**     Abstract  :
**      Implementacion de la clase abstracta.
**							Capturador de pulsos de la entrada
**     tanto de la cantidad de pulsos como del tiempo
**     desde el primer hasta el ultimo pulso
*/

#include "Capturador.h"
#include "Mydefines.h"
//const struct CapturadorClass Capturador;

#ifdef RPM
/*
** ===================================================================
**     Method      :  Capturador_Procesar
**     Description :  Procesa el capturador para que obtenga los 
**     microsegundos y los pulsos.
** ===================================================================
*/
void Capturador_Procesar(void* _self){
  struct CapturadorClass ** vTable= (struct CapturadorClass ** )_self;
  
  (**vTable).Procesar(_self);    
}

/*
** ===================================================================
**     Method      :  Capturador_Comenzar
**     Description :  Comienza la capturacion de pulsos
** ===================================================================
*/
void Capturador_Comenzar(void* _self){
  struct CapturadorClass ** vTable=(struct CapturadorClass ** ) _self;
  
  (**vTable).Comenzar(_self);    
}

/*
** ===================================================================
**     Method      :  Capturador_Procesar
**     Description :  Termina la capturacion de pulsos
** ===================================================================
*/
void Capturador_Terminar(void* _self){
  struct CapturadorClass ** vTable=(struct CapturadorClass ** ) _self;
  
  (**vTable).Terminar(_self);    
}

/*
** ===================================================================
**     Method      :  Capturador_getMicroSegundos 
**     Description :  Obtiene los microsegundos del capturador
** ===================================================================
*/
ulong Capturador_getMicroSegundos(void * _self){
  struct CapturadorClass ** vTable= (struct CapturadorClass ** )_self;
  
  return (**vTable).getMicroSegundos(_self);
  
}

/*
** ===================================================================
**     Method      :  Capturador_getPulsos 
**     Description :  Obtiene los pulsos del capturador
** ===================================================================
*/
uint Capturador_getPulsos(void * _self){
  struct CapturadorClass ** vTable= (struct CapturadorClass ** )_self;
  
  return (**vTable).getPulsos(_self);
}

#endif
