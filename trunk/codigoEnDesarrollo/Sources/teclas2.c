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
/* definiciones del teclado */
#define KeyRampaIni	80
#define KeyRampaFin	16
#define KeyDelta	8
#define NoR	1		// Set cuando se activo por tiempo ('f') y no debe el flanco ('r')
  
/* definiciones usadas en rutina */
#define _NO_KEY		0x0F
    
/* variables externas */
byte	KeyStt;					// tecla presionada. NULL: teclas liberadas
byte	KeyAnt;					// estado anterior
byte	KeyRampa;				// valor de recarga para ir acelerando
byte	KeyCnt;					// contador para generar el tiempo entre repeticiones
extern	byte	KeyEdge;				// teclas presionada. Reset en main
byte	KeyDeb;					// contador para el debounce
byte	SwAct;					// arma los bits del switch actual
extern	word	CNT1_RTI;				// usado en demora de 10 seg. en la navegacion  
byte FlagCleaner;
//extern byte flagalar[10];

byte	CntInt;					// contador de interrupciones para procesar cada 4
byte	KeyFlags;				// flags indicadores. ver detalle en main	

void Switches(void)
{

/* armo el SwAct cada 4 interrupciones rotando un registro */

  if(PUL_GetVal()) SwAct|=0x10;
  else SwAct&=0xEF;

	SwAct>>=1;				// rota un lugar
	CntInt--;
	if(CntInt==0){			// todo el proceso
		CntInt=4;				// reinicia...
	
		if(SwAct!=KeyAnt){		// proceso del debounce
			KeyDeb=5;
			KeyAnt=SwAct;
			}
		if(KeyDeb!=0){			// posible proceso de edge
			KeyDeb--;
			if(KeyDeb==0){			// acaba de terminar el debounce
			if((SwAct!=_NO_KEY)||(KeyStt==0x0d)){
				KeyEdge= (KeyStt ^ SwAct) & (KeyStt ^ 2);
				if(KeyEdge==0x08)KeyEdge='d';
				if(KeyEdge==0x04)KeyEdge='u';
				if((KeyEdge==0x02)&&(!(KeyFlags & NoR)))
				{KeyEdge='r';KeyFlags&=0xfe;}
				if(KeyEdge==0x01){
				KeyEdge='k';
        FlagCleaner=1;
				}

				}
			if (KeyStt==_NO_KEY)KeyFlags&=0xfe;	// reset el flag de tecla larga
			if(KeyStt!=SwAct)KeyCnt=KeyRampaIni;
			KeyStt=SwAct;
			CNT1_RTI=10000;					// contador de 10 segundos	
	
			}
		}
	
	/* proceso que genera la rampa de repeticiones */
//	if(KeyEdge==' '){
	if(KeyCnt>KeyRampaFin)KeyCnt--;
	else{
	KeyCnt=KeyRampa;
	if(KeyStt==_NO_KEY){if(KeyRampa<KeyRampaIni)KeyRampa+=KeyDelta;}
	else{				if(KeyRampa>KeyRampaFin)KeyRampa-=KeyDelta;
			if(KeyStt==0x07)KeyEdge='d';
			if(KeyStt==0x0b)KeyEdge='u';
			if((KeyStt==0x0d)&&
				(KeyRampa<KeyRampaIni*3/4)){
					KeyEdge='f';
					KeyRampa=KeyRampaIni;
					KeyFlags|=NoR;
					}
//			if(KeyStt==0x0e)KeyEdge='r';
			CNT1_RTI=10000;					// contador de 10 segundos				
//		}
		}
		}
	}
         
}  
