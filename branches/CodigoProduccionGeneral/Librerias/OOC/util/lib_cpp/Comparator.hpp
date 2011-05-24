#ifndef _COMPARATOR_HPP
#define _COMPARATOR_HPP

#include "PE_Types.h"

class Comparator{
  public:
    virtual bool equals(void*)=0;
    virtual int compare(void*)=0; 
};


#endif