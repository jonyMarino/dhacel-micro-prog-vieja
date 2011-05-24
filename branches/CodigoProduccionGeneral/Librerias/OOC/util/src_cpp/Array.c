//  MODULE: Array.c

#include <stdlib.h>
#include <assert.h>
#include "stddef.h"
#include "Array.h"

//#include "Cpu.h"

#pragma DATA_SEG ARRAY_LIST_DATA                                            
#pragma CODE_SEG ARRAY_LIST_CODE 
#pragma CONST_SEG DEFAULT

void Array_Constructor(void * _self,va_list * args);
void Array_destructor(void * _self);

const struct Class Array={
  CLASS_INITIALIZATION(Class,Array,Object,Array_Constructor,Array_destructor,Object_differ,Object_puto)  
};

/*
** ===================================================================
**     Method      :  Array_Constructor 
**    Description : Constructor
** ===================================================================
*/
void Array_Constructor(void * _self,va_list * args){
  struct Array *_a =(struct Array *)_self;
  int cantidad = va_arg(*args,int);
  _a->cantidad=cantidad;
  _a->nodos=(void**)calloc(cantidad, sizeof(void*));
  _a->objetosSonPropios=FALSE;
}



/*
** ===================================================================
**     Method      :  Array_Destructor 
**    Description  :  Destructor
** ===================================================================
*/
void Array_destructor(void * _self){
  struct Array* _a =(struct Array *)_self;
  
  if(_a->objetosSonPropios){
    int i;
    
    for(i=0;i<_a->cantidad;i++)
      delete(&_a->nodos[i]);  
  }
  free(_a->nodos);
}


/*
** ===================================================================
**     Method      :  Array_get 
**    Description : 
** ===================================================================
*/
void* Array_get(void * _self,int index){
  struct Array* _a =(struct Array *)_self;
  
  if(index>_a->cantidad)
    return NULL;
  return _a->nodos[index];
}

/*
** ===================================================================
**     Method      :  Array_set
**    Description  : 
** ===================================================================
*/
void Array_set(void * _self,int index,void * dato){
  struct Array* _a =(struct Array *)_self;
  
  if(index>_a->cantidad)
    return;  //cambiar : error
  _a->nodos[index]=dato;
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
void Array_ownsObjects(void * _self,bool owns){
    struct Array* _a =(struct Array *)_self;
    
    _a->objetosSonPropios=owns;  
}

/*
** ===================================================================
**     Method      :  Array_count 
**    Description : Devuelve la cantidad de elementos en el array
** ===================================================================
*/
int Array_count(void * _self){
  struct Array* _a =(struct Array *)_self;
  
  return _a->cantidad;
}

/*
** ===================================================================
**     Method     :  Array_search 
**    Description :  Busca linealmente coincidencia de un elemento 
**      con el comparador
** ===================================================================
*/
void * Array_search( void * _self,Comparator * comparator){
  struct Array* _a =(struct Array *)_self;
  int i;
  
  for(i=0;i<_a->cantidad;i++){
    if(comparator->equals(_a->nodos[i]))
      return _a->nodos[i];
  }
  
  return NULL;
  
}

/*
** ===================================================================
**     Method      :  Array_isEmpty 
**    Description : indica si el array esta vacio
** ===================================================================
*/
bool Array_isEmpty(void * _self){
    struct Array* self =(struct Array *)_self;
   return Array_count(self)==-1;
}


#pragma CODE_SEG ARRAY_LIST_CODE 
