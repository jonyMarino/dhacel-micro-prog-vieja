#ifndef _FECHA_TIEMPO_H
#define _FECHA_TIEMPO_H

#include "Object.h"
#include "PE_Types.h"



struct FechaTiempo{
  struct Object super;
};

typedef struct { /* It contains actual number of hours, minutes, seconds and hundreths of seconds. */
  byte Hour; /* hours (0 - 23) */
  byte Min; /* minutes (0 - 59) */
  byte Sec; /* seconds (0 - 59) */
} TIMEREC;


typedef struct { /* It contains actual year, month, and day description. */
  word Year; /* years (1998 - 2099) */
  byte Month; /* months (1 - 12) */
  byte Day; /* days (1 - 28/31) */
} DATEREC;



struct FechaTiempoClass{
  struct Class super;
  void(*getTiempo)(void * self,TIMEREC *time);
  byte (*setTiempoValidado)(void * self,byte hour,byte min,byte secs);
  void (*getFecha)(void * self,DATEREC *date);
  byte (*setFechaValidada)(void * self,word year,byte month,byte day);
};

extern const struct Class FechaTiempoClass;
extern const struct FechaTiempoClass FechaTiempo;


#define FECHA_TIEMPO_CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto,getTiempo,setTiempoValidado,getFecha,setFechaValidada)\
  CLASS_INITIALIZATION(description,name,super,contructor,destructor,differ,puto),\
  getTiempo,  \
  setTiempoValidado,  \
  getFecha,  \
  setFechaValidada
  
bool isTiempoValido(byte hora,byte min,byte segs);
bool isFechaValida(word anio,byte mes,byte dia);
  
void getTiempo(void * self,TIMEREC *time);
byte setTiempo(void * self,byte hour,byte min,byte secs);
void getFecha(void * self,DATEREC *date);
byte setFecha(void * self,word year,byte month,byte day);

/*
** ===================================================================
**     Function      :  FechaTiempo_getMaximoDiaDelMes 
**
**     Description :
**         Retorna la maxima cantidad de dias para el mes de ese anio
**     Parameters  :
**         Year            - anio
**         Month           - Mes
**     Returns     :
**         maxima cantidad de dias ara el mes de ese anio
**         0							 - Error
** ===================================================================
*/
byte  FechaTiempo_getMaximoDiaDelMes(word Year,byte Month);



  
#endif