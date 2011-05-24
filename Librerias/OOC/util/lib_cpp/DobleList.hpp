#ifndef _DOBLE_LIST_HPP
#define _DOBLE_LIST_HPP

#include "NodoDoble.hpp"
#include "LinkedList.hpp"

class DobleList:public LinkedList{
  public:
    DobleList();
    void * differ(DobleList * _l);
    unsigned char deleteNodo(NodoDoble * _n);
    NodoDoble * insertFirst(void * dato);
    void * getDato(unsigned int nNodo);
    unsigned int cantidad(void);
  private:
    NodoDoble* getNodoDoble();
};

#endif