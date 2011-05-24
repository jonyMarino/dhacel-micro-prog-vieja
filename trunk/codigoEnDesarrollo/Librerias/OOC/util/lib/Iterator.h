#ifndef _ITERATOR_H
#define _ITERATOR_H

#include "Object.h"
#include "PE_Types.h"

struct Iterator{
  struct Object super;
};


struct IteratorClass{
  struct Class super;
  void * (*next)(void *);
  bool (*hasNext)(void*);
};

#define Iterator_next(it) \
  					(*(**((struct IteratorClass**)it)).next)(it)

#define Iterator_hasNext(it) \
  					(*(**((struct IteratorClass**)it)).hasNext)(it)


#define ITERATOR_CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto,next,hasNext)\
 CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto),  \
 next, \
 hasNext
#endif