#ifndef _B_T_F_P_CONF_H
#define _B_T_F_P_CONF_H

#include "Configuracion.h"

struct BTFPConf{
  fbyte dia;
  fbyte mes;
  word anio;  
};

// dia
#define _BTFPConf_getDia(self)\
 CONFIGURACION_GET_BYTE( self,dia,BTFPConf)

#define _BTFPConf_setDia(self,valor) \
  CONFIGURACION_SET_BYTE( self,dia,BTFPConf,valor)
  
// mes
#define _BTFPConf_getMes(self)\
 CONFIGURACION_GET_BYTE( self,mes,BTFPConf)

#define _BTFPConf_setMes(self,valor) \
  CONFIGURACION_SET_BYTE( self,mes,BTFPConf,valor)

// anio
#define _BTFPConf_getAnio(self)\
 CONFIGURACION_GET_WORD( self,anio,BTFPConf)

#define _BTFPConf_setAnio(self,valor) \
  CONFIGURACION_SET_WORD( self,anio,BTFPConf,valor)


#endif