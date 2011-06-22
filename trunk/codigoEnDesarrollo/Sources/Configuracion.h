#ifndef _CONFIGURACION_H
#define _CONFIGURACION_H

#include "IFsh10.h"

typedef word fbyte;

#define CONFIGURACION_SET_WORD(self,atributo,subclass,valor)\
  EscribirWord((word)&( (struct subclass*)self)->atributo,valor)    

#define CONFIGURACION_GET_WORD( self,atributo,subclass) \
  ( (struct subclass*)self)->atributo


#define CONFIGURACION_SET_BYTE(self,atributo,subclass,valor)\
  EscribirWord((word)&( (struct subclass*)self)->atributo,valor)    

#define CONFIGURACION_GET_BYTE( self,atributo,subclass) \
  ( (struct subclass*)self)->atributo

#endif