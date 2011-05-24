#ifndef _COMPARATOR_H
#define _COMPARATOR_H

#include "Object.h"
#include "PE_Types.h"

struct Comparator{
  void * _class;  
};

struct ComparatorClass{
  struct Class super;
  bool (*equals)(void *,void*);
  int (*compare)(void*,void*);
};

extern const struct Class ComparatorClass;
extern const struct ComparatorClass Comparator; 

#define _Comparator_equals(comparator,obj) \
          (*(*(struct ComparatorClass**)comparator)->equals)(comparator,obj)

#define _Comparator_compare(comparator,obj) \
          (*(*(struct ComparatorClass**)comparator)->compare)(comparator,obj)


#define COMPARATOR_CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto,equals,compare)\
  CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto),\
  equals,  \
  compare
  
#endif