#ifndef PID_H
#define PID_H

#include "Mydefines.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"

struct ArgControl{
  byte NroControl;
  byte NroAlarma;
  byte salida;
};

extern int duty_cont_ch[CANTIDAD_CANALES];
extern long suminteg[CANTIDAD_CANALES];
extern long buffer_deriv[CANTIDAD_CANALES];
extern int deriv[CANTIDAD_CANALES];
extern int ValCont[CANTIDAD_CANALES];
extern int dutytmp;

#ifdef VPROP
extern int duty_vprop;
extern long cnt_duty_vprop;
#endif

void calculosDeControl(struct ArgControl* arg);

#endif