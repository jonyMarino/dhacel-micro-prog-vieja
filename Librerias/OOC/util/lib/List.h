#ifndef _LIST_H
#define _LIST_H

#include "Collection.h"


/*C++ 
_class List:public Collection
{
 public:
 
  virtual Object * get(uint index)=0;
}
*/
struct List{
  struct Collection super;
};

struct ListClass{
  struct CollectionClass super;
  struct Object * (*get)(void *_self,uint index);          
};

extern const void * const List;

struct Object * get(void *_self,uint index); 
  
#endif 