#include "List.h"

const void *const  List = &Collection;
struct Object * get(void *_self,uint index){
  struct ListClass * class = classOf(_self);
  return class->get(_self,index);
}