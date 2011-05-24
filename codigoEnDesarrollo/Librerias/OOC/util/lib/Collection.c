#include "Collection.h"

const void * const  Collection=&Object;

bool add(void * _self,void * obj){
  struct CollectionClass * class = classOf(_self);
  return class->add(_self,obj);
}

void clear(void * _self){
  struct CollectionClass * class = classOf(_self);
  class->clear(_self);
}

bool contains(void * _self,void * obj){
  struct CollectionClass * class = classOf(_self);
  return class->contains(_self,obj);
}

bool isEmpty(void * _self){
  struct CollectionClass * class = classOf(_self);
  return class->isEmpty(_self);
}

struct Iterator * iterator(void * _self){
  struct CollectionClass * class = classOf(_self);
  return class->iterator(_self);
}

bool moveOut(void * _self,void * obj){
  struct CollectionClass * class = classOf(_self);
  return class->moveOut(_self,obj);
}

uint size(void * _self){
  struct CollectionClass * class = classOf(_self);
  return class->size(_self);
}
