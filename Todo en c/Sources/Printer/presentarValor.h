#ifndef _PRESENTA_VALOR_H
#define _PRESENTA_VALOR_H


#include "PE_Types.h"
#include "OutputStream.h"

struct ArgPresentaValor{
  int *val;
  char * nombre;
};


char * getCartel(struct ArgPresentaValor* arg);
    
int getValor(struct ArgPresentaValor* arg);
    
void imprimirValor(struct ArgPresentaValor* arg);

int getDecimales();
 

#endif