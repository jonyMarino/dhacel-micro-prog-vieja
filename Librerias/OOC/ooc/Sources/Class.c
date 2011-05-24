/*  MODULE Class.c*/
#include <stdlib.h>
#include <assert.h>

#include "Class.h"

void * newObj(const struct Tclass * _class,...){ 
    void * p;
    
    DisableInterrupts;
    p = malloc(_class->size);
    EnableInterrupts;
    
    assert(p);
    #ifdef NDEBUG
      if(!p)
        return NULL;
    #endif
		
		*(const struct Tclass **)p = _class;
    
    if (_class->constructor)
    { 
      va_list ap;
      va_start(ap, _class);
      _class->constructor(p, & ap);
      va_end(ap);
    }
    return p;  
}

void newAllocObj(void *self,const struct Tclass * _class,...){
  *(const struct Tclass **)self=_class;
    if (_class->constructor)
    { 
      va_list ap;
      va_start(ap, _class);
      _class->constructor(self, & ap);
      va_end(ap);
    }    
}


void deleteObj(const struct Tclass *** self){
  if (self && *self && ** self && (** self)->destructor)
    *self = (** self) ->destructor(*self);
  DisableInterrupts;
  free(*self);
  EnableInterrupts;
  *self=NULL;
}

void DestructObj(const struct Tclass ** self){
  if (self && * self && (* self)->destructor)
    (void)(* self) ->destructor(self);
}
