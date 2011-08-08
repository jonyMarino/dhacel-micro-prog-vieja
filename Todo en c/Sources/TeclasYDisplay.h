#ifndef _TECLAS_Y_DISPLAY_H
#define _TECLAS_Y_DISPLAY_H

#include "PE_Types.h"
#include "Mydefines.h" 
#include "display.h"
#include "teclas.h"


#define PresentarValorExpo(val,nDys) ((*(displays[nDys].presentarEspcific))((int)val,&displays[nDys]))
#define PresentarValor(val,nDys) ((*(displays[nDys].presentarValor))((int)val,&displays[nDys]))
#define PresentarMsj(text,nDys) ((*(displays[nDys].presentarTexto))((char*)text,&displays[nDys]))

void handlerTeclasYDisplay(struct Display *_displays);
void AsciiToSevenSeg(void*);
void Pasar_ASCII(char *PtrText,void*);
void PasarNumero(int val,void*);
void PasarNumeroExpo(int val,void*);
void initDisplay(struct Display *_displays);


#endif