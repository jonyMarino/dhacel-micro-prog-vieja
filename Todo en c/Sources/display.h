#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "PE_Types.h" 

#define MAX_NUM_DISPLAY 9999
#define MIN_NUM_DISPLAY -1999

void ResetScroll(void);

void DpyAndSwitch(void);

#ifdef jony_28_06
void PasarASCII(const char *PtrText,byte Num_display);
#endif
 void PasarASCII(const char *PtrText,byte Num_display);
void Pasar_Numero(int valor,byte Num_display, byte Dot);

void Pasar_Numero_Expo(int valor,byte Num_display, byte Dot);

#endif

