#ifndef _HUMEDAD_H
#define _HUMEDAD_H

typedef struct {         //coordenadas para entrar en la tabla de humedad
  unsigned int x;                //diferencia de temp entre bulbo seco y humedo
  unsigned int y;                //temperatura del bulbo seco
}coordenadas;

extern unsigned int Humedad;

char buscarEnTabla (int x, char longTabla, int * tabla);
coordenadas buscarCordenadas ( int xM, int yM);
unsigned int interpolacionDoble (void);

#endif