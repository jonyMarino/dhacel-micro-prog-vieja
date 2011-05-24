#ifndef _GETTER_GENERICO_H
#define _GETTER_GENERICO_H

#include "Object.h"

 struct GetterGenerico{
  struct Object super;
  int (*_get)(void*);
 };
 
 struct GetterGenericoClass{
  struct Class super;
  int (*getValObj)(const void*_self,const void*obj);
 };
 
extern const struct Class GetterGenericoClass;
extern const struct GetterGenericoClass GetterGenerico; 
 
 /*
** ===================================================================
**     Method      :  Prop_getVal 
**     Description :  Obtiene el valor de la propiedad del objeto
** ===================================================================
*/
int Getter_getVal(void * self,void * Obj);


#define GETTER_GENERICO_CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto,getVal)\
  CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto),\
  getVal

#endif