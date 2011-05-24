 /* MODULE: GetterGenerico.c*/
 
#include "GetterGenerico.h"
 
const struct Class GetterGenericoClass;

const struct GetterGenericoClass GetterGenerico={
  GETTER_GENERICO_CLASS_INITIALIZATION(GetterGenericoClass,
                                       GetterGenerico,
                                       Object,
                                       NULL,
                                       NULL,
                                       NULL,
                                       NULL,
                                       Getter_getVal)
}; 

/*
** ===================================================================
**     Method      :  Prop_getVal 
**     Description :  Obtiene el valor de la propiedad del objeto
** ===================================================================
*/
int Getter_getVal(void * self,void * Obj){
  const struct GetterGenerico * _g=self;
  return (*(_g->_get))(Obj);
}