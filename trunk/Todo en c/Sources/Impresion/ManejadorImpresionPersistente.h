#ifndef _MANEJADOR_IMPRESION_PERSISTENTE_H
#define _MANEJADOR_IMPRESION_PERSISTENTE_H


#include "ManejadorImpresion.h"
#include "MIPConf.h"
#include "MethodTimer.h"


struct ManejadorImpresionPersistente {
    struct ManejadorImpresion super;
    struct MIPConf * conf;
    struct MethodTimer mt;
};

extern const struct ManejadorImpresionClass ManejadorImpresionPersistente;


#endif
