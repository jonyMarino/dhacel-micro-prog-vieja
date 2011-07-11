//  MODULE: Array.c

#include <stdlib.h>
#include <assert.h>
#include "stddef.h"
#include "Array.h"
#include "Object.h"

#pragma DATA_SEG ARRAY_LIST_DATA                                            
#pragma CODE_SEG ARRAY_LIST_CODE 
#pragma CONST_SEG DEFAULT

Array* array;

/*
** ===================================================================
**     Method      :  Array_Constructor 
**    Description : Constructor
** ===================================================================
*/
void initArray(int cantidad){
 
  array->cantidad=cantidad;
  array->nodos=(void**)calloc(cantidad, sizeof(void*));
  array->objetosSonPropios=FALSE;
}


/*
** ===================================================================
**     Method      :  Array_Destructor 
**    Description  :  Destructor
** ===================================================================
*/
void Array_delete(){
  
  
  if(array->objetosSonPropios){
    int i;
    
    for(i=0;i<array->cantidad;i++)
      deleteAndNil(&array->nodos[i]);  
  }
  free(array->nodos);
}


/*
** ===================================================================
**     Method      :  Array_get 
**    Description : 
** ===================================================================
*/
void* Array_get(int index){
  
  if(index>array->cantidad)
    return NULL;
  return array->nodos[index];
}

/*
** ===================================================================
**     Method      :  Array_set
**    Description  : 
** ===================================================================
*/
void Array_set(int index,void * dato){
 
  if(index>array->cantidad)
    return;  //cambiar : error
   
   array->nodos[index]=dato;
}

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
void Array_ownsObjects(bool owns){
    
    array->objetosSonPropios=owns;  
}

/*
** ===================================================================
**     Method      :  Array_count 
**    Description : Devuelve la cantidad de elementos en el array
** ===================================================================
*/
int Array_count(){
  
  return array->cantidad;
}


/*
** ===================================================================
**     Method      :  Array_isEmpty 
**    Description : indica si el array esta vacio
** ===================================================================
*/
bool Array_isEmpty(){
   return Array_count()==-1;
}


#pragma CODE_SEG ARRAY_LIST_CODE 