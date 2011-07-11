#ifndef INTEGRADOR_H
#define INTEGRADOR_H

extern long Acumulador[CANTIDAD_CANALES];
extern long valorAmostrar[CANTIDAD_CANALES];
void _sumador(void);
void integrador_reset(void);
void integrador_decimales(void);

#endif