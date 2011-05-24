#ifndef _EPM_203_MANEJADOR_H
#define _EPM_203_MANEJADOR_H

#include "OutputStream.h"
#include "EPM203Conf.h"
#include "Timer.h"


struct EPM203Manejador{
  struct OutputStream super;
  struct EPM203Conf * conf; 
  struct Timer timer;
};


extern const struct OutputStreamClass EPM203Manejador;

//Fuente
byte EPM203Manejador_getFuente(void * _self);
byte EPM203Manejador_setFuenteAndConfig(void * _self,byte val);

//direccion
byte EPM203Manejador_getDireccion(void * _self);
byte EPM203Manejador_setDireccionAndConfig(void * _self,byte val);


#endif