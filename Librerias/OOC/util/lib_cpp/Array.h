#ifndef _ARRAY_H
#define _ARRAY_H

#include <limits.h>
#include "Object.h"

#include "PE_Types.h"
#include "Comparator.hpp"
#include "Object.h"

#pragma DATA_SEG ARRAY_LIST_DATA                                            
#pragma CODE_SEG ARRAY_LIST_CODE 
#pragma CONST_SEG DEFAULT

#ifdef __cplusplus
extern "C"
{
#endif

struct Array{
  struct Object super;
  void ** nodos;
  int cantidad;
  bool objetosSonPropios;
};

extern const struct Class Array;

/*
** ===================================================================
**     Method      :  Array_Constructor 
**    Description : Constructor
** ===================================================================
*/
//void Array_constructor(struct Array * self);

/*
** ===================================================================
**     Method      :  Array_get 
**    Description : 
** ===================================================================
*/
void* Array_get(void * _self,int index);

/*
** ===================================================================
**     Method      :  Array_set
**    Description  : 
** ===================================================================
*/
void Array_set(void * _self,int index,void * dato);

/*
** ===================================================================
**     Method      :  Array_OwnsObjects 
**    Description :  Define si los objetos solo son apuntados por el
**    array y por ende el mismo puede destruirlos 
**    @param owns TRUE- los objetos son propios y se destruiran al 
**                      destruir el array
**                FALSE-  Los objeton no pertenecen al array
** ===================================================================
*/
void Array_ownsObjects(void * _self,bool owns);

/*
** ===================================================================
**     Method      :  Array_count 
**    Description : Devuelve la cantidad de elementos en el array
** ===================================================================
*/
int Array_count(void * _self);

/*
** ===================================================================
**     Method     :  Array_search 
**    Description :  Busca linealmente coincidencia de un elemento 
**      con el comparador
** ===================================================================
*/
void * Array_search(void * _self,Comparator * comparator);

/*
** ===================================================================
**     Method      :  Array_isEmpty 
**    Description : indica si el array esta vacio
** ===================================================================
*/
bool Array_isEmpty(void * _self);

/*
** ===================================================================
**     Method     :  PContainer_Delete 
**    Description :  Elimina la referencia guardada
** ===================================================================
*/
void *Array_delete(void * _self,void * Obj);


#define INITIALIZATION_ARRAY(CLASS,ARRAY)\
  &CLASS,    \
  (void**)&ARRAY,		\
  sizeof(ARRAY)/sizeof(void*), \
  FALSE				
  
  
#define NEW_ARRAY(name,ARRAY) \
  struct Array name = { \
    INITIALIZATION_ARRAY(Array,ARRAY) \
  }
  
#ifdef __cplusplus
}
#endif  
  
#pragma DATA_SEG DEFAULT                                            
#pragma CODE_SEG DEFAULT 



#endif