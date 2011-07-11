#ifndef _MANEJADORIMPRESION_H
#define _MANEJADORIMPRESION_H

#include "MethodContainer.h"
#include "RlxMTimer.h"
#include "OutputStream.h"


typedef enum {
  _Sensor,
  _Valor
}TipoAimprimir;

void ManejadorImpresionInit();

int getIntervalo();

void setIntervalo(int intervalo);

bool getHabilitado();

void setHabilitado(bool habilitar);  

int getCuentaParcial();

void sincronismo(); 
  
void imprimir();

void imprimirPeriodico();

void addMethodAImprimir(void * Obj,TipoAimprimir tAi);
  

#endif
