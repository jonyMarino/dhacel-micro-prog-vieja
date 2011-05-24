#ifndef _PRESENTA_VALOR_HPP
#define _PRESENTA_VALOR_HPP

#include "Imprimible.hpp"
#include "PE_Types.h"
#include "OutputStream.hpp"


#pragma CONST_SEG DEFAULT


class PresentaValor : public Imprimible{
  public:
    PresentaValor(int *valor,char * nombre){
      valorApresentar = valor;
      cartel= nombre;  
    }
    
     int getValor();
    /**/ 
    void imprimirValor(OutputStream& os);

    
    int getDecimales();
    
  private:
    int *valorApresentar;
    char *cartel;
};

#endif