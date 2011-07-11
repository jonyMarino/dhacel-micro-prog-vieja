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
#include "DobleList.h"

 
/*
** ===================================================================
**     Method     : DobleList_DeleteNodo 
**    Description : Elimina el nodo pasado, qeu debe pertenecer a la 
**                lista
** ===================================================================
*/
unsigned char DobleList_deleteNodo(struct NodoDoble * _n){
  struct NodoDoble*next;
  struct NodoDoble * nodo = LinkedList_getPrimerNodo(); 
  
  if(!_n)
    return EXIT_FAILURE;
  
  if(_n == nodo)						// Estoy Eliminando el primer nodo?
    setPrimerNodo( getNext() );
  
  //  Enlazo los nodos  anterior y siguiente
  if(getPrev())
    setNext( getNext());
  next=getNext();
  if(next)
    setPrev( getPrev() );
  
  delete(_n);
  
}

/*
** ===================================================================
**     Method     : DobleList_InsertFirst 
**    Description : Inserta un dato al principio de la Lista
** ===================================================================
*/
struct NodoDoble * DobleList_insertFirst(void * dato){
  struct NodoDoble * _n = malloc( sizeof(struct NodoDoble) );
  _n = (struct NodoDoble*)getPrimerNodo();
  
  if(!_n)
    return NULL;
  
  if( getPrimerNodo() ){
    setPrev(_n);
    setNext((struct NodoDoble*)getPrimerNodo());  
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
void * DobleList_getDato(unsigned int index){
  struct NodoDoble * _n = LinkedList_getPrimerNodo();
  uint i;
  
  
  for(i=0;i<index;i++){
    if(_n)
      _n=getNext();
    else
      break;
  }
  
  if(i==index){
    if(_n){
      return getDato();
      return 0;
    }
    return NULL;
  }
  return NULL;
}


/*
** ===================================================================
**     Method     : DobleList_cantidad 
**    Description : Devuelve el tamanio 
** ===================================================================
*/
unsigned int DobleList_cantidad()
{
	struct NodoDoble * _n = LinkedList_getPrimerNodo();
	void * tmpNodo;
	unsigned int count=0;

	for(tmpNodo=_n; tmpNodo != NULL; tmpNodo=getNext()) count++;

	return count;
}