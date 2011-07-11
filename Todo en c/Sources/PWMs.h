#ifndef PWMs_H
#define PWMs_H
#include "Cpu.h"
extern const dword PeriodsTicks[PWM_Last_Period+1];

void PWM01_Init(void);
void PWM45_Init(void);
void PWM23_Init(void);
void PWM67_Init(void);

void setPWM_period(TPeriod Mode, byte salida);
void PWM_SetRatio16(word Ratio,byte salida);

#endif