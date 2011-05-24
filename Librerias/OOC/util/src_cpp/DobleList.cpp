/*MODULE  DobleList
**     Filename  : DobleList.CPP
**     Processor : MC9S12
**     Version   : 0.0.1
**     Compiler  : Metrowerks HC12 CPP Compiler
**     Date/Time : 20/07/2009, 14:20
**     Abstract  :
**							Lista doblemente enlazada
*/

#include <stdlib.h>
#include "stddef.h"
#include "DobleList.hpp"

 
/*
** ===================================================================
**     Method     : DobleList_Construct 
**    Description : Constructor del Objeto
** ===================================================================
*/
DobleList::DobleList(void){}

/*
** ===================================================================
**     Method     : DobleList_DeleteNodo 
**    Description : Elimina el nodo pasado, qeu debe pertenecer a la 
**                lista
** ===================================================================
*/
unsigned char DobleList::deleteNodo(NodoDoble * _n){
  NodoDoble*next;
  NodoDoble * nodo = getPrimerNodo(); 
  
  if(!_n)
    return EXIT_FAILURE;
  
  if(_n == nodo)						// Estoy Eliminando el primer nodo?
    setPrimerNodo( nodo->getNext() );
  
  //  Enlazo los nodos  anterior y siguiente
  if(nodo->getPrev())
    nodo->getPrev()->setNext( _n->getNext());
  next=_n->getNext();
  if(next)
    next->setPrev( _n->getPrev() );
  
  delete(_n);
  
}

/*
** ===================================================================
**     Method     : DobleList_InsertFirst 
**    Description : Inserta un dato al principio de la Lista
** ===================================================================
*/
NodoDoble * DobleList::insertFirst(void * dato){
  NodoDoble * _n = new NodoDoble(dato,(NodoDoble*)getPrimerNodo(),NULL);
  
  if(!_n)
    return NULL;
  
  if( getPrimerNodo() ){
    ( (NodoDoble*)getPrimerNodo() ) ->setPrev(_n);
    _n->setNext(getPrimerNodo());  
  }
  
  setPrimerNodo(_n);
  
  return _n;
}

/*
** ===================================================================
**     Method     : DobleList_getDato
**    Description : Obtiene el dato a partir del numero de nodo(0-size)
** ===================================================================
*/
void * DobleList::getDato(unsigned int index){
  NodoDoble * _n = getPrimerNodo();
  uint i;
  
  
  for(i=0;i<index;i++){
    if(_n)
      _n=((Nodo*)_n)->getNext();
    else
      break;
  }
  
  if(i==index){
    if(_n){
      return ((Nodo*)_n)->getDato();
      return 0;
    }
    return NULL;
  }
  return NULL;
}


