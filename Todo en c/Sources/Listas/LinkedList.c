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
#include "nodo.h"
#include "LinkedList.h"

#pragma CODE_SEG  LinkedList_CODE
#pragma DATA_SEG  LinkedList_DATA
#pragma CONST_SEG DEFAULT 

struct Nodo * nodo[CANT_MAX_LISTAS];      //primer Nodo de la lista,puedo seleccionar entre "n" listas

/*
** ===================================================================
**     Method     : LinkedListInit 
**    Description : inicia la lista
** ===================================================================
*/
void LinkedListInit(unsigned char NroList){
      nodo[NroList]=NULL;
 }
/*
** ===================================================================
**     Method     : deleteLinkedList 
**    Description : limpia la lista
** ===================================================================
*/
void deleteLinkedList(unsigned char NroList){
  LinkedList_clear(NroList);
}


/*
** ===================================================================
**     Method     : LinkedList::Vacia 
**    Description : indica si la LinkedLista esta vacia
** ===================================================================
*/
unsigned char LinkedList_isEmpty(unsigned char NroList) {
	return (nodo[NroList] == NULL);
}

/*
** ===================================================================
**     Method     : LinkedList_clear
**    Description : limpiar
** ===================================================================
*/
void LinkedList_clear(unsigned char NroList){
  struct Nodo * tmp;
  
  for(tmp= nodo[NroList]; tmp!=NULL; nodo[NroList]=tmp){    
    tmp = getNext(tmp);
    free(nodo[NroList]);
    nodo[NroList] = NULL;
  }
}


/*
** ===================================================================
**     Method     : LinkedList::add 
**    Description : Agrega un nodo al principio
** ===================================================================
*/
bool LinkedList_add(void *dato,unsigned char NroList)
{

	/* creo el nuevo nodo */
  struct Nodo *n = malloc(sizeof(struct Nodo));
	
	initNodo(n,dato);

	/* Enlazo a la LinkedLista */
  if(!n)
    return EXIT_FAILURE;
  
  nodo[NroList]=n;
  
  return EXIT_SUCCESS;
}


/*
** ===================================================================
**     Method     : LinkedList::Aniadir 
**    Description : Agrega un nodo al final
** ===================================================================
*/
bool LinkedList_aniadirFinal(void *dato,unsigned char NroList)
{
	
	/* creo el nuevo nodo */
	struct Nodo*n=malloc(sizeof(struct Nodo));
	
  if(!n) /*tengo memoria disponible??*/
    return EXIT_FAILURE;	
  
    initNodo(n,dato);
    
	/* Enlazo al LinkedLista */
	if ( LinkedList_isEmpty(NroList) ) 
		nodo[NroList] = n;
	
	else {
		/* busco el ultimo nodo */
	  struct Nodo*ultimo;
		
		for(ultimo=nodo[NroList]; getNext(ultimo) != NULL; ultimo = getNext(ultimo));
		
		setNext(ultimo,n);
	}

	return EXIT_SUCCESS;
}



/*
** ===================================================================
**     Method     : LinkedList Cantidad 
**    Description : Devuelve el tamaño de la Lista
** ===================================================================
*/
unsigned int LinkedList_size(unsigned char NroList)
{
	struct Nodo * tmpNodo;
	unsigned int count=0;

	for(tmpNodo=nodo[NroList]; tmpNodo != NULL; tmpNodo=getNext(tmpNodo)) 
	  count++;

	return count;
}

/*
** ===================================================================
**     Method     : LinkedList::Eliminar_Primero 
**    Description : Elimina el primer nodo y devuelve el dato 
** ===================================================================
*/
void * LinkedList_eliminarPrimero(unsigned char NroList)
{
	struct Nodo * tmpNodo;
	void *dato;

	if (isEmpty(NroList) == TRUE) 
	  return NULL;

	tmpNodo = nodo;
	nodo[NroList] = getNext(tmpNodo);
	dato = getDato(tmpNodo);
	
	free(tmpNodo);

	return dato;
}

/*
** ===================================================================
**     Method     : LinkedList_get 
**    Description : Obtener dato, 
**   index coresponde a el numero de objeto en la lista
** ===================================================================
*/
void * LinkedList_get(unsigned int index,unsigned char NroList){
  struct Nodo * tmpNodo;
	unsigned int count=0;
	
  for(tmpNodo= nodo[NroList]; tmpNodo != NULL; tmpNodo=getNext(tmpNodo)){
    if(count==index)
      return getDato(tmpNodo);
    count++;
  }
  return NULL;

}

/*
** ===================================================================
**     Method     : LinkedList deleteReference 
**    Description : destruye la referencia
**     Extrae un objeto intermedio y conecta la lista con el sig.
** ===================================================================
*/
bool LinkedList_moveOut(void * dato,unsigned char NroList){
	struct Nodo * tmpNodo;
  struct Nodo * tmpNodoAnterior=NULL;
  
  for(tmpNodo=nodo[NroList]; tmpNodo != NULL; tmpNodo=getNext(tmpNodo)){
    
    if(getDato(tmpNodo)==dato){
      LinkedList_deleteNodo(tmpNodo,tmpNodoAnterior,NroList);
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
void LinkedList_deleteNodo(struct Nodo*tmpNodo,struct Nodo*tmpNodoAnterior,unsigned char NroList){
  	struct Nodo * NodoAntTmp;
  	
  if(nodo[NroList]==tmpNodo)       //es el primero??
    nodo[NroList] = getNext(tmpNodo);  
  else{
    for(NodoAntTmp= nodo[NroList]; (NodoAntTmp != NULL) && (NodoAntTmp->next != tmpNodo); NodoAntTmp=getNext(NodoAntTmp));
    setNext(NodoAntTmp,getNext(tmpNodo));
  }
  
  free(tmpNodo);
}



/*
** ===================================================================
**     Method     : LinkedList_contains 
**    Description : informa si el dato esta en la lista
** ===================================================================
*/
bool LinkedList_contains(void * dato,unsigned char NroList){
	struct Nodo * tmpNodo;
  
  for(tmpNodo= nodo[NroList]; tmpNodo != NULL; tmpNodo=getNext(tmpNodo)){
    if(getDato(tmpNodo)==dato)
      return TRUE; //esta en la lista     
  }
  return FALSE; // no lo encontro

}

/*
** ===================================================================
**     Method     : getPrimerNodo
**    Description : devuelve el puntero al primer nodo
** ===================================================================
*/
struct Nodo * LinkedList_getPrimerNodo(unsigned char NroList){
  return nodo[NroList];
}

/*
** ===================================================================
**     Method     : setPrimerNodo
**    Description : devuelve el puntero al primer nodo
** ===================================================================
*/
void LinkedList_setPrimerNodo(struct Nodo * _nodo,unsigned char NroList){
  nodo[NroList] = _nodo;
}

/*
** ===================================================================
**     Method     : hasNext
**    Description : sirve para saber si hay un proximo objeto
** ===================================================================
*/

bool hasNext(struct Nodo * _nodo){
  return  _nodo->next!=NULL;
}

/*
** ===================================================================
**     Method     : next
**    Description : sirve para consegir el dato
** ===================================================================
*/


void * getDataObj(struct Nodo * _nodo){
  if(!_nodo)
    return NULL; 
  else{      
    void * result= getDato(_nodo);
    return result;
  } 
}
