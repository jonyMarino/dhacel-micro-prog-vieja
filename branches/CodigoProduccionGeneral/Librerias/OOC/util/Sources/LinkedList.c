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


#include <stdarg.h>

#include "stddef.h"
#include "LinkedList.h"

#pragma CODE_SEG  LinkedList_CODE
#pragma DATA_SEG  LinkedList_DATA
#pragma CONST_SEG DEFAULT 

void LinkedList_DefConstruct(void*_self,va_list * args);

uchar LinkedList_Differ(void * _self,struct LinkedList * _l);

bool LinkedList_contains(void * _self,void * dato);

const struct LinkedListClass LinkedList={
    CLASS_INITIALIZATION(LinkedListClass,
                       LinkedList,
                       List,
                       LinkedList_DefConstruct,
                       LinkedList_Destruct,
                       LinkedList_Differ,
                       Object_puto),
                       LinkedList_add,
                       LinkedList_Vaciar,
                       LinkedList_contains,   
                       LinkedList_Vacia,
                       LinkedList_iterator,
                       LinkedList_deleteReference, 
                       LinkedList_Cantidad,
                       LinkedList_get,
                        LinkedList_addFirst,
                        LinkedList_addLast,
                        LinkedList_getFirst,
                        LinkedList_getLast,
                        LinkedList_Eliminar_Primero,
                        LinkedList_removeLast                      
};


void LinkedList_deleteNodo(void *_self,void*tmpNodo,void*tmpNodoAnterior);

/*
** ===================================================================
**     Method     : LinkedList_Construct 
**    Description : Constructor del Objeto
** ===================================================================
*/
void LinkedList_Construct(void*_self){
  struct LinkedList * _l= _self;
  
  _l->nodo=NULL;
}

/*
** ===================================================================
**     Method     : LinkedList_DefConstruct 
**    Description : Constructor por defecto del Objeto
** ===================================================================
*/
void LinkedList_DefConstruct(void*_self,va_list * args){
  LinkedList_Construct(_self);
}

/*
** ===================================================================
**     Method     : LinkedList_Destruct 
**    Description : Destructor por defecto del Objeto
** ===================================================================
*/
void * LinkedList_Destruct(void * _self){
  struct LinkedList * _l= _self;
  
  LinkedList_Vaciar(_l);
  
  return _l;
}

/*
** ===================================================================
**     Method     : LinkedList_Differ 
**    Description : Comparador del Objeto
** ===================================================================
*/
uchar LinkedList_Differ(void * _self,struct LinkedList * _l){
  struct LinkedList * _ls= _self;
  return _ls->nodo==_l->nodo;
}

/*
** ===================================================================
**     Method     : LinkedList_Vacia 
**    Description : indica si la LinkedLista esta vacia
** ===================================================================
*/
uchar LinkedList_Vacia(void * _self)
{ struct LinkedList * _l= _self;
	return (_l->nodo == NULL);
}

/*
** ===================================================================
**     Method     : LinkedList_Vacia 
**    Description : indica si la LinkedLista esta vacia
** ===================================================================
*/
void LinkedList_Vaciar(void * _self){
  struct LinkedList * _l= _self;
  struct Nodo * tmp;
  
  for(tmp= _l->nodo;tmp!=NULL;_l->nodo=tmp){    
    tmp= Nodo_getNext(tmp);
    deleteAndNil(&_l->nodo);
  }
}


/*
** ===================================================================
**     Method     : LinkedList_add 
**    Description : Agrega un nodo al principio
** ===================================================================
*/
t_Lst_Control LinkedList_add(void * _self, void *dato)
{
	struct LinkedList * _l= _self;

	/* creo el nuevo nodo */
	struct Nodo *n= _new(&Nodo,dato,_l->nodo);

	/* Enlazo a la LinkedLista */
  if(!n)
    return LST_ERROR;
  
  _l->nodo=n;
  return LST_OK;
}


/*
** ===================================================================
**     Method     : LinkedList_Aniadir 
**    Description : Agrega un nodo al final
** ===================================================================
*/
t_Lst_Control LinkedList_Aniadir(void *_self, void *dato)
{
	struct LinkedList * _l= _self;
	
	/* creo el nuevo nodo */
	struct Nodo*n=_new(&Nodo,dato,NULL);
  
  if(!n)
    return LST_ERROR;	


	/* Enlazo al LinkedLista */
	if (LinkedList_Vacia(_l) == TRUE)
		_l->nodo = n;
	else {
		/* busco el ultimo nodo */
		struct Nodo*ultimo;
		
		for(ultimo=_l->nodo; Nodo_getNext(ultimo) != NULL; ultimo=Nodo_getNext(ultimo)) {}
		Nodo_setNext(ultimo,n);
	}

	return LST_OK;
}



/*
** ===================================================================
**     Method     : LinkedList_Cantidad 
**    Description : Devuelve el tamanio de la LinkedLista
** ===================================================================
*/
unsigned int LinkedList_Cantidad(void *_self)
{
	struct LinkedList * _l= _self;
	void * tmpNodo;
	unsigned int count=0;

	for(tmpNodo=_l->nodo; tmpNodo != NULL; tmpNodo=Nodo_getNext(tmpNodo)) count++;

	return count;
}

/*
** ===================================================================
**     Method     : LinkedList_Eliminar_Primero 
**    Description : Elimina el primer nodo y devuelve el dato 
** ===================================================================
*/
void * LinkedList_Eliminar_Primero(void *_self)
{
	struct LinkedList * _l= _self;
	void * tmpNodo;
	void *dato;

	if (LinkedList_Vacia(_l) == TRUE) return NULL;

	tmpNodo = _l->nodo;
	_l->nodo = Nodo_getNext(tmpNodo);
	dato = Nodo_getDato(tmpNodo);
	deleteAndNil(&tmpNodo);

	return dato;
}

/*
** ===================================================================
**     Method     : LinkedList_get 
**    Description : Obtener dato
** ===================================================================
*/
void * LinkedList_get(void * _self,uint index){
  struct LinkedList * _l= _self;
	void * tmpNodo;
	unsigned int count=0;
	
  for(tmpNodo=_l->nodo; tmpNodo != NULL; tmpNodo=Nodo_getNext(tmpNodo)){
    if(count==index)
      return Nodo_getDato(tmpNodo);
    count++;
  }
  return NULL;

}

/*
** ===================================================================
**     Method     : LinkedList_deleteReference 
**    Description : destruye la referencia
** ===================================================================
*/
bool LinkedList_deleteReference(void * _self,void * dato){
  struct LinkedList * _l= _self;
	void * tmpNodo;
  void * tmpNodoAnterior=NULL;
  
  for(tmpNodo=_l->nodo; tmpNodo != NULL; tmpNodo=Nodo_getNext(tmpNodo)){
    
    if(Nodo_getDato(tmpNodo)==dato){
      LinkedList_deleteNodo(_self,tmpNodo,tmpNodoAnterior);
      return TRUE;
    }
    
    tmpNodoAnterior=tmpNodo;
      
  }
  return FALSE;
} 

/*
** ===================================================================
**     Method     : LinkedList_deleteNodo 
**    Visivility  : Private
**    Description : elimina el nodo
** ===================================================================
*/
void LinkedList_deleteNodo(void *_self,void*tmpNodo,void*tmpNodoAnterior){
  struct LinkedList * _l= _self;
  
  if(_l->nodo==tmpNodo)
    _l->nodo= Nodo_getNext(tmpNodo);  
  else
    Nodo_setNext(tmpNodoAnterior,Nodo_getNext(tmpNodo));
      
  _delete(tmpNodo);
}



/*
** ===================================================================
**     Method     : LinkedList_contains 
**    Description : destruye la referencia
** ===================================================================
*/
bool LinkedList_contains(void * _self,void * dato){
  struct LinkedList * _l= _self;
	void * tmpNodo;
  
  for(tmpNodo=_l->nodo; tmpNodo != NULL; tmpNodo=Nodo_getNext(tmpNodo)){
    if(Nodo_getDato(tmpNodo)==dato)
      return TRUE;      
  }
  return FALSE;

}

/*
** ===================================================================
**     Method     : LinkedList_linkedListIterator 
**    Description : construye el iterador correspondiente 
** ===================================================================
*/
void LinkedList_linkedListIterator(void * _self,void * it){
  newAlloced(it,&LinkedListIterator,_self);  
}

/*
** ===================================================================
**     Method     : LinkedList_iterator 
**    Description : construye el iterador correspondiente 
** ===================================================================
*/
void * LinkedList_iterator(void * _self){
  return _new(&LinkedListIterator,_self);  
}



/*Iterador*/
void LinkedListIterator_defConstructor(void *_self,va_list * args);

const struct IteratorClass LinkedListIterator={
  ITERATOR_CLASS_INITIALIZATION(Class,
                       LinkedListIterator,
                       Object,
                       LinkedListIterator_defConstructor,
                       NULL,
                       NULL,
                       NULL,
                       LinkedListIterator_next,
                       LinkedListIterator_hasNext
                       )
  
};

//const void * const LinkedListIterator=&TableLinkedListIterator;

#pragma INLINE
void LinkedListIterator_constructor(void *_self,struct LinkedList * list){
  struct LinkedListIterator * _it=_self;
  _it->nodoNext=list->nodo; 
}

void LinkedListIterator_defConstructor(void *_self,va_list * args){
  LinkedListIterator_constructor(_self,va_arg(*args,void*));
}

bool LinkedListIterator_hasNext(void * _self){
  struct LinkedListIterator * _it=_self;
  return  _it->nodoNext!=NULL;
}

void *LinkedListIterator_next(void * _self){
  struct LinkedListIterator * _it=_self;
  if(!_it->nodoNext)
    return NULL; 
  else{      
    void * result= Nodo_getDato(_it->nodoNext);
    _it->nodoNext=Nodo_getNext(_it->nodoNext);
    return result;
  }
  
}

/*
 *	LinkedList management and selectors
 */
void addFirst(void * _self,void * obj){
  struct LinkedListClass * class=classOf(_self);
  class->addFirst(_self,obj);
}

void addLast(void * _self,void * obj){
  struct LinkedListClass * class=classOf(_self);
  class->addLast(_self,obj);
}

struct Object * getFirst(void * _self){
  struct LinkedListClass * class=classOf(_self);
  return class->getFirst(_self);
}

struct Object * getLast(void * _self){
  struct LinkedListClass * class=classOf(_self);
  return class->getLast(_self);  
}

struct Object * removeFirst(void * _self){
  struct LinkedListClass * class=classOf(_self);
  return class->removeFirst(_self);  
}

struct Object * removeLast(void * _self){
  struct LinkedListClass * class=classOf(_self);
  return class->removeLast(_self);
}
