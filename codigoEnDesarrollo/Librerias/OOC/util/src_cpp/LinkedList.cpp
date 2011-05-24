                                     /*MODULE  SimpleLinkedList
**     Filename  : SimpleLinkedList.C
**     Project   : Controlador
**     Processor : MC9S12GC32CFU
**     Version   : 0.0.1
**     Compiler  : Metrowerks HC12 C Compiler
**     Date/Time : 31/03/2008, 08:45
**     Abstract  :
**							LinkedLista simple. 
*/


#include <stdlib.h>

#include "stddef.h"
#include "LinkedList.hpp"

#pragma CODE_SEG  LinkedList_CODE
#pragma DATA_SEG  LinkedList_DATA
#pragma CONST_SEG DEFAULT 



/*
** ===================================================================
**     Method     : LinkedList 
**    Description : Constructor del Objeto
** ===================================================================
*/
LinkedList::LinkedList(void):nodo(NULL){}
/*
** ===================================================================
**     Method     : ~LinkedList 
**    Description : Destructor por defecto del Objeto
** ===================================================================
*/
LinkedList::~LinkedList(void){
  clear();
}

/*
** ===================================================================
**     Method     : LinkedList::Differ 
**    Description : Comparador del Objeto
** ===================================================================
*/
unsigned char LinkedList::differ(LinkedList * _l){
  return (unsigned char)(nodo ==_l->nodo);
}

/*
** ===================================================================
**     Method     : LinkedList::Vacia 
**    Description : indica si la LinkedLista esta vacia
** ===================================================================
*/
unsigned char LinkedList::isEmpty() {
	return (nodo == NULL);
}

/*
** ===================================================================
**     Method     : LinkedList::Vacia 
**    Description : indica si la LinkedLista esta vacia
** ===================================================================
*/
void LinkedList::clear(void){
  Nodo * tmp;
  
  for(tmp= nodo; tmp!=NULL; nodo=tmp){    
    tmp= tmp->getNext();
    delete(nodo);
    nodo = NULL;
  }
}


/*
** ===================================================================
**     Method     : LinkedList::add 
**    Description : Agrega un nodo al principio
** ===================================================================
*/
bool LinkedList::add(void *dato)
{

	/* creo el nuevo nodo */
	Nodo *n = new Nodo(dato,nodo);

	/* Enlazo a la LinkedLista */
  if(!n)
    return EXIT_FAILURE;
  
  nodo=n;
  
  return EXIT_SUCCESS;
}


/*
** ===================================================================
**     Method     : LinkedList::Aniadir 
**    Description : Agrega un nodo al final
** ===================================================================
*/
bool LinkedList::aniadir(void *dato)
{
	
	/* creo el nuevo nodo */
	Nodo*n=new Nodo(dato,NULL);
  
  if(!n)
    return EXIT_FAILURE;	


	/* Enlazo al LinkedLista */
	if ( isEmpty() )
		nodo = n;
	else {
		/* busco el ultimo nodo */
		Nodo*ultimo;
		
		for(ultimo= nodo; ultimo->getNext() != NULL; ultimo = ultimo->getNext() );
		  ultimo->setNext(n);
	}

	return EXIT_SUCCESS;
}



/*
** ===================================================================
**     Method     : LinkedList::Cantidad 
**    Description : Devuelve el tamanio de la LinkedLista
** ===================================================================
*/
unsigned int LinkedList::size(void)
{
	Nodo * tmpNodo;
	unsigned int count=0;

	for(tmpNodo=nodo; tmpNodo != NULL; tmpNodo=tmpNodo->getNext()) 
	  count++;

	return count;
}

/*
** ===================================================================
**     Method     : LinkedList::Eliminar_Primero 
**    Description : Elimina el primer nodo y devuelve el dato 
** ===================================================================
*/
void * LinkedList::eliminarPrimero(void)
{
	Nodo * tmpNodo;
	void *dato;

	if (isEmpty() == TRUE) 
	  return NULL;

	tmpNodo = nodo;
	nodo = tmpNodo->getNext();
	dato = tmpNodo->getDato();
	
	delete(tmpNodo);

	return dato;
}

/*
** ===================================================================
**     Method     : LinkedList::get 
**    Description : Obtener dato
** ===================================================================
*/
void * LinkedList::get(unsigned int index){
	Nodo * tmpNodo;
	unsigned int count=0;
	
  for(tmpNodo= nodo; tmpNodo != NULL; tmpNodo=tmpNodo->getNext()){
    if(count==index)
      return tmpNodo->getDato();
    count++;
  }
  return NULL;

}

/*
** ===================================================================
**     Method     : LinkedList::deleteReference 
**    Description : destruye la referencia
** ===================================================================
*/
bool LinkedList::moveOut(void * dato){
	Nodo * tmpNodo;
  Nodo * tmpNodoAnterior=NULL;
  
  for(tmpNodo=nodo; tmpNodo != NULL; tmpNodo=tmpNodo->getNext()){
    
    if(tmpNodo->getDato()==dato){
      deleteNodo(tmpNodo,tmpNodoAnterior);
      return TRUE;
    }
    
    tmpNodoAnterior=tmpNodo;
      
  }
  return FALSE;
} 

/*
** ===================================================================
**     Method     : LinkedList::deleteNodo 
**    Visivility  : Private
**    Description : elimina el nodo
** ===================================================================
*/
void LinkedList::deleteNodo(Nodo*tmpNodo,Nodo*tmpNodoAnterior){
  
  if(nodo==tmpNodo)
    nodo= tmpNodo->getNext();  
  else
    tmpNodoAnterior->setNext(tmpNodo->getNext());
      
  delete(tmpNodo);
}



/*
** ===================================================================
**     Method     : LinkedList::contains 
**    Description : destruye la referencia
** ===================================================================
*/
bool LinkedList::contains(void * dato){
	Nodo * tmpNodo;
  
  for(tmpNodo= nodo; tmpNodo != NULL; tmpNodo=tmpNodo->getNext()){
    if(tmpNodo->getDato()==dato)
      return TRUE;      
  }
  return FALSE;

}

/*
** ===================================================================
**     Method     : LinkedList::linkedListIterator 
**    Description : construye el iterador correspondiente 
** ===================================================================
*/
void LinkedList::linkedListIterator(LinkedListIterator * it){
  it->nodoNext=nodo;
}

/*
** ===================================================================
**     Method     : LinkedList::iterator 
**    Description : construye el iterador correspondiente 
** ===================================================================
*/
Iterator * LinkedList::iterator(void){
  return NULL;//new LinkedListIterator(this);  
}

/*
** ===================================================================
**     Method     : getPrimerNodo
**    Description : devuelve el puntero al primer nodo
** ===================================================================
*/
Nodo * LinkedList::getPrimerNodo(){
  return nodo;
}

/*
** ===================================================================
**     Method     : setPrimerNodo
**    Description : devuelve el puntero al primer nodo
** ===================================================================
*/
void LinkedList::setPrimerNodo(Nodo * _nodo){
  nodo = _nodo;
}


/*Iterador*/

LinkedList::LinkedListIterator::LinkedListIterator(){
}

LinkedList::LinkedListIterator::LinkedListIterator(LinkedList * list){
  nodoNext=list->nodo; 
}


bool LinkedList::LinkedListIterator::hasNext(void){
  return  nodoNext!=NULL;
}

void * LinkedList::LinkedListIterator::next(void){
  if(!nodoNext)
    return NULL; 
  else{      
    void * result= nodoNext->getDato();
    nodoNext= nodoNext->getNext();
    return result;
  } 
}

