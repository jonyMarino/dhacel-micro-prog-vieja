#ifndef _M_I_P_CONF_H
#define _M_I_P_CONF_H

#include "Configuracion.h"

struct MIPConf{
  word intervalo;
  fbyte habilitado;  
};

// intervalo
#define _MIPConf_getIntervalo(self)\
 CONFIGURACION_GET_WORD( self,intervalo,MIPConf)

#define _MIPConf_setIntervalo(self,valor) \
  CONFIGURACION_SET_WORD( self,intervalo,MIPConf,valor)


// habilitado
#define _MIPConf_getHabilitado(self)\
 CONFIGURACION_GET_BYTE( self,habilitado,MIPConf)

#define _MIPConf_setHabilitado(self,valor) \
  CONFIGURACION_SET_BYTE( self,habilitado,MIPConf,valor)
  


#endif
