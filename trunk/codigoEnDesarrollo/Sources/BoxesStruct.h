#ifndef _BoxesStruct
#define _BoxesStruct
#include "PE_Types.h"
#include "stddef.h"

typedef void (**PunteroF)(void);
#define NO_FUNCTION NULL
/* definicion de la estructura de dato de los Boxes*/         
/*******************************************************/
typedef const struct{
  int * Fdir;							/* Direccion del parámetro a Escribir*/
  byte (*OnWrite)(int,byte);		/* Funcion a ejecutar Luego de la escritura del parámetro*/
  int * Inf,* Sup;			  /* Limites inferior y superior */
  byte chan;
}TDato;
			
/* definicion de boxes con variables de texto no en Rom*/
/*******************************************************/
typedef struct{
		void(*DirProc)(void);		/* funcion que procesa al box*/
		TDato * sDato;             /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
		char * TxtDpy;				  /* texto del titulo */
    const char  **TxtDpy0;
 		int * VdeNavegacion;	/*Variable que determina los proximos estados. if FALSE, depende del primero*/		
		const PunteroF * Teclar; //Proximo estado toque rapido
		PunteroF   Teclaf; //Proximo estado toque mantenido

}Textual;

/* definicion de boxes con variables numericas no en Rom*/
/********************************************************/
typedef struct{
		void(*DirProc)(void);		    /* funcion que procesa al box*/
		TDato * sDato;                 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/	
		char * TxtDpy;				    /* texto del titulo */
		byte Dot;					          /* punto decimal a enc */
		int * VdeNavegacion;			/*Variable que determina los proximos estados. if FALSE, depende del primero*/		
		const PunteroF * Teclar;			//Proximo estado toque rapido
		PunteroF   Teclaf;			//Proximo estado toque mantenido		 	
}Numerico;

/* definicion de boxes con variables numericas no en Rom*/
/********************************************************/
typedef struct{
		void(*DirProc)(void);		    /* funcion que procesa al box*/
		int * Dato;                 /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/	
		char * TxtDpy;				    /* texto del titulo */
		byte Dot;					          /* punto decimal a enc */
		int * VdeNavegacion;			/*Variable que determina los proximos estados. if FALSE, depende del primero*/		
		const PunteroF * Teclar;			//Proximo estado toque rapido
		PunteroF   Teclaf;			//Proximo estado toque mantenido		 	
}NumericoRO;

/* definicion de boxes de titulo */
/*********************************/

typedef struct{
		void(*DirProc)(void);		    /* funcion que procesa el box. DirProc es un puntero a una funcion que no tiene parametros y que regresa void */
		char * TxtDpy;				      /* texto del titulo */
		char * TxtDpySup;			    /* texto del titulo */
		int * VdeNavegacion;			/*Variable que determina los proximos estados. if FALSE, depende del primero*/
		const PunteroF * Teclar;			/* Toque Rapido*/
		PunteroF   Teclaf;			/* Toque Prolongado*/
}Titulo;

/* Puntero a los Boxes*/
typedef union{
	Numerico * Num;
	NumericoRO * NumRO;
	Textual	* Txt;
	Titulo * Tit;	
	}TPtrBox;
///VALORES DE LOS TEXTOS////////////
//////////AUTO SINTONIA/////////////
#define St_OFF 0
#define Stc 1
#define St_Ok 4
#define St_Err 5

//////////TIPO SEGMENTO/////////////
#define			End		0
#define     rMPt  1
#define     rMPV	2								
#define			MSt		3
#define			SteP  4
#define			JMP		5

//////////TIPO TEMPERATURA INICIAL///
#define SEt 0
#define AMb 1	

//////////TIPO DE CONTROL/////////////
#define _onoff 0
#define _pid 1								

///////////TOLERANCIAS//////////////
      
#define	OFF  0
#define	Lo   1
#define	Hi   2
#define	bAnd 3			
#define	no   4

//////////CONDICION DE EMERGENCIA///
#define cc   0
#define rS   1
#define rP   2
#define EndP 3
#define no   4
///////////////////////////////////


///////////VERSION/BLOQUEOS////////
typedef enum{BL1,BL2,BL3,BL4}TVersion;


#endif