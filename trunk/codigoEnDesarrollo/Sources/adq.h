#ifndef ADQ_H
#define ADQ_H

#define ADQ_SI                1
#define ADQ_NO               -2
#define ADQ_FULL             -3
#define BLANCO                0xFFFF
#define MAX_CANTIDAD_VALORES  4

typedef struct{
  int * valAdq;
  byte chan;
}datoAdq;

byte adquirirValor(word index,int val);
byte BorrarPagAdq(char);
byte BorrarTodoAdq(int a,byte chan);
byte bajarTodosDatosAdq(datoAdq dat[],byte cantidad);
void bajarDatoAdq(word index);
long buscarPosicionVacia(void);
int adquirirValorAut(datoAdq dat[],byte cantidad);
byte setflagFechaHora(int a,byte chan);
//void calculoInternoFechaYHora(word *anio,word *mes,word *dia,word *hora,word *min,word *seg);
void calculoInternoFechaYHora();
void convertirString (char str[], int val, char chan);
int guardarFechaYhora(word *anio,word *mes,word *dia,word *hora,word *min,word *seg);

#endif