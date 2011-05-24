//#include "RelojInternoPersistenciaDS1307_protected.h"
#include "RelojInternoPersistenciaDS1307.h"
#include "BaseTiempo_protected.h"
#include "PE_Error.h"

void RelojInternoPersistenciaDS1307_defConstructor(void * self,va_list * args);
byte RelojInternoPersistenciaDS1307_setTiempoValidado(void * _self,byte horas,byte min,byte segs);
byte RelojInternoPersistenciaDS1307_setFechaValidada(void * self,word Year,byte Month,byte Day);
bool RelojInternoPersistenciaDS1307_getConfigurado(void * _self);

const struct BaseTiempoClass RelojInternoPersistenciaDS1307 = {
    BASE_TIEMPO_CLASS_INITIALIZATION(BaseTiempoClass,
                             RelojInternoPersistenciaDS1307,  
                             BaseTiempo,
                             RelojInternoPersistenciaDS1307_defConstructor,
                             Object_dtor,
                             Object_differ,
                             Object_puto,
                             TmDt1_GetTime,
                             RelojInternoPersistenciaDS1307_setTiempoValidado,
                             TmDt1_GetDate,
                             RelojInternoPersistenciaDS1307_setFechaValidada,
                             RelojInternoPersistenciaDS1307_getConfigurado,
                             BaseTiempo_incUnSegundo)  
};


/*
** ===================================================================
**     Method      :  RelojInternoPersistenciaDS1307_constructor (bean TimeDate)
**
**     Description :
** ===================================================================
*/
void RelojInternoPersistenciaDS1307_constructor(void * _self)
{
  struct RelojInternoPersistenciaDS1307 * self = _self;
    
  RelojInternoPersistenciaDS1307_sincronizar(_self);
  
}
/*
** ===================================================================
**     Method      :  TmDt1_DefConstructor (bean TimeDate)
**
**     Description :
**         This method is internal. It is used by Processor Expert
**         only.
** ===================================================================
*/
void RelojInternoPersistenciaDS1307_defConstructor(void * _self,va_list * args){
  struct RelojInternoPersistenciaDS1307 * self = _self;
  newAlloced(&self->ds1307,&BaseTiempoDS1307);
  super_ctor(&RelojInternoPersistenciaDS1307,self,args);
  RelojInternoPersistenciaDS1307_constructor(self);  
}


/*
** ===================================================================
**     Method      :  RelojInternoPersistenciaDS1307_sincronizar
**     Description :
** ===================================================================
*/
void RelojInternoPersistenciaDS1307_sincronizar(void *_self){
  struct RelojInternoPersistenciaDS1307 * self = _self;
  TIMEREC tiempo;
  DATEREC fecha;
  
  getTiempo(&self->ds1307,&tiempo);
  super_setTiempoValidado(&RelojInternoPersistenciaDS1307,self,tiempo.Hour,tiempo.Min,tiempo.Sec);
  getFecha(&self->ds1307,&fecha);
  super_setFechaValidada(&RelojInternoPersistenciaDS1307,self,fecha.Year,fecha.Month,fecha.Day);
}

/*
** ===================================================================
**     Method      :  BTFechaPersistente_defConstructor
**
**     Description :
** ===================================================================
*/
byte RelojInternoPersistenciaDS1307_setTiempoValidado(void * _self,byte horas,byte min,byte segs){
  struct RelojInternoPersistenciaDS1307 * self = _self;
  byte err = super_setTiempoValidado(&RelojInternoPersistenciaDS1307,_self,horas,min,segs);
  if(!err) {
    err = setTiempo(&self->ds1307,horas,min,segs);  
  }
  return err;
}

/*
** ===================================================================
**     Method      :  TmDt1_SetDate (bean TimeDate)
**
**     Description :
**         Set a new actual date.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Year            - Years (16-bit unsigned integer)
**         Month           - Months (8-bit unsigned integer)
**         Day             - Days (8-bit unsigned integer)
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_RANGE - Parameter out of range
** ===================================================================
*/
byte RelojInternoPersistenciaDS1307_setFechaValidada(void * _self,word Year,byte Month,byte Day)
{
  struct RelojInternoPersistenciaDS1307 * self = _self;
    byte err = super_setFechaValidada(&RelojInternoPersistenciaDS1307,_self,Year,Month,Day);
  if(!err) {
    err = setFecha(&self->ds1307,Year,Month,Day);  
  }
  return err;
}

/*
** ===================================================================
**     Method      :  RelojInternoPersistenciaDS1307DS1307_getConfigurado
**
**     Description :
** ===================================================================
*/
bool RelojInternoPersistenciaDS1307_getConfigurado(void * _self){
  struct RelojInternoPersistenciaDS1307 * self = _self;
  return TRUE;
}