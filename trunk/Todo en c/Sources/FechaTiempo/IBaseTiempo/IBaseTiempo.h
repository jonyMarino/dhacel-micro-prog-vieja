#ifndef _I_BASETIEMPO_H
#define _I_BASETIEMPO_H

#include "FechaTiempo.h"

struct IBaseTiempo{
  struct FechaTiempo super;
};

struct IBaseTiempoClass{
  struct FechaTiempoClass super;
  bool (*isConfigurado)(void*);  
};

extern const struct IBaseTiempoClass IBaseTiempo;

bool isConfigurado(void*);

#define I_BASE_TIEMPO_CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto,getTiempo,setTiempo,getFecha,setFecha,isConfigurado)\
  FECHA_TIEMPO_CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto,getTiempo,setTiempo,getFecha,setFecha),\
  isConfigurado


#endif
