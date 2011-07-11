#ifndef _NODO_H
#define _NODO_H


#pragma CODE_SEG  NODO_CODE
#pragma DATA_SEG  NODO_DATA
#pragma CONST_SEG DEFAULT


struct Nodo {
	void * dato;
	struct Nodo *next;
};


void initNodo(struct Nodo * nodo,void*_dato);
    
void* getDato(struct Nodo * nodo);
    
void setDato(struct Nodo * nodo,void* _dato);
    
struct Nodo* getNext(struct Nodo * nodoAnt);
    
void setNext(struct Nodo * nodoAnt,struct Nodo * nodo);
    
unsigned char differ(struct Nodo* node1,struct Nodo* node2);
  


#pragma CODE_SEG  DEFAULT
#pragma DATA_SEG  DEFAULT
#pragma CONST_SEG DEFAULT

#endif