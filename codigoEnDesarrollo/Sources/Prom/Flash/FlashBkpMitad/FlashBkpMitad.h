#ifndef _FLASH_BKP_MITAD_H
#define _FLASH_BKP_MITAD_H

#include "FlashBkp.h"
#include "IFshMem.h"
#include "FshTypeSizes.h"

struct FlashBkpMitad{
  struct FlashBkp super;
};

extern const struct FlashBkpClass FlashBkpMitad;
///////////////////////////////////////

#define INITIALIZATION_FLASH_BKP_MITAD(_class,bkpAddr)      \
    INITIALIZATION_FLASH_BKP(_class,bkpAddr)

#define NEW_FLASH_BKP_MITAD(name,bkpAddr) \
  struct FlashBkpMitad name={																				\
    INITIALIZATION_FLASH_BKP_MITAD(&FlashBkpMitad,bkpAddr)  \
  }
  
  
#endif