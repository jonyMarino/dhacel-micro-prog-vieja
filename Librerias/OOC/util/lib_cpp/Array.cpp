  //  MODULE: Array.cpp

#include <stdlib.h>
#include <assert.h>
#include "stddef.h"
#include "Array.hpp"

//#include "Cpu.h"

#pragma DATA_SEG ARRAY_LIST_DATA                                            
#pragma CODE_SEG ARRAY_LIST_CODE 
#pragma CONST_SEG DEFAULT
/*
void Array_Constructor(void * _self,va_list * args);
void Array_destructor(void * _self);

const struct Class Array={
  CLASS_INITIALIZATION(Class,Array,Object,Array_Constructor,Array_destructor,Object_differ,Object_puto)  
};
 */
/*
** ===================================================================
**     Method      :  Array_Constructor 
**    Description : Constructor
** ===================================================================
*/
Array::Array(va_list * args){
  int _cantidad = va_arg(*args,int);
  cantidad=_cantidad;
  nodos=(void**)calloc(_cantidad, sizeof(void*));
  objetosSonPropios=FALSE;
}



/*
** ===================================================================
**     Method      :  Array_Destructor 
**    Description  :  Destructor
** ===================================================================
*/
Array::~Array(){
  
  if(objetosSonPropios){
    int i;
    
    for(i=0;i<cantidad;i++)
      delete(&nodos[i]);  
  }
  free(nodos);
}


/*
** ===================================================================
**     Method      :  Array_get 
**    Description : 
** ===================================================================
*/
void* Array::get(int index){
  
  if(index>cantidad)
    return NULL;
  return nodos[index];
}

/*
** ===================================================================
**     Method      :  Array_set
**    Description  : 
** ===================================================================
*/
void Array::set(int index,void * dato){
  
  if(index>cantidad)
    return;  //cambiar : error
   nodos[index]=dato;
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
void Array::ownsObjects(bool owns){
    
    objetosSonPropios=owns;  
}

/*
** ===================================================================
**     Method      :  Array_count 
**    Description : Devuelve la cantidad de elementos en el array
** ===================================================================
*/
int Array::count(){
  
  return cantidad;
}

/*
** ===================================================================
**     Method     :  Array_search 
**    Description :  Busca linealmente coincidencia de un elemento 
**      con el comparador
** ===================================================================
*/
void * Array::search(Comparator * comparator){
  
  int i;
  
  for(i=0;i<cantidad;i++){
    if(comparator->equals(nodos[i]))
      return nodos[i];
  }
  
  return NULL;
  
}

/*
** ===================================================================
**     Method      :  Array_isEmpty 
**    Description : indica si el array esta vacio
** ===================================================================
*/
bool Array::isEmpty(){
   
   return count()==-1;
}


#pragma CODE_SEG ARRAY_LIST_CODE 