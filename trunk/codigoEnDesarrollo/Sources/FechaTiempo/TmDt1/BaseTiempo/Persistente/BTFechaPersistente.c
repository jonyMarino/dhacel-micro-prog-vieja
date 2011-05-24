#include "BTFechaPersistente.h"
#include "BaseTiempo_protected.h"


#pragma DATA_SEG BTFechaPersistente_DATA                                            
#pragma CODE_SEG BTFechaPersistente_CODE 
#pragma CONST_SEG DEFAULT

void BTFechaPersistente_defConstructor(void * self,va_list * args);

void BTFechaPersistente_incUnSegundo(void * _self);

byte BTFechaPersistente_setDate(void * _self,word anio,byte mes,byte dia);

const struct BaseTiempoClass BTFechaPersistente = {
    BASE_TIEMPO_CLASS_INITIALIZATION(BaseTiempoClass,
                             BTFechaPersistente,  
                             BaseTiempo,
                             BTFechaPersistente_defConstructor,
                             Object_dtor,
                             Object_differ,
                             Object_puto,
                             TmDt1_GetTime,
                             BaseTiempo_setTime,
                             TmDt1_GetDate,
                             BTFechaPersistente_setDate,
                             BaseTiempo_getConfigurado,
                             BTFechaPersistente_incUnSegundo)  
};



/*
** ===================================================================
**     Method      :  BTFechaPersistente_constructor 
**
**     Description :
** ===================================================================
*/
void BTFechaPersistente_constructor(void * _self,struct BTFPConf* conf)
{
  struct BTFechaPersistente * self =(struct BTFechaPersistente *) _self;
  BaseTiempo_constructor(_self,_BTFPConf_getAnio(conf),_BTFPConf_getMes(conf),_BTFPConf_getDia(conf),0,0,0,TRUE);
  self->conf = conf;
}
/*
** ===================================================================
**     Method      :  BTFechaPersistente_defConstructor
**
**     Description :
** ===================================================================
*/
void BTFechaPersistente_defConstructor(void * self,va_list * args){
  BTFechaPersistente_constructor(self,va_arg(*args,struct BTFPConf*));  
}

/** @fn void salvarFecha(void * _self,DATEREC * date)
*/
static void salvarFecha(void * _self,DATEREC * date){
  struct BTFechaPersistente * self =(struct BTFechaPersistente *) _self;
  _BTFPConf_setDia(self->conf,date->Day);
  _BTFPConf_setMes(self->conf,date->Month);  
  _BTFPConf_setAnio(self->conf,date->Year);
}

/*
** ===================================================================
**     Method      :  BTFechaPersistente_defConstructor
**
**     Description :
** ===================================================================
*/
byte BTFechaPersistente_setDate(void * _self,word anio,byte mes,byte dia){
  struct BTFechaPersistente * self =(struct BTFechaPersistente *) _self;
  DATEREC date;
  
  super_setFechaValidada(&BTFechaPersistente,_self,anio,mes,dia);
  getFecha(_self,&date);
  salvarFecha(_self,&date);
  return 0;
}
/*
** ===================================================================
**     Method      :  BTFechaPersistente_incUnSegundo
**
**     Description :
** ===================================================================
*/
void BTFechaPersistente_incUnSegundo(void * _self){
  DATEREC dateTmp;
  DATEREC date;
  
  getFecha(_self,&dateTmp);
  BaseTiempo_incUnSegundo(_self);
  getFecha(_self,&date);
  
  if(memcmp(&dateTmp,&date,sizeof(DATEREC))!=0)
    salvarFecha(_self,&date);  
}

