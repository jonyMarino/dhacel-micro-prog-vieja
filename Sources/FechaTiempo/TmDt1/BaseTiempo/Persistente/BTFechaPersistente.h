#ifndef _BT_FECHA_PERSISTENTE_H
#define _BT_FECHA_PERSISTENTE_H


#include "BaseTiempo.h"
#include "BTFPConf.h"


struct BTFechaPersistente {
  struct BaseTiempo super;
  struct BTFPConf * conf;
};

extern const struct BaseTiempoClass BTFechaPersistente;


#endif
