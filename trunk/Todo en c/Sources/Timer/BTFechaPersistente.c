#include "BTFechaPersistente.h"
#include "BaseTiempoDS1307.h"
#include "BTFPConf.h"


struct BTFPConf * conf;
/*
** ===================================================================
**     Method      :  BTFechaPersistente_init 
**
**     Description :
** ===================================================================
*/
void BTFechaPersistenteInit(BTFPConf * _conf)
{
  conf=_conf;
}


void salvarFecha(DATEREC * date){
  
  _BTFPConf_setDia(conf,date->Day);
  _BTFPConf_setMes(conf,date->Month);  
  _BTFPConf_setAnio(conf,date->Year);
}

/*
** ===================================================================
**     Method      :  BTFechaPersistente_defConstructor
**
**     Description :
** ===================================================================
*/
byte BTFechaPersistente_setDate(word anio,byte mes,byte dia){
  
  DATEREC date;
  
  BaseTiempoDS1307_setFechaValidada(anio,mes,dia);
  
  getFecha(&date);
  
  salvarFecha(&date);
  
  return 0;
}
