#ifndef _ITERATOR_HPP
#define _ITERATOR_HPP

#include "PE_Types.h"

class Iterator{
  public:
    virtual void * next(void)=0;
    virtual bool hasNext(void)=0;
};


#endif