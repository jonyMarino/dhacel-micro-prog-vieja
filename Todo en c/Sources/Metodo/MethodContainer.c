/*MODULE: MethodContainer.c*/

#include <stdlib.h>
#include "MethodContainer.h"
#include "Method.h"
#include "Object.h"

/*
** ===================================================================
**     Method     :  MethodContainer_Add 
**    Description : Agrega una funcion a la lista
** ===================================================================
*/
bool MethodContainer_add(void * Mobj,unsigned char NroList){
  
 //obj debe ser tipo method
  struct Method * tmpObj= Mobj;
  
  LinkedList_aniadirFinal(tmpObj,NroList);
  
  return EXIT_SUCCESS;
}

/*
** ===================================================================
**     Method     :  MethodContainer_Execute 
**    Description : Ejecuta las funciones agergadas al contenedor
** ===================================================================
*/
void executeMethods(unsigned char NroList){
  struct Nodo * tmpNodo;
  
  for(tmpNodo=LinkedList_getPrimerNodo(NroList);tmpNodo!=NULL;tmpNodo=getNext(tmpNodo)){
    struct Method * m = (struct Method *)getDataObj(tmpNodo);
    (*(m->pmethod))(m->Obj);
   
    if(!hasNext(tmpNodo))  //el proximo es NULL??
      break;
  }
}

