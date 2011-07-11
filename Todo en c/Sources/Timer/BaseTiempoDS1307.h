#ifndef _BASETIEMPO_DS1307_H
#define _BASETIEMPO_DS1307_H

#include "PE_Types.h"
#include "FechaTiempo.h"

/*Registros de tiempo*/
typedef union{
  struct{
    byte hourLow:4;
    byte hourHigh:2;
    bool amPm:1;
  }bits;
  byte _byte;
}HoursRegister;

typedef union{
  struct{
    byte minLow:4;
    byte minHigh:3;
  }bits;
  byte _byte;
}MinutesRegister;

typedef union{
  struct{
    byte secLow:4;
    byte secHigh:3;
    bool CH:1;
  }bits;
  byte _byte;
}SecondsRegister;

typedef struct{
  SecondsRegister seconds;
  MinutesRegister minutes;
  HoursRegister hours;  
}TimeRegisters;

/*Registros de Fecha*/
typedef union{
  struct{
    byte dayLow:4;
    byte dayHigh:2;
  }bits;
  byte _byte;
}DaysRegister;

typedef union{
  struct{
    byte monthLow:4;
    byte monthHigh:1;
  }bits;
  byte _byte;
}MonthsRegister;

typedef union{
  struct{
    byte yearLow:4;
    byte yearHigh:4;
  }bits;
  byte _byte;
}YearRegister;

typedef struct{
  DaysRegister days;
  MonthsRegister months;
  YearRegister years;
}DateRegister;


void EI2C1_OnNACK(void);

void EI2C1_OnTxChar(void);

void EI2C1_OnRxChar(void);

void BaseTiempoDS1307_getFecha(DATEREC *date);

byte BaseTiempoDS1307_setFechaValidada(word year,byte month,byte day);

void BaseTiempoDS1307_getTiempo(TIMEREC *time);

byte BaseTiempoDS1307_setTiempoValidado(byte horas,byte min,byte segs);

void BaseTiempoDS1307_Init();

#endif
