#ifndef _CONFIGURACION_H
#define _CONFIGURACION_H

#include "IFsh10.h"


#define CONFIGURACION_SET_WORD(_self,atributo,subclass,val)\
  EscribirWord((word)&( (struct subclass*)_self)->atributo,val)    

#define CONFIGURACION_SET_WORD_PROM(DirPRom,val)\
  EscribirWord((word)&DirPRom,val)  

#define CONFIGURACION_GET_WORD( _self,atributo,subclass) \
  ( (struct subclass*)_self)->atributo




#endif