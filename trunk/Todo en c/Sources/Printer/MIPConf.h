#ifndef _M_I_P_CONF_H
#define _M_I_P_CONF_H

#include "Configuraciones.h"
#include "cnfbox.h"
#include "Parametros.h"

struct MIPConf{
  word intervalo;
  word habilitado;  
};


// intervalo
#define _MIPConf_getIntervalo(_self)\
 CONFIGURACION_GET_WORD( _self,intervalo,MIPConf)

#define _MIPConf_setIntervalo(_self,val) \
  CONFIGURACION_SET_WORD_PROM(PRom[R_PrnIntervalo],val)


// habilitado
#define _MIPConf_getHabilitado(_self)\
 CONFIGURACION_GET_WORD( _self,habilitado,MIPConf)

#define _MIPConf_setHabilitado(_self,val) \
  CONFIGURACION_SET_WORD_PROM(PRom[R_PrnHabilitado],val)
  


#endif
