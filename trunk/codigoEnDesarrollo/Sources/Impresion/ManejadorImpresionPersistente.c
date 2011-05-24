#include "System.h"
#include "ManejadorImpresionPersistente.h"
#include "ManejadorImpresion_protected.h"
#include "OutputStream.h"
#include "EPM203Manejador.h"

#include "Getter.h"


void * ManejadorImpresionPersistente_defCtor(void * _self,va_list * args);

void ManejadorImpresionPersistente_imprimir(void * _self);
int ManejadorImpresionPersistente_getIntervalo(void * _self);
void ManejadorImpresionPersistente_setIntervalo(void * _self,int intervalo);
bool ManejadorImpresionPersistente_getHabilitado(void * _self);
void ManejadorImpresionPersistente_setHabilitado(void * _self, bool habilitar);

const struct ManejadorImpresionClass ManejadorImpresionPersistente={
  MANEJADOR_IMPRESION_CLASS_INITIALIZATION(ManejadorImpresionClass,
                       ManejadorImpresionPersistente,
                       ManejadorImpresion,
                       ManejadorImpresionPersistente_defCtor,
                       Object_dtor,
                       Object_differ,
                       Object_puto,
                       ManejadorImpresionPersistente_imprimir,
                       ManejadorImpresionPersistente_getIntervalo,
                       ManejadorImpresionPersistente_setIntervalo,
                       ManejadorImpresionPersistente_getHabilitado,
                       ManejadorImpresionPersistente_setHabilitado)  
};


void ManejadorImpresionPersistente_printHora(void * _self){
  struct ManejadorImpresionPersistente *self = _self;
  ManejadorImpresion_print(_self);
  Timer_Stop(&self->mt);  
}

void * ManejadorImpresionPersistente_ctor(void * _self,struct OutputStream * os,struct MIPConf * conf){
  struct ManejadorImpresionPersistente *self = _self;
  
  ManejadorImpresion_ctor(_self,os);
  self->conf = conf;
  newAlloced(&self->mt,&MethodTimer,(ulong)2000,ManejadorImpresionPersistente_printHora,_self);
  Timer_Stop(&self->mt);
  ManejadorImpresionPersistente_setIntervalo(_self,_MIPConf_getIntervalo(self->conf));
  if(_MIPConf_getHabilitado(self->conf))
    ManejadorImpresionPersistente_setHabilitado(_self,TRUE);        
  return _self;
}

void * ManejadorImpresionPersistente_defCtor(void * _self,va_list * args){
  return ManejadorImpresionPersistente_ctor(_self,va_arg(*args,void*),va_arg(*args,void*));
}

void ManejadorImpresionPersistente_imprimir(void * _self){
  struct ManejadorImpresionPersistente* self = _self;
  
  if(! BaseTiempo_getConfigurado(Sys_getBaseTiempo()) 
  && ManejadorImpresion_getCuentaParcial(_self) ==0){
    char str[30];

    if(classOf(ManejadorImpresion_getOS(_self))!=&EPM203Manejador){
      
      sprintf(str,"******************\n");
      write(ManejadorImpresion_getOS(_self),str);
      sprintf(str,"Fecha no ingresada\n");
      write(ManejadorImpresion_getOS(_self),str);
      ManejadorImpresion_print(_self);
      return;
    }else{     
      sprintf(str,"*** Fecha no ingresada ***\n");
      write(ManejadorImpresion_getOS(_self),str);
      Timer_Restart(&self->mt);
      return;
    }
  
    
  }
  ManejadorImpresion_print(_self);    
}

/**/
int ManejadorImpresionPersistente_getIntervalo(void * _self){
  struct ManejadorImpresionPersistente* self = _self;

  return _MIPConf_getIntervalo(self->conf);
}

/**/
void ManejadorImpresionPersistente_setIntervalo(void * _self,int intervalo){
  struct ManejadorImpresionPersistente* self = _self;

  if( _MIPConf_setIntervalo(self->conf,intervalo)== ERR_OK ){
    ManejadorImpresion_setIntervalo(_self,intervalo);
  }
    
}

/**/
bool ManejadorImpresionPersistente_getHabilitado(void * _self){
  struct ManejadorImpresionPersistente* self = _self;
  return _MIPConf_getHabilitado(self->conf);
}

/**/
void ManejadorImpresionPersistente_setHabilitado(void * _self, bool habilitar){
  struct ManejadorImpresionPersistente* self = _self;
  
  if( _MIPConf_setHabilitado(self->conf,habilitar) == ERR_OK) 
    ManejadorImpresion_setHabilitado(_self,habilitar);
}



