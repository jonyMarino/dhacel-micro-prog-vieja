#ifndef _GETTERS_VISUAL_H
#define _GETTERS_VISUAL_H

#include "GetterGenerico.h"
#include "GetterWName.h"
#include "Object.h"
#include "stdtypes.h"

#pragma DATA_SEG GETTERS_VISUAL_DATA                                            
#pragma CODE_SEG GETTERS_VISUAL_CODE 
#pragma CONST_SEG DEFAULT 

struct ConstrGetVisual{
  struct GetterGenerico _prop_base;
  const void * classOf;              //Descriptor de la clase Visual
  char * descripcion;
};

struct GetterVisual{
    struct getter_w_name * _base;
  //Public:
  
  //Private:
  const struct ConstrGetVisual * _CGetter;
  void * Obj;
};

extern const struct GetterWNameClass GetterVisual;
/*
** ===================================================================
**     Function      :  pProp_Constructor
**     Type        :  Polimorfica 
**     Description :  Constructor general de propiedad mediante 
**                    una propiedad de clase
** ===================================================================
*/
void * pGetter_Constructor(struct ConstrGetVisual * ClassGet,void * Obj);
/*
** ===================================================================
**     Method      :  PropVisual_getDescripcion 
**     Description :  Obtiene un string con la descripción de la
**                   propiedad
** ===================================================================
*/
char * GetterVisual_getDescripcion(void * self);

/*
** ===================================================================
**     Method      :  _GetterPrint 
**     Type         : Virtual
**     Description :  Imprime por pantalla el dato de la propiedad con 
**                    su formato correspondiente
** ===================================================================
*/
void vGetterVisual_Print(void* self,uchar num_display);

/*
** ===================================================================
**     Method      :  GetterVisual_getVal 
**     Description :  Obtiene el valor del getter
** ===================================================================
*/
int GetterVisual_getVal(void* _self);
/*
** ===================================================================
    GETTER NUMERICO
** ===================================================================
*/

struct ConstrGetterNum{
  struct ConstrGetVisual _base;
  uchar dot;
};

struct GetterNum{
  struct GetterVisual _prop_base;
};

extern const struct GetterWNameClass GetterNum;

/*
** ===================================================================
    GETTER TEXTUAL
** ===================================================================
*/
struct ConstrGetterTxt{
  struct ConstrGetVisual _base;
  char* (*getTxt)(uchar);
};

struct GetterTxt{
  struct GetterVisual _prop_base;
};

extern const struct GetterWNameClass GetterTxt;

#pragma DATA_SEG DEFAULT                                            
#pragma CODE_SEG DEFAULT 


#endif