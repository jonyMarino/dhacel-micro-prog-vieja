#ifndef _GETTER_H
#define _GETTER_H

#include "Object.h"

struct Getter{
  struct  Object super;
};

struct  GetterClass{
  struct Class super;
  int (*get)(void*);
  void (*print)(void*,uchar); 
};


extern const struct Class GetterClass;
extern const struct GetterClass Getter; 


#define _GetterPrint(_getter,num_disp) \
              (*((*(struct  GetterClass**)_getter)->print))(_getter,num_disp) 
              
#define _Getter_getVal(_getter) \
              (*((*(struct  GetterClass**)_getter)->get))(_getter) 


#define GETTER_CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto,get,print)\
  CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto),\
  get,  \
  print
  
#endif