#ifndef _FLASH_BKP_256_H
#define _FLASH_BKP_256_H

#include "FlashBkp.h"
#include "IFshMem.h"
#include "FshTypeSizes.h"

struct FlashBkp256{
  struct FlashBkp super;
};

extern const struct FlashBkpClass FlashBkp256;

word FlashBkp256_getWord(void * _self,word * address); 
///////////////////////////////////////

#define INITIALIZATION_FLASH_BKP_256(class,bkpAddr)      \
    INITIALIZATION_FLASH_BKP(class,bkpAddr)  						 						                	

#define NEW_FLASH_BKP_256(name,bkpAddr) \
  struct FlashBkp256 name={																				\
    INITIALIZATION_FLASH_BKP_256(&FlashBkp256,bkpAddr)  \
  }
  
  
#endif