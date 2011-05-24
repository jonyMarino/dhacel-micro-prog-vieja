#ifndef _NODO_HPP
#define _NODO_HPP


#pragma CODE_SEG  NODO_CODE
#pragma DATA_SEG  NODO_DATA
#pragma CONST_SEG DEFAULT

class Nodo {
  public:
    Nodo(void * _dato,Nodo * _next);
    inline void* getDato(void){return dato;};
    void  setDato(void* dato);
    inline Nodo* getNext(void){ return next;};
    void  setNext(Nodo * nodo);
    
  private:
    unsigned char differ(Nodo* node);
  	void * dato;
  	Nodo * next;
};


#pragma CODE_SEG  DEFAULT
#pragma DATA_SEG  DEFAULT
#pragma CONST_SEG DEFAULT

#endif