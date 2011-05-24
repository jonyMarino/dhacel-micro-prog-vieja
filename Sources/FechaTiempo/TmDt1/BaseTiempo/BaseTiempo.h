#ifndef _BASETIEMPO_H
#define _BASETIEMPO_H


#include "TmDt1.h"
#include "IBaseTiempo.h"

#include "RlxMTimer.h"

struct BaseTiempoClass{
  struct IBaseTiempoClass super;
  void (*inc1Segundo)(void * _self);
};

struct BaseTiempo {
  struct TmDt1 super;
  int segundosAcumulados;
  struct RlxMTimer timer;
  bool configurado;
};

#define BASE_TIEMPO_CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto,getTiempo,setTiempo,getFecha,setFecha,isConfigurado,inc1Segundo)\
  I_BASE_TIEMPO_CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto,getTiempo,setTiempo,getFecha,setFecha,isConfigurado),\
  inc1Segundo
  
extern const struct Class BaseTiempoClass;
extern const struct BaseTiempoClass BaseTiempo;

bool BaseTiempo_habilitada(void * _self);
void BaseTiempo_habilitar(void * _self,bool habilitar);

bool BaseTiempo_getConfigurado(void * _self);

#endif
