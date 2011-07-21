#ifndef _TIME_OUT_H
#define _TIME_OUT_H

#include "PE_Types.h"
#include "MethodTimer.h"

typedef enum{
  CONTINUO,
  ALTERNATIVE,
  ONETIME
}modoTimeOut;


struct TimeOut{
  struct MethodTimer * _base;
  bool * isTime;
  modoTimeOut modo;
};

void setFlag(void * a);
struct TimeOut* InitTimeOut(word _tiempo,bool * flag,modoTimeOut modo);
void liberarTime(struct TimeOut * t);
void reStartTimeOut(struct TimeOut* to);

#endif