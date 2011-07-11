#ifndef _MANEJADOR_IMPRESION_PERSISTENTE_H
#define _MANEJADOR_IMPRESION_PERSISTENTE_H


#include "ManejadorImpresion.h"
#include "MIPConf.h"
#include "MethodTimer.h"

void ManejadorImpresionPersistente_printHora();
      
void ManejadorImpresionPersistenteInit(struct MIPConf * configuracion);
      
void ManejadorImpresionPersistente_imprimir();
      
int ManejadorImpresionPersistente_getIntervalo();
      
void ManejadorImpresionPersistente_setIntervalo(int intervalo);
      
bool ManejadorImpresionPersistente_getHabilitado();
      
void ManejadorImpresionPersistente_setHabilitado(bool habilitar);
    
  
     
#endif
  
