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
#include "Mydefines.h"
#include "PE_Types.h"
#include "teclas.h"
#include "TimerOld.h"
#include "boxes.h"
#include "StaticTimer.h"


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

													
/* variables externas */

byte	ValidKey=0;					// tecla presionada. NULL: teclas liberadas
byte	KeyAnt=0;					// estado anterior
byte	KeyRampa=0;				// valor de recarga para ir acelerando
byte	KeyEdge;				  // teclas presionada. Reset en main

byte	KeyCnt=0;					// contador para generar el tiempo entre repeticiones
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
 
	   for(i=0;keys[i].nombre!=0;i++){
	    if(keys[i].teclas==ValidKey){
	     KeyEdge=keys[i].nombre;
	     break;
	    }
	   }
	   
			
	  if(KeyAnt==0x02 && ValidKey==0x00 && !Anterior_F) 
	    KeyEdge='r';

	  if(ValidKey==0) 
	    Anterior_F=FALSE;
	  
	  flagTimeReturn=FALSE;
	  Timer_Run(TRETURN,&flagTimeReturn, UNICO_SET);
	  KeyAnt = ValidKey;
	  KeyCnt=0;
	  KeyRampa=KEY_RAMPA_INI;
    }
	  
	}

	  if(KeyAnt==0) 
	    return;

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
  	
}


char get_key(void){
  char key;  
  Cpu_DisableInt();
  key= KeyEdge;
  KeyEdge =0;
  Cpu_EnableInt();
  
  return key;
}



