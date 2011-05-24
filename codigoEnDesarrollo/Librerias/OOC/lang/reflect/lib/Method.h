#ifndef _METHOD_H
#define _METHOD_H


#pragma DATA_SEG METHOD_DATA                                            
#pragma CODE_SEG METHOD_CODE 
#pragma CONST_SEG DEFAULT

#include "Object.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*pMethod)(void*);

struct Method{
  void * Class;
  pMethod pmethod;
  void* Obj;
};


extern const struct Class Method;


void Method_constructor(void* _self,pMethod method,void * Obj);


void Method_execute(void * self);

pMethod Method_getMethod(void * self);

void Method_setMethod(void * self,pMethod method);

void * Method_getObj(void * self); 


#define NEW_METHOD(name,method,obj)  \
  struct Method name={               \
    &Method,                         \
    method,                          \
    obj                              \
  }
  
  
#ifdef __cplusplus
}
#endif

#pragma DATA_SEG DEFAULT                                            
#pragma CODE_SEG DEFAULT 

#endif