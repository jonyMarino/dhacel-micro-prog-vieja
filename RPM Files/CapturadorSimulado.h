#ifndef _CAPTURADOR_SIMULADO_H
#define _CAPTURADOR_SIMULADO_H

#include "Capturador.h"
#include "Capture.h"

struct CapturadorSimulado{
//private:
  struct Capturador _base; 
  ulong microsegundos; 
  uint pulsos;
};

void * CapturadorSimulado_getInstancia(void);

void CapturadorSimulado_Procesar(void * _self);

void  CapturadorSimulado_Comenzar(void * _self);

void  CapturadorSimulado_Terminar(void * _self);

ulong CapturadorSimulado_getMicroSegundos(void * _self);

void CapturadorSimulado_setMicroSegundos(void * _self,ulong val);

uint CapturadorSimulado_getPulsos(void * _self);

void CapturadorSimulado_setPulsos(void * _self,uint val);


#endif