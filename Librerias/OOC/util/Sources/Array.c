//  MODULE: Array.c

#include <stdlib.h>
#include <assert.h>
#include "stddef.h"
#include "Array.h"
//#include "Cpu.h"

#pragma DATA_SEG ARRAY_LIST_DATA                                            
#pragma CODE_SEG ARRAY_LIST_CODE 
#pragma CONST_SEG DEFAULT

void Array_defConstructor(void *, va_list *);
void Array_destructor(void *);

const struct Class Array={
  CLASS_INITIALIZATION(Class,
                       Array,
                       Object,
                       Array_defConstructor,
                       Array_destructor,
                       NULL,
                       NULL)
};

//const void *const Array = &ArrayClass; 

/*
** ===================================================================
**     Method      :  Array_Constructor 
**    Description : Constructor
** ===================================================================
*/
void Array_Constructor(void * self,int cantidad){
  struct Array *_a = (struct Array *)self;
  
  _a->cantidad=cantidad;
  _a->nodos=(void**)calloc(cantidad, sizeof(void*));
  _a->objetosSonPropios=FALSE;
}

/*
** ===================================================================
**     Method      :  Array_DefConstructor 
**    Description  :  Constructor por defecto   
** ===================================================================
*/
void Array_defConstructor(void * self, va_list * arg){
  Array_Constructor(self,va_arg(*arg,int));  
}


/*
** ===================================================================
**     Method      :  Array_Destructor 
**    Description  :  Destructor
** ===================================================================
*/
void Array_destructor(void * self){
  struct Array* _a =(struct Array *)self;
  
  if(_a->objetosSonPropios){
    int i;
    
    for(i=0;i<_a->cantidad;i++)
      deleteAndNil(&_a->nodos[i]);  
  }
  free(_a->nodos);
}


/*
** ===================================================================
**     Method      :  Array_get 
**    Description : 
** ===================================================================
*/
void* Array_get(void * self,int index){
  struct Array* _a =(struct Array *)self;
  
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
void Array_set(void * self,int index,void * dato){
  struct Array* _a =(struct Array *)self;
  
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
void Array_ownsObjects(void* self,bool owns){
    struct Array* _a =(struct Array *)self;
    
    _a->objetosSonPropios=owns;  
}

/*
** ===================================================================
**     Method      :  Array_count 
**    Description : Devuelve la cantidad de elementos en el array
** ===================================================================
*/
int Array_count(void * self){
  struct Array* _a =(struct Array *)self;
  
  return _a->cantidad;
}

/*
** ===================================================================
**     Method     :  Array_search 
**    Description :  Busca linealmente coincidencia de un elemento 
**      con el comparador
** ===================================================================
*/
void * Array_search(void * self,void * comparator){
  struct Array* _a =(struct Array *)self;
  int i;
  
  for(i=0;i<_a->cantidad;i++){
   /* if(_Comparator_equals(comparator,_a->nodos[i]))
      return _a->nodos[i];       */
  }
  
  return NULL;
  
}

/*
** ===================================================================
**     Method      :  Array_isEmpty 
**    Description : indica si el array esta vacio
** ===================================================================
*/
bool Array_isEmpty(void * self){
   return Array_count(self)==-1;
}


#pragma CODE_SEG ARRAY_LIST_CODE 
