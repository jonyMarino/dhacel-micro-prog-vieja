//  MODULE: ArrayList.c

#include <stdlib.h>
#include <assert.h>
#include "stddef.h"
#include "ArrayList.h"
#include "Iterator.h"

#pragma DATA_SEG ARRAY_LIST_DATA                                            
#pragma CODE_SEG ARRAY_LIST_CODE 
#pragma CONST_SEG DEFAULT

void ArrayList_DefConstructor(void *, va_list *);
void *ArrayList_Destructor(void *);
void ArrayList_Expand(void * self);
void * ArrayList_iterator(void * _self);
bool ArrayList_contains(void * _self,void * dato);
void ArrayList_vaciar(void * _self);

const struct ListClass ArrayList={
  CLASS_INITIALIZATION(Class,
                       ArrayList,
                       Object,
                       ArrayList_DefConstructor,
                       ArrayList_Destructor,
                       Object_differ,
                       Object_puto),
                       ArrayList_Add,
                       ArrayList_vaciar,
                       ArrayList_contains,   
                       ArrayList_isEmpty,
                       ArrayList_iterator,
                       ArrayList_delete, 
                       ArrayList_count,
                       ArrayList_get                     
}; 

/*
** ===================================================================
**     Method      :  ArrayList_Constructor 
**    Description : Constructor
** ===================================================================
*/
void ArrayList_Constructor(void * self){
  struct ArrayList *_a =self;
  
  _a->cuenta=0;
  _a->memoria=0;
  _a->nodos=NULL;
  _a->objetosSonPropios=FALSE;
}

/*
** ===================================================================
**     Method      :  ArrayList_DefConstructor 
**    Description  :  Constructor por defecto   
** ===================================================================
*/
void ArrayList_DefConstructor(void * self, va_list * arg){
  ArrayList_Constructor(self);  
}


/*
** ===================================================================
**     Method      :  ArrayList_Destructor 
**    Description  :  Destructor
** ===================================================================
*/
void *ArrayList_Destructor(void * self){  
  ArrayList_vaciar(self);
  return self;
}

/*
** ===================================================================
**     Method      :  ArrayList_vaciar 
**    Description  :  vacia la lista
** ===================================================================
*/
void ArrayList_vaciar(void * _self){
  struct ArrayList* self = _self;
  
  if(self->objetosSonPropios){
    int i;    
    for(i=0;i< self->cuenta;i++)
      _delete(self->nodos[i]);   
  }
  free(self->nodos);
  self->cuenta=0;
  self->memoria=0;
  self->nodos=NULL;
}

/*
** ===================================================================
**     Method      :  ArrayList_contains 
**    Description  :  devuelve true si el elemento se encuentra
** ===================================================================
*/
int ArrayList_indexOf(void * _self,void * dato){
  struct ArrayList* _a =_self;
  int i;
  
  for(i=0;i<_a->cuenta;i++){
    if(_a->nodos[i]==dato)
      return i;
  }
  
  return -1;  
}

/*
** ===================================================================
**     Method      :  ArrayList_contains 
**    Description  :  devuelve true si el elemento se encuentra
** ===================================================================
*/
bool ArrayList_contains(void * _self,void * dato){
  return ArrayList_indexOf(_self,dato)!=0;
}


/*
** ===================================================================
**     Method      :  ArrayList_iterator 
**    Description  :  devuelve un iterador para el array
** ===================================================================
*/
struct ArrayListIterator{
  struct Iterator super;
  void * arrayList;
  int i;  
};

void* ArrayListIterator_DefConstructor(void * _self, va_list * args){
  struct ArrayListIterator *self = _self;
  self->arrayList = va_arg(*args,void*);
  self->i = 0;  
}

void * ArrayListIterator_next(void * _self){
  struct ArrayListIterator *self = _self;
  return ArrayList_get(self->arrayList,++self->i);
}

bool ArrayListIterator_hasNext(void* _self){
  struct ArrayListIterator *self = _self;  
  return ArrayList_get(self->arrayList,self->i)!=NULL;
}

void * ArrayList_iterator(void * _self){

  const struct IteratorClass ArrayListIterator={
    CLASS_INITIALIZATION(Class,
                       ArrayListIterator,
                       Object,
                       ArrayListIterator_DefConstructor,
                       Object_dtor,
                       Object_differ,
                       Object_puto),
                       ArrayListIterator_next,
                       ArrayListIterator_hasNext
 };
 
 return _new(&ArrayListIterator,_self);
   
}

/*
** ===================================================================
**     Method      :  ArrayList_Add 
**     Description : Agerga un nuevo valor en el Array en la ultima
**     posicion del array
** ===================================================================
*/
void ArrayList_Add(void * self, void* Obj){
  struct ArrayList *_a =self;
  
  if(_a->cuenta==_a->memoria) 
    ArrayList_Expand(self); //pido una posicion mas
  _a->nodos[_a->cuenta]=Obj;
  ++_a->cuenta;
    
}

/*
** ===================================================================
**     Method      :  ArrayList_Add 
**     Description : Agerga un nuevo valor en el Array en la 
**     posicion indicada del array. en caso de que la posicion supere
**     en mas de uno la posicion final, el objeto no se insertara
** ===================================================================
*/
uchar ArrayList_Put(void * self, void* Obj, int index){
  struct ArrayList *_a =self;
  int i;
   
  if(index>_a->cuenta)
    return 1;           //Exception
    
  if(index==LIST_LAST_POSITION){
    ArrayList_Add(self,Obj);
    return 0;			 
  }
    
  if(_a->cuenta==_a->memoria)
    ArrayList_Expand(self); //pido mas memoria
  
  
  
  for(i=index;i<_a->cuenta;i++)
    _a->nodos[i+1]=_a->nodos[i];
        
  _a->nodos[ ++_a->cuenta ]=Obj;
  
   
  
  return 0; 
}


/*
** ===================================================================
**     Method      :  ArrayList_AsArray 
**    Description : Devuelve el array con los valores
** ===================================================================
*/
void* ArrayList_get(void * self,int index){
  struct ArrayList* _a =self;
  
  if(index>_a->cuenta)
    return NULL;
  return _a->nodos[index];
}

/*
** ===================================================================
**     Method      :  ArrayList_Expand 
**    Description : Agrega una cantidad de memoria igual a cantidad al
** 		array.
** ===================================================================
*/
void ArrayList_Expand(void * self){
  struct ArrayList* _a =self;
  void * ptrTmp;
  
  if(!_a->memoria){     //no hay memoria inicial pedida
    ArrayList_expandInit(self,2);  
    return;
  }
  ptrTmp=realloc(_a->nodos,(_a->memoria*2)*sizeof(void*));
  assert(ptrTmp);
  _a->nodos = ptrTmp; 
  _a->memoria*=2;
}
/*
** ===================================================================
**     Method      :  ArrayList_expandInit 
**    Description : Inicia el array con una can
** ===================================================================
*/
#warning cambiar a segundo constructor
void ArrayList_expandInit(void * self,int cantidad){
  struct ArrayList* _a =self;
  void * ptrTmp;
  
  if(_a->memoria==0){   
    DisableInterrupts; 
    ptrTmp=malloc((cantidad)*sizeof(void*));
    EnableInterrupts; 
    assert(ptrTmp);
    _a->nodos = ptrTmp; 
    _a->memoria=cantidad; 
  }
}

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
void ArrayList_OwnsObjects(void* self,bool owns){
    struct ArrayList* _a =self;
    
    _a->objetosSonPropios=owns;  
}

/*
** ===================================================================
**     Method      :  ArrayList_count 
**    Description : Devuelve la cantidad de elementos en el array
** ===================================================================
*/
int ArrayList_count(void * self){
  struct ArrayList* _a =self;
  
  return _a->cuenta;
}

/*
** ===================================================================
**     Method     :  ArrayList_search 
**    Description :  Busca linealmente coincidencia de un elemento 
**      con el comparador
** ===================================================================
*/
void * ArrayList_search(void * self,void * comparator){
  struct ArrayList* _a =self;
  int i;
  
  for(i=0;i<_a->cuenta;i++){
    if(_Comparator_equals(comparator,_a->nodos[i]))
      return _a->nodos[i];
  }
  
  return NULL;  
}

/*
** ===================================================================
**     Method      :  ArrayList_isEmpty 
**    Description : indica si el array esta vacio
** ===================================================================
*/
bool ArrayList_isEmpty(void * self){
   return ArrayList_count(self)==-1;
}


/*
** ===================================================================
**     Method     :  PContainer_Delete 
**    Description :  Elimina la referencia guardada
** ===================================================================
*/
bool ArrayList_delete(void*_self,void * Obj){
  struct ArrayList * _a= _self;
  size_t i;
  bool result=FALSE;
  
  for(i=0;i<_a->cuenta;++i){
    if(_a->nodos[i]==Obj){      
      result=TRUE;
      break;  
    }
  }
  ++i;
  for(;i<_a->cuenta;++i){
    _a->nodos[i-1]=_a->nodos[i];  
  }
  
  if(result)
    _a->cuenta--;
  
  return result;
  
}


#pragma CODE_SEG ARRAY_LIST_CODE 
