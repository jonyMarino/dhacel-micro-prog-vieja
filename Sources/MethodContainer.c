/*MODULE: MethodContainer.c*/

#include <stdlib.h>
#include <stdarg.h>
#include "MethodContainer.h"

void MethodContainer_Defconstructor(void * _self,va_list * args);
void *MethodContainer_Destructor(void* _self);

const struct Class MethodContainer={
    &Class,
  "",
  &Object,
  sizeof(struct MethodContainer),
  MethodContainer_Defconstructor,
  MethodContainer_Destructor,
    NULL, // differ
  NULL, // puto
};

/*
** ===================================================================
**     Method     :  MethodContainer_Constructor 
**    Description : Constructor del objeto 
** ===================================================================
*/
void MethodContainer_Constructor(void * _self){
  struct MethodContainer * mc= _self;
  newAlloced(&mc->list,&LinkedList);  
}
/*
** ===================================================================
**     Method     :  MethodContainer_Demconstructor 
**    Description : Constructor por defecto del objeto 
** ===================================================================
*/
void MethodContainer_Defconstructor(void * _self,va_list * args){
  MethodContainer_Constructor(_self);  
}
/*
** ===================================================================
**     Method     :  MethodContainer_Destructor 
**    Description : Destructor del objeto 
** ===================================================================
*/
void *MethodContainer_Destructor(void* _self){
  struct MethodContainer * mc= _self;
  deleteAlloced(&mc->list);
  return _self;
}

/*
** ===================================================================
**     Method     :  MethodContainer_Add 
**    Description : Agrega una funcion al contenedor y retorna una referencia
**  al metodo
** ===================================================================
*/
void MethodContainer_add(void * _self,struct Method *method){
  struct MethodContainer * mc= _self;
  
  LinkedList_add(&mc->list,method);
}

/*
** ===================================================================
**     Method     :  MethodContainer_Execute 
**    Description : Ejecuta las funciones agergadas al contenedor
** ===================================================================
*/
void MethodContainer_execute(void*_self){
  struct MethodContainer * mc= _self;
  struct LinkedListIterator it;
  
  LinkedList_linkedListIterator(&mc->list,&it);
  
  while(LinkedListIterator_hasNext(&it)){
    struct Method * m = LinkedListIterator_next(&it);
    (*(m->pmethod))(m->Obj);
  }
  
  deleteAlloced(&it);
}
/*
** ===================================================================
**     Method     :  MethodContainer_iterator 
**    Description : devuelve el iterador del method container
** ===================================================================
*/
void MethodContainer_iterator(void*_self,void * it){
  struct MethodContainer * mc= _self;
  LinkedList_linkedListIterator(&mc->list,it);
}

/*
** ===================================================================
**     Method     :  MethodContainer_isEmpty 
**    Description : Se fija si el contenedor esta vacio
** ===================================================================
*/
uchar MethodContainer_isEmpty(void*_self){
  struct MethodContainer * mc= _self;
  
  return LinkedList_Vacia(&mc->list);
  
}
/*
** ===================================================================
**     Method     :  MethodContainer_getSize 
**    Description :  Retorna la cantidad de metodos guardados
** ===================================================================
*/
uint MethodContainer_getSize(void*_self){
  struct MethodContainer * mc= _self;
  
  return LinkedList_Cantidad(&mc->list);
}

/*
** ===================================================================
**     Method     :  MethodContainer_getMethod 
**    Description :  Retorna el metodo(funcion+Obj) 
**                  guardado en la posicion i
** ===================================================================
*/
const struct Method* MethodContainer_getMethod(void*_self,uint i){
  struct MethodContainer * mc= _self;
  return LinkedList_get(&mc->list,i); 
}

/*
** ===================================================================
**     Method     :  MethodContainer_DeleteMethod 
**    Description :  Elimina el metodo
** ===================================================================
*/
void MethodContainer_delete(void * _self,struct Method *method){
  struct MethodContainer * mc= _self;
  void * p_tmp;
  
  LinkedList_deleteReference(&mc->list,method);
}


