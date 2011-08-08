#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "matriz.h"
#include "PE_Types.h"
#include "Mydefines.h" 


#define MAX_NUM_DISPLAY 9999
#define MIN_NUM_DISPLAY -1999

 struct Display{
  const byte nroDisplay;
  const byte nroDigitos;           //numero de digitos que tiene el display
  char Apresentar[MAX_DIGITOS];    //valor o mensaje a presentar
  char deco7seg[MAX_DIGITOS];      //valor o mensaje decodificado a 7 segmentos
  byte decimales;
  byte longMsj;
  byte posicionScroll;
  void (*presentarTexto) (char*,void*);
  void (*presentarValor) (int,void*);
  void (*presentarEspcific) (int,void*);
  
};


#define setDecimalesDys(dys,nro,val) dys[nro].decimales=val
#define getDecimalesDys(dys,nro) dys[nro].decimales

extern struct Display displays[CANTIDAD_DISPLAYS];
extern const byte  DigInfOn[4];
extern const byte  DigSupOn[4];
extern bool led[NUM_SALIDAS];


void ResetScroll(void);

void SetLeds();

void SelecDigit(struct Display* dys,byte digit);

void SetValDigit(struct Display* dys,byte nroCaracter);

byte handlerScroll(struct Display* dys,byte digit);


#endif

