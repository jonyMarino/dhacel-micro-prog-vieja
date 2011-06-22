#ifndef _PROGRAMADOR_H
#define _PROGRAMADOR_H
#include "PE_Types.h"
#include "Mydefines.h"

//#ifdef programador


#define MAX_PROGRAMAS 5
#define MAX_SEGMENTOS 24

#define TAMANIO_SEGMENTO 4

typedef struct{
  int tipo_segmento;
  byte condicion_emer;
  byte tipo_tolerancia;
  int temperatura_f;
  word tiempo;  
  int tolerancia;
  } TSegmentos;		                                    //10 bytes(debe ser par)

#ifdef jony_17_08

#define TAMANIO_GENERAL 4 

typedef struct{
  byte condicion_emer;
  byte tipo_tolerancia;
  int temperatura_inicial;
  #ifndef _APARATO_VIEJO
  int tipo_temperatura_inicial;  					   
  #else
  int nada;
  #endif
  int tolerancia;

  }Tinfoprograma;                                     //8 bytes (debe ser par)
#endif
  
typedef struct{
 Tinfoprograma Gral;
 TSegmentos Segmento[MAX_SEGMENTOS];
 byte nada[8]; //debe haber por lo menos un nada declarada para que se ponga en End en el segmento 26
}TProgramas;		 //sizeof(TProgramas)= 256 bytes


#ifdef prog_viejo
#define NO_PROGRAMA 0
#else
#define NO_PROGRAMA -1
#endif

#ifdef prog_viejo
#define OFFSET_SEG1 1
#else
#define OFFSET_SEG1 0
#endif

#define velocidad tiempo
#define ciclos tiempo 

#define sub_programa temperatura_f  

typedef enum {
  PROG_STOPED, 
 PROG_RUNNING 
}t_prog_state;
//Zona que rompe encapsulacion
typedef struct {																						//Esta estructura contiene la info para recuperar el programa luego de un JUMP (Se verifica ante un END)
    byte Programa;																		//(Programa al cual volver)+1, si es cero no se vuelve a ningun programa. Se verifica en End
    byte Segmento;																		//Segmento al cual volver->Es un Jump
    int Ciclos;																			//Ciclos de repeticion del programa. Se ejecuta en End
}TProgramReturn;


struct TProg{
  int programa_actual[CANTIDAD_SAL_CONTROL];				//Programa actualmente corriendo, es distinto de PRom[R_Programa], ya que el tipo de segmento JUMP lo puede modificar
  int segmento_actual[CANTIDAD_SAL_CONTROL];				//Segmento corriendo del programa actual,
  dword segundos_segmento[CANTIDAD_SAL_CONTROL];			//Segundos transcurridos en el segmento actual
  #ifdef jony_25_06
  t_prog_state prog_state[CANTIDAD_SAL_CONTROL];    //Estado del Programa(Stop o Running)
  #endif

  TProgramReturn program_return[CANTIDAD_SAL_CONTROL][MAX_PROGRAMAS];

  int set_point_programa[CANTIDAD_SAL_CONTROL];				//Set Point del Programa actual
  int temperatura_anterior[CANTIDAD_SAL_CONTROL];		//Temperatura que habia al comenzar el segmento

};

extern struct TProg progToSave;

extern const TProgramas  Programa[MAX_PROGRAMAS];
#define SetPointPrograma progToSave.set_point_programa 				
#define SegundosSegmento progToSave.segundos_segmento 		
#define ProgramaActual progToSave.programa_actual 				
#define SegmentoActual progToSave.segmento_actual 
extern bool Prog_actualizar;
extern int SegmentoMostrado[CANTIDAD_SAL_CONTROL];
extern int TiempoEnMinutos[CANTIDAD_SAL_CONTROL];
//Fin
#ifdef jony_25_06
void Prog_ActualizarSetPoints(void);
#endif

#ifdef jony_17_08
const TSegmentos * get_ActualSegment(byte channel);
#endif

void Prog_Init(void);

bool isProgFinish(byte chan);

byte get_Lim_Seg(byte programa);

void ActualizarSetPointPrograma(byte canal);

void runPrograma(
byte canal,
byte programa, 
byte Segmento
#ifdef jony_15_08
,long segs
#endif
);

void ReestablecerPrograma(byte canal);


//////////////////// Referente a Estado del Programa ////////////////

#ifdef jony_25_06
void Prog_Stop(void);
void Prog_Continue(void);
void Prog_reset(void);
#endif

#ifdef jony_08_07
t_prog_state get_ProgState(byte canal);
#endif


//////////////////////////////////////////////////////////////////////



#define get_SP_Programa(chan) SetPointPrograma[chan]

void GuardarPrograma(void); 

#endif