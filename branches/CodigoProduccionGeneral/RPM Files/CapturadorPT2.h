#ifndef _CAPTURADOR_PT2_H
#define _CAPTURADOR_PT2_H

#include "Capturador.h"
#include "Capture.h"

struct CapturadorPT2{
//private:
  struct Capturador _base; 
  TCapture ultimoproceso; 
};

void * CapturadorPT2_getInstancia(void);

void CapturadorPT2_Procesar(void * _self);

void  CapturadorPT2_Comenzar(void * _self);

void  CapturadorPT2_Terminar(void * _self);

ulong CapturadorPT2_getMicroSegundos(void * _self);

uint CapturadorPT2_getPulsos(void * _self);

#endif