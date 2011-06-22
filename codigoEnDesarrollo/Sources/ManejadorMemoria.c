#include "ManejadorMemoria.h"

word super_getWord(void * __class,void* _self,word*address){
  struct ManejadorMemoriaClass * _class = (struct ManejadorMemoriaClass *) super(__class);
  if(_self && _class->getWord)
  return _class->getWord(_self,address);
}

byte super_setWord(void * __class, void* _self,word*address,word data){
  struct ManejadorMemoriaClass *_class = (struct ManejadorMemoriaClass *) super(__class);
  if(_self && _class->setWord)
  return _class->setWord(_self,address,data);
}