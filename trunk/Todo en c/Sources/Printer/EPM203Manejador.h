#ifndef _EPM_203_MANEJADOR_HPP
#define _EPM_203_MANEJADOR_HPP

#include "OutputStream.h"
#include "Timer.h"
#include "EPM203_Conf.h"

void EPM203Manejador_init( struct EPM203Conf * configuracion);

byte EPM203Manejador_getFuente();

byte EPM203Manejador_setFuenteAndConfig(byte val);

byte EPM203Manejador_getDireccion();

byte EPM203Manejador_setDireccionAndConfig(byte val);
    
byte EPM203Manejador_setFuente(byte val);

byte EPM203Manejador_setDireccion(byte val);

 
    
   
    
   



#endif