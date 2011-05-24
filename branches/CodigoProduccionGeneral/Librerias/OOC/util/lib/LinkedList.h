#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include "stddef.h"
#include "nodo.h"
#include "Iterator.h"
#include "List.h"

#pragma CODE_SEG  LinkedList_CODE
#pragma DATA_SEG  LinkedList_DATA


/*C++ 
_class LinkedList:public List
{
 public:
  void addFirst(void * _self,void * obj);
  void addLast(void * _self,void * obj);
  Object * getFirst(void);
  Object * getLast(void);
  Object * removeFirst(void);
  Object * removeLast(void);
 
 private:
    Nodo * nodo;
 
}
*/
struct LinkedList{
  struct List super;
  struct Nodo * nodo;
};


struct LinkedListClass{
  struct ListClass super;
  void(*addFirst)(void * _self,void * obj);
  void(*addLast)(void * _self,void * obj);
  struct Object *(*getFirst)(void * _self);
  struct Object *(*getLast)(void * _self);
  struct Object * (*removeFirst)(void * _self);
  struct Object * (*removeLast)(void * _self);
};

extern const struct LinkedListClass LinkedList;

/* Prototipos de LinkedList */
void addFirst(void * _self,void * obj);
void addLast(void * _self,void * obj);
struct Object * getFirst(void * _self);
struct Object * getLast(void * _self);
struct Object * removeFirst(void * _self);
struct Object * removeLast(void * _self);



/* Prototipos de LinkedList viejos*/
typedef enum {LST_ERROR, LST_OK} t_Lst_Control;
void LinkedList_Construct(void*_self);
void * LinkedList_Destruct(void * _self);
uchar LinkedList_Differ(void * _self,struct LinkedList * _l);
bool LinkedList_contains(void * _self,void * dato);
uchar LinkedList_Vacia(void *_self);
void LinkedList_Vaciar(void * _self);
t_Lst_Control LinkedList_add(void *_self, void *dato);
t_Lst_Control LinkedList_Aniadir(void *_self, void *dato);
void * LinkedList_Eliminar_Primero(void *_self);
unsigned int LinkedList_Cantidad(void *_self);
void LinkedList_linkedListIterator(void * _self,void * it);
#define LinkedList_addFirst NULL
#define LinkedList_addLast  NULL
#define LinkedList_getFirst NULL
#define LinkedList_getLast  NULL
#define LinkedList_removeLast NULL
/*
** ===================================================================
**     Method     : LinkedList_iterator 
**    Description : construye el iterador correspondiente 
** ===================================================================
*/
void * LinkedList_iterator(void * _self);
void * LinkedList_get(void * _self,int index);
bool LinkedList_deleteReference(void * _self,void * dato);

/*Iterador*/
struct LinkedListIterator{
  struct Iterator _base;
  struct Nodo * nodoNext;
};

extern const struct IteratorClass LinkedListIterator;

bool LinkedListIterator_hasNext(void * _self);
void *LinkedListIterator_next(void * _self);

#define HENCE_INITIALIZATION_LINKED_LIST(_class)  \
    _class,								      \
    NULL

#define INICIALITATION_LINKED_LIST() HENCE_INITIALIZATION_LINKED_LIST(&LinkedList)


#pragma CODE_SEG  DEFAULT
#pragma DATA_SEG  DEFAULT

#endif