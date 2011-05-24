#include "EPM203Manejador.h"


#include <assert.h>
#include <termio.h>
#include "OutputStream.h"
#include "WDog1.h"

void EPM203Manejador_defCtor(void * _self,va_list * args);
void EPM203Manejador_write(void * _self,uchar * b);
void EPM203Manejador_writeByte(void * _self,uchar b);

const struct OutputStreamClass EPM203Manejador={
  OUTPUT_STREAM_CLASS_INITIALIZATION(OutputStreamClass,
                                     EPM203Manejador,
                                     OutputStream,
                                     EPM203Manejador_defCtor,
                                     Object_dtor,
                                     Object_differ,
                                     Object_puto,
                                     OutputStream_close,
                                     OutputStream_flush,
                                     EPM203Manejador_write,
                                     EPM203Manejador_writeByte)
};

byte EPM203Manejador_getFuente(void * _self);
byte EPM203Manejador_setFuente(void * _self,byte val);
byte EPM203Manejador_getDireccion(void * _self);
byte EPM203Manejador_setDireccion(void * _self,byte val);
//Ctor
void EPM203Manejador_ctor(void * _self,struct EPM203Conf * conf){
  struct EPM203Manejador * self = _self;
  self->conf = conf;
//  newAlloced(&self->timer,&Timer,(ulong)100); 
  /*9600 Baud sin RTS/DTR*/
  writeByte(_self,0x1d);
  writeByte(_self,0x42);
  writeByte(_self,0x3);
  EPM203Manejador_setFuente(_self,EPM203Manejador_getFuente(self));
  EPM203Manejador_setDireccion(_self,EPM203Manejador_getDireccion(self));
}

void EPM203Manejador_defCtor(void * _self,va_list * args){
  super_ctor(&EPM203Manejador,_self,args);
  EPM203Manejador_ctor(_self,va_arg(*args,void*));  
}

void EPM203Manejador_write(void * _self,uchar * b) {
  struct EPM203Manejador * self = _self;
 // static char res=0;
 // char actual;
  /*pedir status*/
/*  do{
    
    writeByte(_self,0x1b);
    writeByte(_self,0x76);
    actual=TERMIO_GetChar();
  }while(!(actual&0x20));
  */
  /*if(res){
    if(res!=actual)
      while(1);  
  }
  res=actual;
  */
  /*imprimir*/
//  while(!Timer_isfinish(&self->timer))
//    WDog1_Clear();
  
  printf(b);
 // Timer_setTime(&self->timer,300);
}


void EPM203Manejador_writeByte(void * _self,uchar b) {
  struct EPM203Manejador * self = _self;
  printf("%c",b);
}

//Fuente
byte EPM203Manejador_getFuente(void * _self){
  struct EPM203Manejador * self = _self;
  return _EPM203Conf_getFuente(self->conf);  
}

byte EPM203Manejador_setFuente(void * _self,byte val){
  if(val>3)
    return ERR_RANGE;
  writeByte(_self,0x1b);
  writeByte(_self,0x25);  
  writeByte(_self,val);
  /*Save setup parameters*/
  writeByte(_self,0x1B);
  writeByte(_self,0x73);
}

byte EPM203Manejador_setFuenteAndConfig(void * _self,byte val){
  struct EPM203Manejador * self = _self;
  byte err = _EPM203Conf_setFuente(self->conf,val);
  if(!err)
    err = EPM203Manejador_setFuente(_self,val);
  return err;
}

//Direccion
byte EPM203Manejador_getDireccion(void * _self){
  struct EPM203Manejador * self = _self;
  return _EPM203Conf_getDireccion(self->conf);  
}

byte EPM203Manejador_setDireccion(void * _self,byte val){
  if(val>1)
    return ERR_RANGE;
  writeByte(_self,0x1b);  
  writeByte(_self,0x7b); 
  writeByte(_self,val);
  /*Save setup parameters*/
  writeByte(_self,0x1B);
  writeByte(_self,0x73);
}

byte EPM203Manejador_setDireccionAndConfig(void * _self,byte val){
  struct EPM203Manejador * self = _self;
  byte err = _EPM203Conf_setDireccion(self->conf,val);
  if(!err)
    err = EPM203Manejador_setDireccion(_self,val);
  return err;
}


