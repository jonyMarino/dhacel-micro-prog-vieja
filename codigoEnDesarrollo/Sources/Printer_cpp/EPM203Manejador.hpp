#ifndef _EPM_203_MANEJADOR_HPP
#define _EPM_203_MANEJADOR_HPP

#include "OutputStream.hpp"
#include "EPM203Conf.h"
#include "Timer.h"


class EPM203Manejador: public OutputStream{
  public:
    EPM203Manejador(){}
    EPM203Manejador( struct EPM203Conf * configuracion);
    //Fuente
    byte getFuente();
    byte setFuenteAndConfig(byte val);

    //direccion
    byte getDireccion();
    byte setDireccionAndConfig(byte val);
    
    byte setFuente(byte val);
    byte setDireccion(byte val);

  private:
    
    struct EPM203Conf * conf; 
    struct Timer timer;
    
   
};



#endif