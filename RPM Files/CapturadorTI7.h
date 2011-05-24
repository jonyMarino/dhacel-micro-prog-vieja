#ifndef _CAPTURADOR_TI7_H
#define _CAPTURADOR_TI7_H

#include "Capturador.h"
#include "Capture.h"

struct CapturadorTI7{
//private:
  struct Capturador _base; 
  TCapture ultimoproceso; 
};

void * CapturadorTI7_getInstancia(void);

void CapturadorTI7_Procesar(void * _self);

void  CapturadorTI7_Comenzar(void * _self);

void  CapturadorTI7_Terminar(void * _self);

ulong CapturadorTI7_getMicroSegundos(void * _self);

uint CapturadorTI7_getPulsos(void * _self);

#endif