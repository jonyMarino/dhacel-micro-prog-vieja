#ifndef _ADJUNTADOR_A_HILO_H
#define _ADJUNTADOR_A_HILO_H

#include "ThreadAttachable.hpp"

struct AdjuntadorAHilo{
  void * _base;
  ThreadAttachable * threadAdjuntable;
};

extern const struct Class AdjuntadorAHilo;

/*
** ===================================================================
**     Method      :  AdjuntadorAHilo_adjuntar 
**     Description :  adjunta el metodo al hilo de ejecucion
** ===================================================================
*/
void AdjuntadorAHilo_adjuntar(void * _self,pMethod method, void * Obj);

#endif