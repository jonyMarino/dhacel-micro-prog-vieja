//////////////////////////////////////////////////////////////
//
// rutina de escaneo. Actua cuando se libera la tecla. Si UP
// 					  se mantiene presionada un tiempo mayor
//					  a 1 seg, entonces es funcion.
//
// Uso: se debe declarar en main y como globales las variables
//		KeyStt y KeyEdge.
//
// IN:  nothing
// OUT: KeyStt:  tecla presionada, NULL teclas liberadas
//		KeyEdge: tecla presionada. Reset en main para asegurarme
//				 que fue debidamente tomada.
//
//////////////////////////////////////////////////////////////

/* archivos include */
#include "IO_Map.h" 		
#include "PUL.h"
//#include "Timer.h"
#include "Mydefines.h"
#include "PE_Types.h"
#include "teclas.h"
#include "TimerOld.h"
#include "boxes.h"
#include "StaticTimer.h"

#ifdef jony_25_06
/* Posicion de cada tecla*/
#define KEY_NULL		0X00
#define KEY_UP      0x04
#define KEY_DOWN    0x08
#define KEY_RESET   0x10
#define KEY_ENTER   0x02											 
/*  Definicion de nombre de tecla y su forma de ser invocada*/
typedef struct {
  char nombre;
  byte teclas;
}t_key;

const t_key keys[]={
  'u',KEY_UP,
  'd',KEY_DOWN,
  'k',KEY_RESET,
  'c',KEY_UP | KEY_RESET,
  's',KEY_DOWN | KEY_RESET,
  'p',KEY_UP | KEY_DOWN | KEY_RESET,
  't',KEY_UP | KEY_ENTER,
  0,0
};
#endif

/* definiciones usadas en rutina */
#define CANTIDAD_TECLAS 4      /*Cantidad de pulsadores/teclas que se leen del intrumento*/
#define CANTIDAD_VALIDACIONES 8 /*Cantidad de ingresos iguales que hace falta para validar
                                o asegurar que una tecla fue presionado(si es 0 no corrobora)*/
/* definiciones del teclado */
#define KEY_RAMPA_INI	80 /*A los cuantos ingresos(la primera vez) iguales se repite la salida para ese pulsador*/
#define KEY_RAMPA_FIN	0  /*Maxima velociadad de ingresos iguales a los que se repite la salida para ese pulsador*/
#define KEY_DELTA	1  /*Variacion de velociadad una vez realizada la salida*/
																
/* variables externas */
//byte	KeyStt;					// tecla presionada. NULL: teclas liberadas
byte	ValidKey=0;					// tecla presionada. NULL: teclas liberadas
byte	KeyAnt=0;					// estado anterior
byte	KeyRampa=0;				// valor de recarga para ir acelerando
byte	KeyEdge;				  // teclas presionada. Reset en main

byte	KeyCnt=0;					// contador para generar el tiempo entre repeticiones
	// teclas presionada. Reset en main
byte	KeyDeb=0;					// contador para el debounce
byte	SwAct=0;					// arma los bits del switch actual
byte	CntInt=0;					// contador de interrupciones para procesar cada 4
bool  Anterior_F=FALSE;
byte KeySum;				
byte FlagCleaner;

void Switches(byte Pulsador)
{
	 byte i;
/* armo el SwAct cada 4 interrupciones insertando en un registro */

  if(!PUL_GetVal()) SwAct|=Pulsador;

	CntInt++;
	if(CntInt==CANTIDAD_TECLAS){	  
	  CntInt=0;				// reinicia...
		
	  if(SwAct!=ValidKey) // proceso del debounce
	  {
		  KeyDeb=CANTIDAD_VALIDACIONES;
		  ValidKey=SwAct;
	  }
	  SwAct=0;
	  
	  if(KeyDeb!=0) {
	  	--KeyDeb;
	  //	return;		
	  }
	  
	//acaba de terminar la validacion
		
    if (KeyAnt!=ValidKey && KeyDeb==0){

	   #ifdef jony_25_06
	   for(i=0;keys[i].nombre!=0;i++){
	    if(keys[i].teclas==ValidKey){
	     KeyEdge=keys[i].nombre;
	     break;
	    }
	   }
	   #else
	   switch(ValidKey){
		  case 0x08: 
		    KeyEdge='d';
				break;
			case 0x04: 
			  KeyEdge='u';
				break;
			case 0x10: 
			  KeyEdge='k';
        FlagCleaner = 1;
			}
		 #endif
			
	  if(KeyAnt==0x02 && ValidKey==0x00 && !Anterior_F) KeyEdge='r';

	  if(ValidKey==0) Anterior_F=FALSE;
	  flagTimeReturn=FALSE;
	  Timer_Run(TRETURN,&flagTimeReturn, UNICO_SET);
	  KeyAnt = ValidKey;
	  KeyCnt=0;
	  KeyRampa=KEY_RAMPA_INI;
    }
	  
	}
//  else
//  {
	  if(KeyAnt==0) 
	    return;
	//  if(KeyAnt!=ValidKey)return;
	  if(KeyCnt<KeyRampa)
		  KeyCnt++;
	  else{
		  KeyCnt=0;
		  if(KeyRampa>KEY_RAMPA_FIN)
			  KeyRampa-=KEY_DELTA;
		  switch(KeyAnt){
			  case 0x08: KeyEdge='d';
				     break;
			  case 0x04: KeyEdge='u';
				     break;
			  case 0x02: 
			       if(KeyRampa<=KEY_RAMPA_INI*10/11){			       
			        KeyEdge='f';
				      KeyRampa=KEY_RAMPA_INI;
				      Anterior_F=TRUE;
			       }
			}
			flagTimeReturn=FALSE;
		  Timer_Run(TRETURN,&flagTimeReturn, UNICO_SET);
	  }
  	#ifndef jony_25_06
  	SwAct=0;
  	#endif
}

#ifdef jony_25_06
#pragma INLINE
char get_key(void){
char key;  
  Cpu_DisableInt();
  key= KeyEdge;
  KeyEdge =0;
  Cpu_EnableInt();
  
  return key;
}
#endif


