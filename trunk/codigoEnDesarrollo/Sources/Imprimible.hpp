#ifndef _IMPRIMIBLE_HPP
#define _IMPRIMIBLE_HPP


#include "PE_Types.h"
#include "OutputStream.hpp"


#pragma CONST_SEG DEFAULT


class Imprimible {
  public:
    
   virtual void imprimirValor(OutputStream& os)=0;

};

#endif