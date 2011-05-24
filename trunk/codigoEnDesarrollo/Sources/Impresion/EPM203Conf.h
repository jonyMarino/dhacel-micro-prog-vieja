#ifndef _EPM203_CONF_H
#define _EPM203_CONF_H

#include "Configuracion.h"

struct EPM203Conf {
  fbyte direccion;
  fbyte fuente;
};

// direccion
#define _EPM203Conf_getDireccion(self)\
 CONFIGURACION_GET_BYTE( self,direccion,EPM203Conf)

#define _EPM203Conf_setDireccion(self,valor) \
  CONFIGURACION_SET_BYTE( self,direccion,EPM203Conf,valor)

//fuente 
#define _EPM203Conf_getFuente(self)\
 CONFIGURACION_GET_BYTE( self,fuente,EPM203Conf)

#define _EPM203Conf_setFuente(self,valor) \
  CONFIGURACION_SET_BYTE( self,fuente,EPM203Conf,valor)
   
#endif
