#include <assert.h>

#include "FlashBkp256.h"
#include "PromBkpProtected.h"
#include "stddef.h"
#include "IFsh10.h"

byte FlashBkp256_setWord(void*,word*,word);
word FlashBkp256_getWord(void*,word*);

bool FlashBkp256_getIndexArray(void * _self,word);
void FlashBkp256_setIndexArray(void * _self,word);
void FlashBkp256_clearIndexArray(void * _self);  
  
const struct FlashBkpClass FlashBkp256={
  &FlashBkpClass,
  "",
  (struct Class *)&FlashBkp,
  sizeof(struct FlashBkp256),
  NULL,
  NULL,
  NULL, // differ
  NULL, // puto
  FlashBkp256_getWord,
  FlashBkp256_setWord,
  FlashBkp256_getWord,
  FlashBkp256_setWord,
  NULL,
  NULL,
  FlashBkp_borrarProm,
  FlashBkp_grabarProm,
  FlashBkp256_getIndexArray,
  FlashBkp256_setIndexArray,
  FlashBkp256_clearIndexArray 
};

#define indiceFlash  ((byte*)((byte*)FlashBkp_getDireccionBkp(self)+PAGE_SIZE/2))


bool FlashBkp256_getIndexArray(void * _self,word index){
  struct FlashBkp256 * self = (struct FlashBkp256 *)_self; 
  
  return !indiceFlash[index];
}

void FlashBkp256_setIndexArray(void * _self,word index){
  struct FlashBkp256 * self = (struct FlashBkp256 *)_self; 
  WriteWord(indiceFlash[index],0);	
}

void FlashBkp256_clearIndexArray(void * _self){
  struct FlashBkp256 * self = (struct FlashBkp256 *)_self; 
}


byte FlashBkp256_setWord(void * _self,word * address,word data){
  struct FlashBkp256 * self = (struct FlashBkp256 *)_self; 
  assert( ((word)address &PAGE_SIZE) <=PAGE_SIZE/2);
  if(((word)address &PAGE_SIZE)>PAGE_SIZE/2)
    return 0; //cambiar : error	
  return super_setWord(&FlashBkp256,_self,address,data);
}

word FlashBkp256_getWord(void * _self,word * address){
  struct FlashBkp256 * self =(struct FlashBkp256 *) _self; 
  assert(((word)address &PAGE_SIZE)<=PAGE_SIZE/2);
  if(((word)address &PAGE_SIZE)>PAGE_SIZE/2)
    return 0; //cambiar : error	
  return super_getWord(&FlashBkp256,_self,address);

}               
