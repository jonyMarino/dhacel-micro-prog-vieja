#ifndef _CAPTURADOR_PT2_H
#define _CAPTURADOR_PT2_H

#include "stdtypes.h"
#include "Capture.h"

struct CapturadorPT2{
  TCapture ultimoproceso; 
};

void CapturadorPT2_Init();

struct CapturadorPT2 * CapturadorPT2_getInstancia();

void CapturadorPT2_Procesar(void * _self);

void  CapturadorPT2_Comenzar();

void  CapturadorPT2_Terminar();

ulong CapturadorPT2_getMicroSegundos(void * _self);

uint CapturadorPT2_getPulsos(void * _self);

#endif