/*MODULE  DobleList
**     Filename  : DobleList.C
**     Project   : Controlador
**     Processor : MC9S12GC32CFU
**     Version   : 0.0.1
**     Compiler  : Metrowerks HC12 C Compiler
**     Date/Time : 23/01/2008, 14:20
**     Abstract  :
**							Listas doblemente enlazada
*/

#include <stdarg.h>

#include "stddef.h"
#include "DobleList.h"


void DobleList_DefConstruct(void*_self,va_list * args);
void * DobleList_Destruct(void * _self);
void * DobleList_Differ(void * _self,struct DobleList * _l);

const struct Class DobleList={
  CLASS_INITIALIZATION(Class,
                       DobleList,
                       Object,
                       DobleList_DefConstruct,
                       DobleList_Destruct,
                       0,
                       0)
};
 
/*
** ===================================================================
**     Method     : DobleList_Construct 
**    Description : Constructor del Objeto
** ===================================================================
*/
void DobleList_Construct(void*_self){
  struct DobleList * _l= _self;
  
  _l->_Nodo=NULL;
}

/*
** ===================================================================
**     Method     : DobleList_DefConstruct 
**    Description : Constructor por defecto del Objeto
** ===================================================================
*/
void DobleList_DefConstruct(void*_self,va_list * args){
  DobleList_Construct(_self);
}

/*
** ===================================================================
**     Method     : DobleList_Destruct 
**    Description : Destructor por defecto del Objeto
** ===================================================================
*/
void * DobleList_Destruct(void * _self){
  struct DobleList * _l= _self;
  return _self;
}

/*
** ===================================================================
**     Method     : DobleList_Differ 
**    Description : Comparador del Objeto
** ===================================================================
*/
void * DobleList_Differ(void * _self,struct DobleList * _l){
  struct DobleList * _ls= _self;
  
}

/*
** ===================================================================
**     Method     : DobleList_DeleteNodo 
**    Description : Elimina el nodo pasado, qeu debe pertenecer a la 
**                lista
** ===================================================================
*/
uchar DobleList_DeleteNodo(void * _self,struct NodoDbl * _n){
  struct DobleList * _l= _self;
  struct NodoDbl*next;
  
  if(!_n)
    return 1;
  
  if(_n == _l->_Nodo)						// Estoy Eliminando el primer nodo?
    _l->_Nodo= ((struct Nodo*)_n)->next;
  
  //  Enlazo los nodos  anterior y siguiente
  if(_n->prev)
    ((struct Nodo*)_n->prev)->next = ((struct Nodo*)_n)->next;
  next=((struct Nodo*)_n)->next;
  if(next)
    next->prev=_n->prev;
  
  deleteAndNil(&_n);
  
}

/*
** ===================================================================
**     Method     : DobleList_InsertFirst 
**    Description : Inserta un dato al principio de la Lista
** ===================================================================
*/
struct NodoDbl * DobleList_InsertFirst(void * _self,void * dato){
  struct DobleList * _l= _self;
  struct NodoDbl * _n = _new(&NodoDbl,dato,_l->_Nodo,NULL);
  
  if(!_n)
    return NULL;
  
  if(_l->_Nodo){
    _l->_Nodo->prev=_n;
    ((struct Nodo*)_n)->next=_l->_Nodo;  
  }
  
  _l->_Nodo=_n;
  
  return _n;
}

/*
** ===================================================================
**     Method     : DobleList_getDato
**    Description : Obtiene el dato a partir del numero de nodo(0-size)
** ===================================================================
*/
void * DobleList_getDato(void * _self,uint nNodo){
  struct DobleList * _l= _self;
  struct NodoDbl * _n = _l->_Nodo;
  uint i;
  
  
  for(i=0;i<nNodo;i++){
    if(_n)
      _n=((struct Nodo*)_n)->next;
    else
      break;
  }
  
  if(i==nNodo){
    if(_n){
      return ((struct Nodo*)_n)->dato;
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
unsigned int DobleList_cantidad(void *_self)
{
	struct DobleList * _l= _self;
	void * tmpNodo;
	unsigned int count=0;

	for(tmpNodo=_l->_Nodo; tmpNodo != NULL; tmpNodo=Nodo_getNext(tmpNodo)) count++;

	return count;
}


