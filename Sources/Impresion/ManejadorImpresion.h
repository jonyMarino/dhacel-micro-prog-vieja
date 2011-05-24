#ifndef _MANEJADORIMPRESION_H
#define _MANEJADORIMPRESION_H


#include "Object.h"
#include "LinkedList.h"
#include "RlxMTimer.h"

struct ManejadorImpresionClass {
  struct Class super;
  void (*imprimir)(void * _self);
  int (*getIntervalo)(void * _self);
  void (*setIntervalo)(void * _self,int intervalo);
  bool (*getHabilitado)(void * _self);
  void (*setHabilitado)(void * _self, bool habilitar);
};

#define MANEJADOR_IMPRESION_CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto,imprimir,getIntervalo,setIntervalo,getHabilitado,setHabilitado)\
  CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto),\
  imprimir, \
  getIntervalo, \
  setIntervalo, \
  getHabilitado,\
  setHabilitado 

struct ManejadorImpresion {
    struct LinkedList super;
    struct OutputStream * os;
    struct RlxMTimer timer;
    byte cuentaParcial;
};

extern const struct Class ManejadorImpresionClass;
extern const struct ManejadorImpresionClass ManejadorImpresion;

void * ManejadorImpresion_ctor(void * _self,struct OutputStream * os);
struct OutputStream * ManejadorImpresion_getOS(void *_self);

/*Virtuales*/
int getIntervaloMI(void * _self);

void setIntervaloMI(void * _self,int intervalo);

bool getHabilitadoMI(void * _self);

void setHabilitadoMI(void * _self, bool habilitar);


#endif
