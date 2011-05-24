#ifndef _LIST_HPP
#define _LIST_HPP

#include "Collection.hpp"

class List:public Collection
{
 public:
  virtual void * get(unsigned int index)=0;
};


  
#endif 