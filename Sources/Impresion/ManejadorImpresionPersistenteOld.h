#ifndef _MANEJADOR_IMPRESION_PERSISTENTE_H
#define _MANEJADOR_IMPRESION_PERSISTENTE_H


#include "ManejadorImpresion.h"
#include "MIPConf.h"


struct ManejadorImpresionPersistente {
    struct ManejadorImpresion super;
    struct MIPConf * conf;
};

extern const struct ManejadorImpresionClass ManejadorImpresionPersistente;


#endif
