#ifndef _METHOD_CONTAINER_H
#define _METHOD_CONTAINER_H

#include "Object.h"
#include "stddef.h"
#include "LinkedList.hpp"
#include "Method.hpp"

struct MethodContainer{
  void * classOf;
  struct LinkedList list;   
};

extern const struct Class MethodContainer;
//
#define MethodContainerIterator	 LinkedListIterator
#define MethodContainerIterator_hasNext	 LinkedListIterator_hasNext
#define MethodContainerIterator_next  LinkedListIterator_next

/*
** ===================================================================
**     Method     :  MethodContainer_Constructor 
**    Description : Constructor del objeto 
** ===================================================================
*/
//newObj(MethodContainer)
/*
** ===================================================================
**     Method     :  MethodContainer_Destructor 
**    Description : Destructor del objeto 
** ===================================================================
*/
void *MethodContainer_Destructor(void*);

/*
** ===================================================================
**     Method     :  MethodContainer_Add 
**    Description : Agrega una funcion al contenedor 
** ===================================================================
*/
void MethodContainer_add(void * _self,struct Method *method);

/*
** ===================================================================
**     Method     :  MethodContainer_execute 
**    Description : Ejecuta las funciones agergadas al contenedor
** ===================================================================
*/
void MethodContainer_execute(void*);
/*
** ===================================================================
**     Method     :  MethodContainer_iterator 
**    Description : devuelve el iterador del method container
** ===================================================================
*/
void MethodContainer_iterator(void*_self,void * it);

/*
** ===================================================================
**     Method     :  _MethodContainer_TypedExecute 
**    Description : Ejecuta las funciones casteadas al tipo que se desea
** ===================================================================
*/
/*#define _MethodContainer_TypedExecute(self,cast,args)          \
  if(self){																										 \
    size_t size=  MethodContainer_getSize(self);							 \
    size_t i;																									 \
    																													 \
    for(i=0;i<size;i++){																			 \
      const TMethodObj* mo=MethodContainer_getMethod(self,i);  \
																															 \
      (cast)mo->pmethod)args;															     \
    }																													 \
  																														 \
  }                                                                                 
  
  */
/*
** ===================================================================
**     Method     :  MethodContainer_isEmpty 
**    Description : Se fija si el contenedor esta vacio
** ===================================================================
*/
uchar MethodContainer_isEmpty(void*_self);
/*
** ===================================================================
**     Method     :  MethodContainer_getSize 
**    Description :  Retorna la cantidad de metodos guardados
** ===================================================================
*/
uint MethodContainer_getSize(void*_self);
/*
** ===================================================================
**     Method     :  MethodContainer_getMethod 
**    Description :  Retorna el metodo(funcion+Obj) 
**                  guardado en la posicion i
** ===================================================================
*/
const struct Method* MethodContainer_getMethod(void*_self,uint i);
/*
** ===================================================================
**     Method     :  MethodContainer_DeleteMethod 
**    Description :  Elimina el metodo
** ===================================================================
*/
void MethodContainer_delete(void * _self,struct Method *method);

//inicializador
#define INITIALITATION_METHOD_CONTAINER(class)  \
  class,																				\
  INICIALITATION_LINKED_LIST()
//instanciador
#define NEW_METHOD_CONTAINER(name)  \
  struct MethodContainer name={			\
    INITIALITATION_METHOD_CONTAINER(&MethodContainer)	 \
  }
#endif