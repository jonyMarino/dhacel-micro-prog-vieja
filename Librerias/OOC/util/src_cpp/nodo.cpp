/*MODULE  nodo
**     Filename  : nodo.CPP
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
#include "nodo.hpp"

#pragma CODE_SEG  Nodo_CODE
#pragma DATA_SEG  Nodo_DATA
#pragma CONST_SEG DEFAULT

/*
** ===================================================================
**     Method     : Nodo
**    Description : Constructor del Objeto
** ===================================================================
*/
Nodo::Nodo(void*_dato,Nodo* _next):dato(_dato),next(_next){}



/*
** ===================================================================
**     Method     : Nodo::setDato 
**    Description : setea el dato
** ===================================================================
*/
void  Nodo::setDato(void* _dato){  
  dato=_dato;
}

/*
** ===================================================================
**     Method     : Nodo::setNext 
**    Description : Setea el puntero al siguiente nodo
** ===================================================================
*/
void Nodo::setNext(Nodo * nodo){
  next=nodo;
}

/*
** ===================================================================
**     Method     : Nodo::differ 
**    Description : Compara los datos de los 2 nodos
** ===================================================================
*/
unsigned char Nodo::differ(Nodo* node){
  
  return (unsigned char)(dato!=node->dato);
}



