/*MODULE  nodo
**     Filename  : nodo.c
**     Processor : MC9S12
**     Version   : 0.0.1
**     Compiler  : Metrowerks HC12 CPP Compiler
**     Date/Time : 20/07/2009, 15:11
**     Abstract  :
**							Nodo utilizable para distintas
**      estructuras de dato(Listas, arboles, etc.) 
*/

#include <stdarg.h>
#include "stddef.h"
#include "nodo.h"

#pragma CODE_SEG  Nodo_CODE
#pragma DATA_SEG  Nodo_DATA
#pragma CONST_SEG DEFAULT


/*
** ===================================================================
**     Method     : Nodo
**    Description : Constructor del Objeto
** ===================================================================
*/
void initNodo(struct Nodo * _nodo,void*_dato){
    _nodo->dato=_dato; 
    _nodo->next=NULL; 
}

/*
** ===================================================================
**     Method     : Nodo::setDato 
**    Description : setea el dato
** ===================================================================
*/
void setDato(struct Nodo * nodo,void* _dato){  
  nodo->dato=_dato; 
}


void* getDato(struct Nodo * nodo){
  return nodo->dato;
}

/*
** ===================================================================
**     Method     : Nodo::setNext 
**    Description : Setea el puntero al siguiente nodo
** ===================================================================
*/
void setNext(struct Nodo * nodoAnt,struct Nodo * _nodo){
  nodoAnt->next=_nodo;
 // _nodo->next=NULL;
}


struct Nodo* getNext (struct Nodo * nodoAnt){ 
  return nodoAnt->next;

}

/*
** ===================================================================
**     Method     : Nodo::differ 
**    Description : Compara los datos de los 2 nodos
** ===================================================================
*/
unsigned char differ(struct Nodo* node1,struct Nodo* node2){
  
  return (unsigned char)(node1->dato!=node2->dato);
}



