#ifndef _DOBLE_LIST_H
#define _DOBLE_LIST_H

#include "NodoDoble.h"
#include "LinkedList.h"

struct DobleList{
  const void *classOf;
  struct NodoDbl * _Nodo;
};

//extern const void * const DobleList;
extern const struct Class DobleList;
/*
** ===================================================================
**     Method     : DobleList_Construct 
**    Description : Constructor por defecto del Objeto
** ===================================================================
*/
//struct DobleList * new(&DobleList);
// newAlloced(struct DobleList *,DobleList);
/*
** ===================================================================
**     Method     : DobleList_Destruct 
**    Description : Destructor del Objeto
** ===================================================================
*/
//DeleteObj(void * _self);
//deleteAlloced(void * _self);
/*
** ===================================================================
**    Method      : DobleList_Destruct 
**    Type        : Virtual
**    Description : retorna !0 si el objeto es distinto al objeto 
**                  pasado
** ===================================================================
*/
void * DobleList_Differ(void * _self,struct DobleList * _l);
/*
** ===================================================================
**     Method     : DobleList_DeleteNodo 
**    Description : Elimina el nodo pasado, qeu debe pertenecer a la 
**                lista
** ===================================================================
*/
uchar DobleList_DeleteNodo(void * _self,struct NodoDbl * _n);

/*
** ===================================================================
**     Method     : DobleList_InsertFirst 
**    Description : Inserta un dato al principio de la Lista
** ===================================================================
*/
struct NodoDbl * DobleList_InsertFirst(void * _self,void * dato);

/*
** ===================================================================
**     Method     : DobleList_getDato
**    Description : Obtiene el dato a partir del numero de nodo(0-size)
** ===================================================================
*/
void * DobleList_getDato(void * _self,uint nNodo); 

/*
** ===================================================================
**     Method     : DobleList_cantidad 
**    Description : Devuelve el tamanio 
** ===================================================================
*/
unsigned int DobleList_cantidad(void *_self);

#endif