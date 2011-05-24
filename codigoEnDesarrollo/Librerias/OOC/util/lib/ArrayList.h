#ifndef _ARRAY_LIST_H
#define _ARRAY_LIST_H

#include <limits.h>

#include "Object.h"
#include "PE_Types.h"
#include "Comparator.h"
#include "List.h"

#pragma DATA_SEG ARRAY_LIST_DATA                                            
#pragma CODE_SEG ARRAY_LIST_CODE 
#pragma CONST_SEG DEFAULT

struct ArrayList{
  struct Object super;
  void ** nodos;
  int cuenta;
  int memoria;  //aca se guarda la memoria pedida  
  bool objetosSonPropios;
};
extern const struct ListClass ArrayList;
//extern const void *const ArrayList;

#define LIST_LAST_POSITION INT_MAX
/*
** ===================================================================
**     Method      :  ArrayList_Constructor 
**    Description : Constructor
** ===================================================================
*/
void ArrayList_Constructor(void * self);

/*
** ===================================================================
**     Method      :  ArrayList_Add 
**     Description : Agerga un nuevo valor en el Array en la ultima
**     posicion del array
** ===================================================================
*/
void ArrayList_Add(void * self, void* Obj);

/*
** ===================================================================
**     Method      :  ArrayList_Add 
**     Description : Agerga un nuevo valor en el Array en la 
**     posicion indicada del array. en caso de que la posicion supere
**     en mas de uno la posicion final, el objeto no se insertara
** ===================================================================
*/
uchar ArrayList_Put(void * self, void* Obj, int index);


/*
** ===================================================================
**     Method      :  ArrayList_AsArray 
**    Description : Devuelve el array con los valores
** ===================================================================
*/
void* ArrayList_get(void * self,int index);

/*
** ===================================================================
**     Method      :  ArrayList_Expand 
**    Description : Aumenta la memoria ocupada por el array
** ===================================================================
*/
void ArrayList_Expand(void * self);
/*
** ===================================================================
**     Method      :  ArrayList_expandInit 
**    Description : Inicia el array con una can
** ===================================================================
*/
void ArrayList_expandInit(void * self,int cantidad);
/*
** ===================================================================
**     Method      :  ArrayList_OwnsObjects 
**    Description :  Define si los objetos solo son apuntados por el
**    array y por ende el mismo puede destruirlos 
**    @param owns TRUE- los objetos son propios y se destruiran al 
**                      destruir el array
**                FALSE-  Los objeton no pertenecen al array
** ===================================================================
*/
void ArrayList_OwnsObjects(void* _self,bool owns);

/*
** ===================================================================
**     Method      :  ArrayList_count 
**    Description : Devuelve la cantidad de elementos en el array
** ===================================================================
*/
int ArrayList_count(void * self);

/*
** ===================================================================
**     Method     :  ArrayList_search 
**    Description :  Busca linealmente coincidencia de un elemento 
**      con el comparador
** ===================================================================
*/
void * ArrayList_search(void * self,void * comparator);

/*
** ===================================================================
**     Method      :  ArrayList_isEmpty 
**    Description : indica si el array esta vacio
** ===================================================================
*/
bool ArrayList_isEmpty(void * self);

/*
** ===================================================================
**     Method     :  PContainer_Delete 
**    Description :  Elimina la referencia guardada
** ===================================================================
*/
bool ArrayList_delete(void*_self,void * Obj);


#define INITIALIZATION_ARRAY_LIST(CLASS,ARRAY)\
  &CLASS,    \
  &ARRAY,		\
  sizeof(ARRAY)/sizeof(void*), \
  sizeof(ARRAY)/sizeof(void*), \
  FALSE				
  
  
#define NEW_ARRAY_LIST(name,ARRAY) \
  struct ArrayList name = { \
    INITIALIZATION_ARRAY_LIST(ArrayList,ARRAY) \
  }
  
#pragma DATA_SEG DEFAULT                                            
#pragma CODE_SEG DEFAULT 



#endif