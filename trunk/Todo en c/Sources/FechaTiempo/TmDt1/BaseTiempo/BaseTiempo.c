#include "BaseTiempo.h"
#include "BaseTiempo_protected.h"
#include "TmDt1_protected.h"

extern const struct Class BaseTiempoClass;

void BaseTiempo_defConstructor(void * self,va_list * args);

const struct BaseTiempoClass BaseTiempo = {
    BASE_TIEMPO_CLASS_INITIALIZATION(BaseTiempoClass,
                             BaseTiempo,  
                             TmDt1,
                             BaseTiempo_defConstructor,
                             Object_dtor,
                             Object_differ,
                             Object_puto,
                             TmDt1_GetTime,
                             BaseTiempo_setTime,
                             TmDt1_GetDate,
                             TmDt1_setFechaValidada,
                             BaseTiempo_getConfigurado,
                             BaseTiempo_incUnSegundo)  
};



/*
** ===================================================================
**     Method      :  BaseTiempo_constructor (bean TimeDate)
**
**     Description :
** ===================================================================
*/
void BaseTiempo_constructor(void * _self,word Year,byte Month,byte Day,byte Hour,byte Min,byte secs,bool start)
{
  struct BaseTiempo * self =(struct BaseTiempo *) _self;
  struct BaseTiempoClass * _class =(struct BaseTiempoClass *) classOf(_self);
  TmDt1_Constructor(_self,Year,Month,Day,Hour,Min,secs);
  self->configurado = FALSE;
  
  newAlloced(&self->timer,&RlxMTimer,(ulong)1000,_class->inc1Segundo,_self);
  if(!start)
    Timer_Stop(&self->timer);
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
void BaseTiempo_defConstructor(void * self,va_list * args){
  BaseTiempo_constructor(self,va_arg(*args,word),va_arg(*args,int),va_arg(*args,int),va_arg(*args,int),va_arg(*args,int),va_arg(*args,int),va_arg(*args,int));  
}

/*
** ===================================================================
**     Method      :  BTFechaPersistente_defConstructor
**
**     Description :
** ===================================================================
*/
byte BaseTiempo_setTime(void * _self,byte horas,byte min,byte segs){
  struct BaseTiempo * self =(struct BaseTiempo *) _self;
  super_setTiempoValidado(&BaseTiempo,_self,horas,min,segs);
  self->configurado = TRUE;
  return 0;
}

/*
** ===================================================================
**     Method      :  BaseTiempo_incUnSegundo
**
**     Description :
** ===================================================================
*/
void BaseTiempo_incUnSegundo(void * _self){
  TmDt1_Inc(_self,1);
}

/*
** ===================================================================
**     Method      :  BaseTiempo_incUnSegundo
**
**     Description :
** ===================================================================
*/
bool BaseTiempo_habilitada(void * _self){
  struct BaseTiempo * self =(struct BaseTiempo *) _self;
  return Timer_isfinish(&self->timer)==FALSE;
}

/*
** ===================================================================
**     Method      :  BaseTiempo_incUnSegundo
**
**     Description :
** ===================================================================
*/
void BaseTiempo_habilitar(void * _self,bool habilitar){
  struct BaseTiempo * self =(struct BaseTiempo *) _self;
  if(habilitar){
    if(Timer_isfinish(&self->timer))
      Timer_Restart(&self->timer);
  }else{
    Timer_Stop(&self->timer);    
  }
}

/*
** ===================================================================
**     Method      :  BTFechaPersistente_incUnSegundo
**
**     Description :
** ===================================================================
*/
bool BaseTiempo_getConfigurado(void * _self){
  struct BaseTiempo * self =(struct BaseTiempo *) _self;
  return self->configurado;
}



