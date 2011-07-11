#ifndef _FECHA_TIEMPO_H
#define _FECHA_TIEMPO_H

#include "PE_Types.h"


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

  
bool isTiempoValido(byte hora,byte min,byte segs);

bool isFechaValida(word anio,byte mes,byte dia);
  
void getTiempo(TIMEREC *time);

byte setTiempo(byte hour,byte min,byte secs);

void getFecha(DATEREC *date);

byte setFecha(word year,byte month,byte day);

byte  FechaTiempo_getMaximoDiaDelMes(word Year,byte Month);



  
#endif