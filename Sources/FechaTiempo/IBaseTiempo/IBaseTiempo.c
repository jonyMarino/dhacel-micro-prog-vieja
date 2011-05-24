#include "IBaseTiempo.h" 

const struct IBaseTiempoClass IBaseTiempo={
NULL,
};

bool isConfigurado(void* _self){
  struct IBaseTiempoClass * _class =(struct IBaseTiempoClass *) classOf(_self);
  return _class->isConfigurado(_self);   
}