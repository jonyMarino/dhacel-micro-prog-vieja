#include "EPM203Manejador.h"
#include "PE_Error.h"


#include <assert.h>
#include <termio.h>

#ifdef _PRINTER

struct EPM203Conf * _conf; 


void EPM203Manejador_init( struct EPM203Conf * configuracion){
  _conf= configuracion;
 
  /*9600 Baud sin RTS/DTR*/
  OutputStream_write(0x1d);
  OutputStream_write(0x42);
  OutputStream_write(0x3);
  EPM203Manejador_setFuente(EPM203Manejador_getFuente());
  EPM203Manejador_setDireccion(EPM203Manejador_getDireccion());
}



//Fuente
byte EPM203Manejador_getFuente(){
  return _EPM203Conf_getFuente(_conf);  
}

byte EPM203Manejador_setFuente(byte val){
  if(val>3)
    return ERR_RANGE;
  OutputStream_write(0x1b);
  OutputStream_write(0x25);  
  OutputStream_write(val);
  /*Save setup parameters*/
  OutputStream_write(0x1B);
  OutputStream_write(0x73);
}

byte EPM203Manejador_setFuenteAndConfig(byte val){
  byte err = _EPM203Conf_setFuente(_conf,val);
  _conf->fuente=val;
  if(!err)
    err = EPM203Manejador_setFuente(val);
  return err;
}

//Direccion
byte EPM203Manejador_getDireccion(){
  return _EPM203Conf_getDireccion(_conf);  
}

byte EPM203Manejador_setDireccion(byte val){
  if(val>1)
    return ERR_RANGE;
  OutputStream_write(0x1b);  
  OutputStream_write(0x7b); 
  OutputStream_write(val);
  /*Save setup parameters*/
  OutputStream_write(0x1B);
  OutputStream_write(0x73);
}

byte EPM203Manejador_setDireccionAndConfig(byte val){
  byte err = _EPM203Conf_setDireccion(_conf,val);
   _conf->direccion=val;
  if(!err)
    err = EPM203Manejador_setDireccion(val);
  return err;
}

#endif