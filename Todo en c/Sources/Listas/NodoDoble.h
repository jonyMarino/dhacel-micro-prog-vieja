#ifndef _NODO_DOBLE_H
#define _NODO_DOBLE_H

#include "nodo.h"

#pragma DATA_SEG NodoDoble_DATA                                            
#pragma CODE_SEG NodoDoble_CODE 


struct NodoDoble{
  struct Nodo _base;
  struct NodoDoble * prev;
};

void initNodoDoble(void * dato,struct NodoDoble * next,struct NodoDoble * _prev);
    
struct NodoDoble * getPrev(void);

void setPrev(struct NodoDoble * nodo);
  
    


#pragma DATA_SEG DEFAULT                                            
#pragma CODE_SEG DEFAULT 


#endif