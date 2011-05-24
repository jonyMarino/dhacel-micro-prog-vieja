#include <assert.h>

#include "FlashBkpMitad.h"
#include "PromBkpProtected.h"
#include "stddef.h"
#include "IFsh10.h"

byte FlashBkpMitad_setWord(void*,word*,word);
word FlashBkpMitad_getWord(void*,word*);

bool FlashBkpMitad_getIndexArray(void * _self,word);
void FlashBkpMitad_setIndexArray(void * _self,word);
void FlashBkpMitad_clearIndexArray(void * _self);  
  
const struct FlashBkpClass FlashBkpMitad={
  CLASS_INITIALIZATION(FlashBkpClass,FlashBkpMitad,FlashBkp,FlashBkp_ctor,Object_dtor,Object_differ,Object_puto),
  FlashBkpMitad_getWord,
  FlashBkpMitad_setWord,
  FlashBkpMitad_getWord,
  FlashBkpMitad_setWord,
  NULL,
  NULL,
  FlashBkp_borrarProm,
  FlashBkp_grabarProm,
  FlashBkpMitad_getIndexArray,
  FlashBkpMitad_setIndexArray,
  FlashBkpMitad_clearIndexArray 
};

#define indiceFlash  ((byte*)((byte*)FlashBkp_getDireccionBkp(self)+PAGE_SIZE/2))


bool FlashBkpMitad_getIndexArray(void * _self,word index){
  struct FlashBkpMitad * self = (struct FlashBkpMitad *)_self; 
  
  return !indiceFlash[index];
}

void FlashBkpMitad_setIndexArray(void * _self,word index){
  struct FlashBkpMitad * self = (struct FlashBkpMitad *)_self; 
  WriteWord((word*)&indiceFlash[index],0);	
}

void FlashBkpMitad_clearIndexArray(void * _self){
  struct FlashBkpMitad * self = (struct FlashBkpMitad *)_self; 
}


byte FlashBkpMitad_setWord(void * _self,word * address,word data){
  struct FlashBkpMitad * self = (struct FlashBkpMitad *)_self; 
  assert( ((word)address &PAGE_SIZE) <=PAGE_SIZE/2);
  if(((word)address &PAGE_SIZE)>PAGE_SIZE/2)
    return ERR_ADDR; //cambiar : error	
  return super_setWord(&FlashBkpMitad,_self,address,data);
}

word FlashBkpMitad_getWord(void * _self,word * address){
  struct FlashBkpMitad * self = (struct FlashBkpMitad *)_self; 
  assert(((word)address &PAGE_SIZE)<=PAGE_SIZE/2);
  if(((word)address &PAGE_SIZE)>PAGE_SIZE/2)
    return ERR_ADDR; //cambiar : error	
  return super_getWord(&FlashBkpMitad,_self,address);

}               
