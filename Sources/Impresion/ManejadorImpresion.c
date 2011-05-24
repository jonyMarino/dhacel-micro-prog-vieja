#include "System.h"
#include "ManejadorImpresion.h"
#include "ManejadorImpresion_protected.h"
#include "BaseTiempo.h"
#include "OutputStream.h"
#include "Sensor.h"

#include "Getter.h"
#include "SensorAdaptador.h"//cambiar: sacar


void * ManejadorImpresion_defCtor(void * _self,va_list * args);

const struct Class ManejadorImpresionClass;

struct ManejadorImpresionClass ManejadorImpresion={
  MANEJADOR_IMPRESION_CLASS_INITIALIZATION(ManejadorImpresionClass,
                       ManejadorImpresion,
                       Object,
                       ManejadorImpresion_defCtor,
                       Object_dtor,
                       Object_differ,
                       Object_puto,
                       ManejadorImpresion_print,
                       ManejadorImpresion_getIntervalo,
                       ManejadorImpresion_setIntervalo,
                       ManejadorImpresion_getHabilitado,
                       ManejadorImpresion_setHabilitado)  
};



void * ManejadorImpresion_ctor(void * _self,struct OutputStream * os){
  struct ManejadorImpresion *self = _self;
  struct ManejadorImpresionClass *class= classOf(_self);
  LinkedList_Construct(_self);

  self->os = os;
  self->cuentaParcial = 0;
  
  newAlloced(&self->timer,&RlxMTimer,(ulong)1000,class->imprimir,_self);
  Timer_Stop(&self->timer);
  
  
  return _self;
}

void * ManejadorImpresion_defCtor(void * _self,va_list * args){
  return ManejadorImpresion_ctor(_self,va_arg(*args,void*));
}

void ManejadorImpresion_print(void * _self){
  struct ManejadorImpresion* self = _self;
  struct LinkedListIterator it;
  TIMEREC time;
  int pos = 0;
  char str[13];
  void * obj = LinkedList_get(_self,0);
  
  if( self->cuentaParcial++ ==0){
//Fecha
    DATEREC date;    
    Sys_getDate(&date);
    
    //cambiar por date.write
    sprintf(str,"%02d/%02d/%d ",(int)date.Day,(int)date.Month,(int)date.Year );
    write(self->os,str);
    pos = 11;   //son 10 lugares
  }
  
  if(self->cuentaParcial==10)
    self->cuentaParcial=0;
  
  
  Sys_getTime(&time);
  //cambiar: por time.write
  sprintf(str,"%d:%02d:%02d ",(int)time.Hour,(int)time.Min,(int)time.Sec );
  write(self->os,str);
  
  pos += 11;
  
  while( pos++ < 19){
    writeByte(self->os,' ');
  }
  
  if(obj){
    
    if(instanceOf(obj,&Sensor)){ 
      //cambiar por sensor.printVal
      SensorAdaptador_printVal(obj,self->os);
    }
  }
  writeByte(self->os,'\n');
  //cambiar por time.write
  
  
/*  LinkedList_linkedListIterator(_self,&it);
  
  

  while(Iterator_hasNext(&it)){
    void* obj = Iterator_next(&it);  
    puto(obj,self->os);
  }
  
  deleteAlloced(&it);
  LinkedList_linkedListIterator(_self,&it);
  
  deleteAlloced(&it);
  */
}

/**/
int ManejadorImpresion_getCuentaParcial(void * _self){
  struct ManejadorImpresion* self = _self;
  return self->cuentaParcial;
}

/**/
struct OutputStream * ManejadorImpresion_getOS(void *_self){
  struct ManejadorImpresion* self = _self;
  return self->os;
}
/**/
int ManejadorImpresion_getIntervalo(void * _self){
  struct ManejadorImpresion* self = _self;

  return Timer_Sys_getTime(&self->timer)/1000;
}

/**/
void ManejadorImpresion_setIntervalo(void * _self,int intervalo){
  struct ManejadorImpresion* self = _self;

  if( ManejadorImpresion_getHabilitado(_self) )
    Timer_setTime(&self->timer,(ulong)intervalo*1000);
  else{
    Timer_setTime(&self->timer,(ulong)intervalo*1000);
    Timer_Stop(&self->timer);   
  }
    
}

/**/
bool ManejadorImpresion_getHabilitado(void * _self){
  struct ManejadorImpresion* self = _self;
  return Timer_isfinish(&self->timer)==FALSE;
}

/**/
void ManejadorImpresion_setHabilitado(void * _self, bool habilitar){
  struct ManejadorImpresion* self = _self;
  
  if(habilitar){ 
    if( !ManejadorImpresion_getHabilitado(_self) )
      Timer_Restart(&self->timer);  
  }else
    Timer_Stop(&self->timer);
}

/*Virtuales*/
int getIntervaloMI(void * _self){
  struct ManejadorImpresionClass * class= classOf(_self); 
  return   class->getIntervalo(_self); 
}

void setIntervaloMI(void * _self,int intervalo){
  struct ManejadorImpresionClass * class= classOf(_self);
  class->setIntervalo(_self,intervalo);
}

bool getHabilitadoMI(void * _self){
  struct ManejadorImpresionClass * class= classOf(_self);
  return class->getHabilitado(_self);
}

void setHabilitadoMI(void * _self, bool habilitar){
  struct ManejadorImpresionClass * class= classOf(_self);
  class->setHabilitado(_self,habilitar);
}



