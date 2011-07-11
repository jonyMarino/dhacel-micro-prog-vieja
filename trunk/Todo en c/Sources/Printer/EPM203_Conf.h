#ifndef _EPM203_CONF_H
#define _EPM203_CONF_H

#include "Configuraciones.h"
#include "cnfbox.h"
#include "Parametros.h"

struct EPM203Conf {
  word direccion;
  word fuente;
};


// direccion
#define _EPM203Conf_getDireccion(_self)\
 CONFIGURACION_GET_WORD( _self,direccion,EPM203Conf)

#define _EPM203Conf_setDireccion(_self,val) \
  CONFIGURACION_SET_WORD_PROM(PRom[R_PrnDireccion],val)
  

//fuente 
#define _EPM203Conf_getFuente(_self)\
   CONFIGURACION_GET_WORD( _self,fuente,EPM203Conf)

#define _EPM203Conf_setFuente(_self,val) \
  CONFIGURACION_SET_WORD_PROM(PRom[R_PrnFuente],val)
  
   
#endif