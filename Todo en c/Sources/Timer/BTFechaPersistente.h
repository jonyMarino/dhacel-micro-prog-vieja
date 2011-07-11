#ifndef _BT_FECHA_PERSISTENTE_H
#define _BT_FECHA_PERSISTENTE_H

#include "FechaTiempo.h"
#include "BTFPConf.h"

void BTFechaPersistenteInit(BTFPConf * _conf);

byte BTFechaPersistente_setDate(word anio,byte mes,byte dia);

void salvarFecha(DATEREC * date);
  

#endif
