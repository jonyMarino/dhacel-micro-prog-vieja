/*IMPLEMENTATION MODULE Capturador; */
/*
**     Filename  : Capturador.H
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
#ifndef _CAPTURADOR_H
#define _CAPTURADOR_H

#include "Object.h"
#include "stdtypes.h"

struct Capturador{
  struct Object super;    
};

 

struct CapturadorClass{
  struct Class super;
  void (*Procesar)(void*);          //procesa la entrada para fijar los microsegundos y los pulsos
  void(*Comenzar)(void*);
  void(*Terminar)(void*);
  ulong (*getMicroSegundos)(void*);
  uint (*getPulsos)(void*);
};

extern const struct CapturadorClass Capturador;

void Capturador_Procesar(void*);

void Capturador_Comenzar(void*);

void Capturador_Terminar(void*);

ulong Capturador_getMicroSegundos(void * _self);

uint Capturador_getPulsos(void * _self);

#endif