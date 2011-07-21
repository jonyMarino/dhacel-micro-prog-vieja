#ifndef LOOP_CONTROL_ALARMAS_H
#define LOOP_CONTROL_ALARMAS_H

#include "Mydefines.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "PID.h"
#include "Alarmas.h"

 struct ArgLoop{
  struct ArgControl *_control;
  struct ArgAlarma *_alarmas;
  byte CantCanales;
  byte CantAlarmas;
 };
 
 void loopDeControlYAlarmas(void* arg);
 void intLoopControlYAlarmas(struct ArgLoop* arg);

#endif