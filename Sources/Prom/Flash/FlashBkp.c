#include <assert.h>
#include "FlashBkp.h"
#include "PromBkpProtected.h"
#include "stddef.h"
#include "IFsh10.h"


const struct Class FlashBkpClass={
  CLASS_INITIALIZATION(Class,FlashBkpClass,Class,Object_ctor,Object_dtor,Object_differ,Object_puto)  
};



const struct FlashBkpClass FlashBkp={
  CLASS_INITIALIZATION(FlashBkpClass,FlashBkp,PromBkp,FlashBkp_ctor,Object_dtor,Object_differ,Object_puto),
  FlashBkp_getWord,
  FlashBkp_setWord,
  FlashBkp_getWord,
  FlashBkp_setWord,
  NULL,
  NULL,
  FlashBkp_borrarProm,
  FlashBkp_grabarProm,
  NULL,
  NULL,
  NULL 
};


void FlashBkp_ctor(void * _self,va_list * args){
  struct FlashBkp * self = (struct FlashBkp *)_self;
  super_ctor(&FlashBkp,_self,args);
  self->direccionBkp = va_arg(*args,const void *);
  self->escrituraHabilitada=TRUE;
  self->paginaAGrabar = NULL;
}

bool getIndexArray(void * _self,word address){
  struct FlashBkpClass * _class = classOf(_self);
  assert(_class && _class->getIndexArray );
  return _class->getIndexArray( _self , address ); 
}

void setIndexArray(void * _self,word address){
  struct FlashBkpClass * _class = classOf(_self);
  assert(_class && _class->setIndexArray );
  _class->setIndexArray( _self , address ); 
}

void clearIndexArray(void * _self){
  struct FlashBkpClass * _class = classOf(_self);
  assert(_class && _class->clearIndexArray );
  _class->clearIndexArray( _self ); 
}

word * FlashBkp_getDireccionBkp(void* _self){
  struct FlashBkp * self= (struct FlashBkp *)_self;
  return self->direccionBkp;
}

/*
** ===================================================================
**    Function    :  FlashBkp_setWord 
**    Description : 
** ===================================================================
*/
byte FlashBkp_setWord(void* self,word*Address,word valor){
  struct FlashBkp * _f=(struct FlashBkp *)self;
  
  word Addr = (word)Address;
  
  if (Addr & 1 || IFsh10_isInPage(Address,_f->direccionBkp)){
    /* Aligned address ? */
    return ERR_NOTAVAIL;
  }
 

  if(*(word *)Addr!=valor){
    byte shift;
    
    
    if(!_f->escrituraHabilitada)		//no esta libre para escribir?
      return ERR_NOTAVAIL;
    
    if((word)_f->paginaAGrabar==0) 
      _f->paginaAGrabar=(void*)(Addr & (65535 ^ (PAGE_SIZE-1)));
    else if (_f->paginaAGrabar!=(void*)(Addr & (65535 ^ (PAGE_SIZE-1))))
      return ERR_BUSY;      /* Todavia no se grabo la página en cola*/  
  
    WriteWord(((word*)_f->direccionBkp)[(Addr&(PAGE_SIZE-1))/2], valor);
    setIndexArray(self,Addr&(PAGE_SIZE-1));
    PromBkp_setAGrabar(self,TRUE);
  }
  return ERR_OK;
}

bool PromBkp_aGrabar(void *self,word direccion){
  
  return getIndexArray(self,direccion&(PAGE_SIZE-1));
}
/*
** ===================================================================
**    Function    :  FlashBkp_getWord 
**    Description : 
** ===================================================================
*/
word FlashBkp_getWord(void*self,word*direccion){
  struct FlashBkp * _f=(struct FlashBkp *)self;
  word dir = (word)direccion;
  
  if(!PromBkp_getAGrabar(self) || !IFsh10_isInPage(direccion,_f->paginaAGrabar) ||   !PromBkp_aGrabar(self,dir)) // con que el primero este para grabar alcanza
    return *direccion;
  else
    return *(word *)&((word*)_f->direccionBkp)[(dir&(PAGE_SIZE-1))/2];

}

/*
** ===================================================================
**    Function    :  FlashBkp_grabarProm 
**    Description : 
** ===================================================================
*/
void FlashBkp_backupSector(void * self){
  struct FlashBkp * _f=(struct FlashBkp *)self;
  word a;  
  
  for (a=0;a<PAGE_SIZE/2;a+=2){
      if (!getIndexArray(self,a)){
        word  addr = (((word*)_f->direccionBkp)[a/2]);
        word value = *(((word*)_f->paginaAGrabar)+a/2);
        WriteWord(addr,value);
    
    }
  }
  clearIndexArray(self);
}
/*
** ===================================================================
**    Function    :  FlashBkp_grabarProm 
**    Description : 
** ===================================================================
*/
void *FlashBkp_grabarProm(void*self){
  struct FlashBkp * _f=(struct FlashBkp *)self;
  byte err;
  void * paginaTmp = _f->paginaAGrabar;

	 if(_f->escrituraHabilitada){ 
  	 _f->escrituraHabilitada=FALSE;
  	 FlashBkp_backupSector(self);                      /* Backup sector */
     return _f->direccionBkp;
	 }
   //PromBkp_callPreEraseListeners(self,paginaTmp);
   err=IFsh10_GrabarFlash2(paginaTmp,_f->direccionBkp);
    if(err)
      return NULL;                     /* Return error code if previous operation finished not correctly */
 	 _f->paginaAGrabar=0;  	 
 	 PromBkp_borrarPagina(self,_f->direccionBkp);
 	 return paginaTmp;  
}
/*
** ===================================================================
**    Function    :  FlashBkp_borrarProm 
**    Description : 
** ===================================================================
*/
byte FlashBkp_borrarProm(void*self,void*direccion){
  struct FlashBkp * _f=(struct FlashBkp *)self;
  
  byte err=EraseSectorInternal(*(word*)direccion);
  if(err)
    return err;
  if(IFsh10_getPage(direccion)==IFsh10_getPage(_f->direccionBkp)){
    PromBkp_setAGrabar(self,FALSE);
    _f->escrituraHabilitada=TRUE;
  }
  
  return ERR_OK;    
}

