#ifndef _FLASH_BKP_H
#define _FLASH_BKP_H

#include "PromBkp.h"
#include "IFshMem.h"
#include "FshTypeSizes.h"




struct FlashBkp{
  struct PromBkp super;
  const void * direccionBkp;
//  word indiceFlash[PAGE_SIZE/32];
  bool escrituraHabilitada;
  void * paginaAGrabar;		//actual pagina a grabar
};

struct FlashBkpClass{
  struct PromBkpClass super;
  bool (*getIndexArray)(void *,word);
  void (*setIndexArray)(void *,word);
  void (*clearIndexArray)(void *);
};

extern const struct Class FlashBkpClass;
extern const struct FlashBkpClass FlashBkp;

void FlashBkp_ctor(void * _self,va_list * args);
byte FlashBkp_setWord(void*,word*,word);
word FlashBkp_getWord(void*,word*);
void *FlashBkp_grabarProm(void*);  
byte FlashBkp_borrarProm(void*self,void*direccion);

//cambiar: protected
word * FlashBkp_getDireccionBkp(void* _self); 
///////////////////////////////////////



#define INITIALIZATION_FLASH_BKP(_class,bkpAddr)       \
    INITIALIZATION_PROM_BKP(_class),              							\
    (const void*)bkpAddr,														\
    TRUE,																						\
    NULL


#endif