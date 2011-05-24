/*MODULE: MethodContainer.cpp*/

#include <stdlib.h>
#include "MethodContainer.hpp"
#include "Method.h"
#include "Object.h"

MethodContainer::MethodContainer(){}
/*
** ===================================================================
**     Method     :  MethodContainer_Add 
**    Description : Agrega una funcion al contenedor y retorna una referencia
**  al metodo
** ===================================================================
*/
bool MethodContainer::add(void *obj){
  
  if(classOf(obj)!=&Method)
    return EXIT_FAILURE; 
  LinkedList::add(obj);
  return EXIT_SUCCESS;
}

/*
** ===================================================================
**     Method     :  MethodContainer_Execute 
**    Description : Ejecuta las funciones agergadas al contenedor
** ===================================================================
*/
void MethodContainer::executeMethods(){
  LinkedListIterator it;
  
  linkedListIterator(&it);
  
  while(it.hasNext()){
    struct Method * m = (struct Method *)it.next();
    (*(m->pmethod))(m->Obj);
  }
}


