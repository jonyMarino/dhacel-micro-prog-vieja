#include "EPM203Manejador.hpp"


#include <assert.h>
#include <termio.h>

EPM203Manejador::EPM203Manejador( struct EPM203Conf * configuracion):conf(configuracion){
  /*9600 Baud sin RTS/DTR*/
  write(0x1d);
  write(0x42);
  write(0x3);
  setFuente(getFuente());
  setDireccion(getDireccion());
}



//Fuente
byte EPM203Manejador::getFuente(){
  return _EPM203Conf_getFuente(conf);  
}

byte EPM203Manejador::setFuente(byte val){
  if(val>3)
    return ERR_RANGE;
  write(0x1b);
  write(0x25);  
  write(val);
  /*Save setup parameters*/
  write(0x1B);
  write(0x73);
}

byte EPM203Manejador::setFuenteAndConfig(byte val){
  byte err = _EPM203Conf_setFuente(conf,val);
  if(!err)
    err = setFuente(val);
  return err;
}

//Direccion
byte EPM203Manejador::getDireccion(){
  return _EPM203Conf_getDireccion(conf);  
}

byte EPM203Manejador::setDireccion(byte val){
  if(val>1)
    return ERR_RANGE;
  write(0x1b);  
  write(0x7b); 
  write(val);
  /*Save setup parameters*/
  write(0x1B);
  write(0x73);
}

byte EPM203Manejador::setDireccionAndConfig(byte val){
  byte err = _EPM203Conf_setDireccion(conf,val);
  if(!err)
    err = setDireccion(val);
  return err;
}


