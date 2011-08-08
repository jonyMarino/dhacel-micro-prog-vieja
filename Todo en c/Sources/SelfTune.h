#ifndef _SELF_TUNE_H
#define _SELF_TUNE_H

#define St_abrt_MAX 2000000
#define St_abrt_MIN 4000

//#define St_T_MAX 2000000
//#define St_T_MIN 6000
#define St_Hist 2

extern dword St_Counter[CANTIDAD_SAL_CONTROL+CANTIDAD_SAL_ALARMA];
extern byte St_Step[CANTIDAD_SAL_CONTROL+CANTIDAD_SAL_ALARMA];

void SelfTuneControl(byte chan, int spaut);

void setEstateAutoTune();

#endif