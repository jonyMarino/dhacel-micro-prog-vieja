/*MODULE: GettersVisual.c*/
#include <stdarg.h>
#include <stddef.h>

#include "display.h"
#include "GettersVisual.h"

#pragma DATA_SEG GETTERS_VISUAL_DATA                                            
#pragma CODE_SEG GETTERS_VISUAL_CODE 
#pragma CONST_SEG DEFAULT 

void GetterVisual_DefConstructor(void* _self,va_list* args);
void GetterVisual_Print(void* self,uchar num_display);


const struct Class GetterClass;
const struct GetterClass Getter;
const struct Class GetterClass; 

const struct GetterWNameClass GetterVisual={
  GETTER_W_NAME_CLASS_INITIALIZATION(GetterWNameClass,
                                     GetterVisual,
                                     GetterWName,
                                     GetterVisual_DefConstructor,
                                     NULL,
                                     NULL,
                                     NULL,
                                     GetterVisual_getVal,
                                     GetterVisual_Print,
                                     GetterVisual_getDescripcion)
};


/*
** ===================================================================
**     Function      :  pGetter_Constructor
**     Type        :  Polimorfica 
**     Description :  Constructor general de propiedad mediante 
**                    una propiedad de clase
** ===================================================================
*/
void * pGetter_Constructor(struct ConstrGetVisual * ClassGet,void * Obj){
  struct ConstrGetVisual * p=ClassGet;
  return new(&*((struct Tclass**)p->classOf),ClassGet,Obj);
  
}
/*
** ===================================================================
**     Method      :  GetterVisual_Constructor 
**     Description :  Constryctor del GetterVisual
** ===================================================================
*/
void  GetterVisual_Constructor(void * _self,void* C_Getter,void* Obj){
  struct GetterVisual * _g=_self; 
  
  _g->_CGetter=C_Getter;
  _g->Obj=Obj; 
}

/*
** ===================================================================
**     Method      :  PropVisual_DefConstructor 
**     Description :  Constryctor por defecto de la PropVisual
** ===================================================================
*/
void GetterVisual_DefConstructor(void* _self,va_list* args){
  GetterVisual_Constructor(_self,va_arg(*args,void*),va_arg(*args,void*));    
}

/*
** ===================================================================
**     Method      :  GetterVisual_getObj 
**     Description :  Devuelve el objeto asociado al getter
** ===================================================================
*/
void *GetterVisual_getObj(void* _self){
   struct GetterVisual * _g=_self;  
   return _g->Obj;
}

/*
** ===================================================================
**     Method      :  GetterVisual_getVal 
**     Description :  Obtiene el valor del getter
** ===================================================================
*/
int GetterVisual_getVal(void* _self){
   struct GetterVisual * _g=_self;  
   return Getter_getVal(_g->_CGetter,_g->Obj);
}
/*
** ===================================================================
**     Method      :  PropVisual_getDescripcion 
**     Description :  Obtiene un string con la descripción de la
**                   propiedad
** ===================================================================
*/

char * GetterVisual_getDescripcion(void * self){
  struct GetterVisual * _g=self;
  return ((struct ConstrGetVisual*)(_g->_CGetter))->descripcion;  
}

/*
** ===================================================================
**     Method      :  PropVisual_Print 
**     Description :  Forma en que imprime 
** ===================================================================
*/
void GetterVisual_Print(void* self,uchar num_display){
  struct GetterVisual * _g=self;
  int val;
  
  val=GetterVisual_getVal(_g); 
  Pasar_Numero(val,num_display,0); 
}

/*
** ===================================================================
    GETTER NUMERICO
** ===================================================================
*/
void GetterNum_Print(void*self,uchar num_display);

const struct GetterWNameClass GetterNum={
  GETTER_W_NAME_CLASS_INITIALIZATION(GetterWNameClass,
                                     GetterNum, 
                                     GetterVisual,
                                     GetterVisual_DefConstructor,
                                     NULL,
                                     NULL,
                                     NULL,
                                     GetterVisual_getVal,
                                     GetterNum_Print,
                                     GetterVisual_getDescripcion)

};


/*
** ===================================================================
**     Method      :  GetterNum_Print 
**     Description :  Forma en que imprime 
** ===================================================================
*/
void GetterNum_Print(void*self,uchar num_display){
  struct GetterVisual* _g=self;
  struct ConstrGetterNum* Block= (struct ConstrGetterNum*) _g->_CGetter; 
  int _val= GetterVisual_getVal(self);
  uchar _dot= Block->dot;
  
  Pasar_Numero(_val,num_display,_dot);
  
}

/*
** ===================================================================
    GETTER TEXTUAL
** ===================================================================
*/

void GetterTxt_Print(void*self,uchar num_display);

const struct GetterWNameClass GetterTxt={
    GETTER_W_NAME_CLASS_INITIALIZATION(GetterWNameClass,
                                     GetterTxt,  
                                     GetterVisual,
                                     GetterVisual_DefConstructor,
                                     NULL,
                                     NULL,
                                     NULL,
                                     GetterVisual_getVal,
                                     GetterTxt_Print,
                                     GetterVisual_getDescripcion)
};



/*
** ===================================================================
**     Method      :  GetterNum_Print 
**     Description :  Forma en que imprime 
** ===================================================================
*/
void GetterTxt_Print(void*self,uchar num_display){
  struct GetterVisual* _g=self;
  struct ConstrGetterTxt* Block= (struct ConstrGetterNum*) _g->_CGetter; 
  int _val= GetterVisual_getVal(self);
  char * str= (*(Block->getTxt))((uchar)_val);
  
  PasarASCII(str,num_display);
  
}

#pragma CODE_SEG GETTERS_VISUAL_CODE  
