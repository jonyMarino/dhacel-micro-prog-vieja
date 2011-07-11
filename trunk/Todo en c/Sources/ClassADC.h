#ifndef CLASS_ADC_H
#define CLASS_ADC_H

#include "Mydefines.h"
#include "PE_Types.h"
#include "Sensor.h"

#pragma DATA_SEG CLASS_ADC_DATA                                            
#pragma CODE_SEG CLASS_ADC_CODE 

#ifndef CANTIDAD_CANALES
  #define CANTIDAD_CANALES 1
#endif 

extern const int ADTIME;

extern const void *const TAdc;	 //clase TAdc

struct TAdc{
  const struct T_Adc * ad;
  byte pin;
};

typedef enum{
  AD_CONVERTING,
  AD_WAITING
}AD_State;

void AD_Constructor(struct TAdc *,byte pin);

int AD_getValue(struct TAdc *);

bool AD_isnew(struct TAdc *);

/*
** ===================================================================
**     Method      :  getSate 
**    Description : Obtiene el estado de los AD
** ===================================================================
*/
AD_State getState(void);

/*
** ===================================================================
**     Method      :  AD_Print 
**    Description :   Imprime el valor del AD
** ===================================================================
*/
void AD_Print(struct TAdc * self,uchar num_disp);

struct T_Adc{
  const struct GetterClass super;
  bool (*isnew)(void*);
};

#define _AD_isnew(ad) \
          (*(*(struct T_Adc**)ad)->isnew)(ad)
          
#pragma DATA_SEG DEFAULT                                            
#pragma CODE_SEG DEFAULT 
#endif