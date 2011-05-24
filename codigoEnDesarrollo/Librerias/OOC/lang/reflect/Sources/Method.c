#include <stdarg.h>
#include "Method.h"


#pragma DATA_SEG METHOD_DATA                                            
#pragma CODE_SEG METHOD_CODE 
#pragma CONST_SEG DEFAULT

void Method_defConstructor(void* _self,va_list * args);

const struct Class Method={
  CLASS_INITIALIZATION(Class,Method,Object,Method_defConstructor,NULL,NULL,NULL)
};

void Method_constructor(void* _self,pMethod method,void * Obj){
  struct Method * m = _self;
  m->Obj=Obj;
  m->pmethod=method;
}

void Method_defConstructor(void* _self,va_list * args){
  Method_constructor(_self, va_arg(*args,pMethod),va_arg(*args,void*));
}


void Method_execute(void * self){
  struct Method *_self=self;
  (*( _self->pmethod))(_self->Obj);
}

pMethod Method_getMethod(void * self){
  struct Method *_self=self;
  return _self->pmethod;
}

void Method_setMethod(void * self,pMethod method){
  struct Method *_self=self;
  if(method==NULL)
    return; //error cambiar por excepcion
  _self->pmethod = method;
}


void * Method_getObj(void * self){
  struct Method *_self=self;
  return _self->Obj;
}

#pragma CODE_SEG METHOD_CODE


