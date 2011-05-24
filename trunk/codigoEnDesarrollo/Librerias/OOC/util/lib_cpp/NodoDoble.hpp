#ifndef _NODO_DOBLE_H
#define _NODO_DOBLE_H

#include "nodo.hpp"

#pragma DATA_SEG NodoDoble_DATA                                            
#pragma CODE_SEG NodoDoble_CODE 

class NodoDoble:public Nodo{
  public:
    NodoDoble(void * dato,NodoDoble * next,NodoDoble * _prev);
    inline NodoDoble * getPrev(void){ return prev; }
    void setPrev(NodoDoble * nodo);
  private:
    NodoDoble * prev;
};


#pragma DATA_SEG DEFAULT                                            
#pragma CODE_SEG DEFAULT 


#endif