#ifndef _MANEJADOR_IMPRESION_PROTECTED_H
#define _MANEJADOR_IMPRESION_PROTECTED_H

int ManejadorImpresion_getCuentaParcial(void * _self);

void ManejadorImpresion_print(void * _self);

/**/
int ManejadorImpresion_getIntervalo(void * _self);

/**/
void ManejadorImpresion_setIntervalo(void * _self,int intervalo);

/**/
bool ManejadorImpresion_getHabilitado(void * _self);

/**/
void ManejadorImpresion_setHabilitado(void * _self, bool habilitar);

#endif