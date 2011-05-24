#ifndef _RELOJ_INTERNO_PERSISTENCIA_DS1307_H
#define _RELOJ_INTERNO_PERSISTENCIA_DS1307_H


#include "BaseTiempo.h"
#include "BaseTiempoDS1307.h"

struct RelojInternoPersistenciaDS1307{
  struct BaseTiempo super;  
  struct BaseTiempoDS1307 ds1307;
};

extern const struct BaseTiempoClass RelojInternoPersistenciaDS1307;

/*
** ===================================================================
**     Method      :  RelojInternoPersistenciaDS1307_sincronizar
**     Description :  Sincroniza el valor del reloj interno con el 
**      guardado en el DS1307
** ===================================================================
*/
void RelojInternoPersistenciaDS1307_sincronizar(void *_self);

#endif