#ifndef _GETTER_W_NAME_H
#define _GETTER_W_NAME_H

#include "Object.h"
#include "getter.h"

#pragma CONST_SEG DEFAULT

struct GetterWName{
  struct Object super;
};



struct GetterWNameClass{ 
  struct GetterClass super;
  char* (*getName)(void*);
};

extern const struct Class GetterWNameClass;
extern const struct GetterWNameClass GetterWName;

#define _GetterWN_get_Name(_getter) \
              (*((*(struct  GetterWNameClass**)_getter)->getName))(_getter) 


#define GETTER_W_NAME_CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto,get,print,getName)\
  GETTER_CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto,get,print),\
  getName 
  
#endif
