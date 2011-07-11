/* archivos include */
#include <string.h>
#include "display.h"
#include "PE_Error.h"
#include "PWM.h"
#include "TmDt1.h"
#include "IFsh10.h"
#include "cnfbox.h"
#include "Programador.h"
//#include "Timer.h"
#include "Sensores.h"
#include "boxes.h"
#include "boxesprog.h"
#ifdef jony_28_06
#include "boxesset.h"
#endif
#include "PTSL.h"
#include "paramdefines.h"
#include "IFsh10.h"
#include "cnfbox.h"
#include "FuncionVF.h"
#include "vfboxes.h"
#include "hold.h"
#include "PE_types.h"
#include "TimerOld.h"
#include "Math.h"
#include "boxescal.h"
#ifdef _PRINTER
  #include "boxesPrn.h"
#endif
#ifdef ADQ
 #include "BTFPConf.h"
#endif
#ifdef INTEGRADOR
#include "Integrador.h"
#endif
#ifdef ADQ
#include "adq.h"
#endif
#include "Mmax.h"
#include "Parametros.h"
#include "Medicion.h"
#include "valorControl.h"

/////////////LEDS//////////////////////
extern bool Date_EnUser;
///////////////////////////////////

#ifdef ADQ
extern int stateAdq;
#endif

#pragma CODE_SEG DEFAULT
#ifdef pirani
static const char UF[]="UF  ";
static const char OF[]="OPEn  ";
#else
static const char UF[]=" UF ";
static const char OF[]="OPEn";
#endif
#pragma DATA_SEG DEFAULT

#define MAX_MAIN_TEXT 25

static char main_text[MAX_MAIN_TEXT]="St  ";

bool flagpote=0;

static bool show_main_text;	                                    // Se pone en uno al poner texto en maintext
																																// hay que ponerlo en cero a mano
static char prueba[20]="MEnSAJE PruEbA    ";

extern volatile const struct BTFPConf bTConf;
extern struct BTFechaPersistente baseTiempo;
extern volatile const struct MIPConf confMI;
extern struct ManejadorImpresionPersistente mi;


char is_box_principal=0;
bool presentar;

#pragma CONST_SEG DEFAULT
//extern const byte Pot_2[];
//extern const int div_dec[];
extern int dutytmp;
extern PunteroF PtrTmp;
            
 //PunteroF PtrDsp=&DSP.DirProc;

extern byte b,d;
extern unsigned int cont;
extern bool ADfinish;
extern int bufferSensor[CANTIDAD_CANALES];
byte Tecla;
extern bool Flag1;
extern dword St_Counter[CANTIDAD_SAL_CONTROL+CANTIDAD_SAL_ALARMA];
extern byte St_Step[CANTIDAD_SAL_CONTROL+CANTIDAD_SAL_ALARMA];
extern int duty_cont_ch[4];
extern int duty_alar_ch[4];
extern long suminteg[CANTIDAD_CANALES];
extern long buffer_deriv[CANTIDAD_CANALES];
long auvis1;

extern int ValCont[CANTIDAD_CANALES];
extern long vxalar;
extern bool SaveNow;
//extern byte flagalar[10];
#ifdef SENS_HUMEDAD
extern unsigned int Humedad;
#endif
#ifndef programador
extern long tempAct;
#endif

/* variables de uso temporal por los boxes cuando se activan */
int Valor_Tmp;				// Buffer del parametro procesado
int bufferNro;
TDato *buffersDato;
TPtrBox PtrBox; //puntero a los tipos de boxes manipulados
bool save_parametros=FALSE;	/* Ordena salvar el valor en la ram para copy a eeprom cuando set*/
bool save;
bool FstTime;	/* set cuando se ejecuto al menos una vez el procesador */
#ifdef programador
static enum {NOPROGRAMA,PROGRAMA}BoxAnterior; // Para saber desde donde se entro al BOx de Segmento
#endif

/* Drivers para simular el hardware */
/************************************/
extern byte DotNum[CANTIDAD_DISPLAYS];			/* punto decimal a encender */


//////////////Funciones internas//////////////////////
void Exit(void);					 // Salir a pantalla principal
void Salir_num(void);	 // Salir por toque rapido o sostenido en pantalla numérica


/*void detectoVersionPote (void){
  version = 1;        // bandera para detectar el paso por el box de version
  void TxtHandler(void);
}
*/
byte Escribir(TDato * sDato,int valor){
  byte err;
  
  if(*(sDato->Inf)>valor || *(sDato->Sup)<valor) return ERR_VALUE;  /* Checkeo Rangos*/
  if(sDato->Fdir!=NULL){																					  /*Escribo*/
    if((word)sDato->Fdir<RAM_END && (word)sDato->Fdir>=RAM_START)//RAM
      *(sDato->Fdir)=valor;
    else if((word)sDato->Fdir>=FLASH_PARAMETROS_START){    //ROM
      err=EscribirWord((word)sDato->Fdir,valor);
      if(err)
        return err; 
    }
    else return ERR_RANGE; 
  }
  if (sDato->OnWrite==NULL)
    return ERR_OK;
  return (*(sDato->OnWrite))(valor, sDato->chan);		/*Ejecuto Accion*/
};

/*escribe siempre*/
  byte EscribirSiempre(TDato * sDato,int valor){
  byte err;
  
  if(*(sDato->Inf)>valor || *(sDato->Sup)<valor) return ERR_VALUE;  /* Checkeo Rangos*/
  if(sDato->Fdir!=NULL){																					  /*Escribo*/
    if((word)sDato->Fdir<RAM_END && (word)sDato->Fdir>=RAM_START)//RAM
      *(sDato->Fdir)=valor;
    else if((word)sDato->Fdir>=FLASH_PARAMETROS_START){    //ROM
      err=EscribirWordSiempre((word)sDato->Fdir,valor);
      if(err)
        return err; 
    }
    else return ERR_RANGE; 
  }
  if (sDato->OnWrite==NULL)
    return ERR_OK;
  return (*(sDato->OnWrite))(valor, sDato->chan);		/*Ejecuto Accion*/
};

/*************Inicializacion para empezar a usar los Boxes*********/ 
void Boxes_Init(void){
  FstTime=TRUE;
}

/* Procesadores de los boxes */
/*****************************/

 
/* maneja titulos: carga buffer de
   los display sup.e inf., con los
   textos de los boxes */
         
void TitleHandler(void){
		
	 is_box_principal=2;
// Primer Ingreso
	if (FstTime){					// es la primera vez que ingresa??
		FstTime=FALSE;			// sacar primera vez
		PtrBox.Tit=(Titulo*)PtrTmp;	 // Poner el puntero PtrBox.Tit con el valor del Box actual
		PasarASCII(&PtrBox.Tit->TxtDpy[0],1);    //mostrar display inferior
		PasarASCII(&PtrBox.Tit->TxtDpySup[0],0); //mostrar display superior	
//	  Tecla=' ';														 //Borrar Tecla presionada
		};
		
//T0QUE RÁPIDO  O MANTENIDO

	if ((Tecla=='r'|| Tecla=='f')&& !save_parametros) 				// Se presiono Tecla rapida o mantenida??? y no hay nada para guardar??
	{ PunteroF Proximo;
	  if (Tecla=='r'){
	    if (PtrBox.Tit->VdeNavegacion==NULL)									// Existe variable selectora del proximo estado	 ???
 		  Proximo=(PunteroF)PtrBox.Tit->Teclar; // Cargar proximo Box por toque rapido
	    else 										 //no hay variable selectora
		  Proximo=PtrBox.Tit->Teclar[*(PtrBox.Tit->VdeNavegacion)];	 // Cargar proximo Box por toque rapido
	  }else
		Proximo=PtrBox.Tit->Teclaf;	 // Cargar proximo Box por toque sostenido
		
	if ((Proximo)!=NULL)		 // Se cargo un box existente???
		{ 
		ResetScroll();
		PtrTmp=Proximo;				 //Cargar puntero a la funcion correspondiente (TitleHandler, NumHandler, etc.)
		FstTime=TRUE;					 //Setear Primera vez
		};
			
			
//	Tecla=' ';						 //Borrar Tecla presionada
			
	};

// EXIT 

	if (Tecla== 'k'){
	 PasarASCII("    ",1);   //borro la pantalla una ves 
	 ResetScroll(); 
	 Exit();	 // Boton de Exit
	}

}



/* Procesador de los boxes de Estado*/
/************************************/

void EstadoHandler (void)
{
   is_box_principal=2;
// Primer Ingreso
if (FstTime){													 // es la primera vez que ingresa?? 
		FstTime=FALSE;										 // sacar primera vez
		PtrBox.Txt=(Textual*)PtrTmp;		   // Poner el puntero PtrBox.Txt con el valor del Box actual 

		PasarASCII(PtrBox.Txt->TxtDpy,1);     //Mostrar DisplayInf
		PasarASCII(PtrBox.Txt->TxtDpy0[*(PtrBox.Txt->sDato->Fdir)],0);    // Mostrar DisplaySup		 

  };

// T0QUE RÁPIDO Y MANTENIDO

	if (Tecla=='r' || Tecla=='f')		// Se presiono Tecla rapida o mantenida???	
	{ PunteroF Proximo;
	  if (Tecla=='r'){
	    if (PtrBox.Txt->VdeNavegacion==NULL) // Existe variable selectora de los Proximos Estados?? 
 		  Proximo=(PunteroF)PtrBox.Txt->Teclar; // Cargar proximo Box por toque rapido
		  else 
		  Proximo=PtrBox.Txt->Teclar[*(PtrBox.Txt->VdeNavegacion)];	 // Cargar proximo Box por toque sostenido
		}else  Proximo=PtrBox.Txt->Teclaf;  // Cargar proximo Box por toque sostenido
		
		if ((Proximo)!=NULL)  // Se cargo un box existente???
		{ 										
		ResetScroll();
		PtrTmp=Proximo;				//Cargar puntero a la funcion correspondiente (TitleHandler, NumHandler, etc.)
		FstTime=TRUE;					//Setear Primera vez
		};
						
//	Tecla=' ';						//Borrar Tecla presionada
			
	};
		
// EXIT

	if (Tecla== 'k') Exit();  // Boton de Exit
}


/* Procesador de los textos*/
/***************************/

/* de acuerdo a la Tecla que se presiona, se incrementa o decrementa los valores
   de las variables numericas de los boxes  y 
   convierte el valor int a ascii, guardandolos en buffer DigitosSup */

void TxtHandler(void){
// PRIMER INGRESO
 	 is_box_principal=2;
	if (FstTime){					 // es la primera vez que ingresa??
		FstTime=FALSE;			 // sacar primera vez
		save=FALSE; /*Reseteo flag de mandar a grabar*/
		PtrBox.Txt=(Textual*)PtrTmp;	// Poner el puntero PtrBox.Txt con el valor del Box actual	    
	  if(PtrBox.Txt->sDato->Fdir != NULL)
	    Valor_Tmp = *(PtrBox.Txt->sDato->Fdir); // Cargar el valor de la variable correspondiente al Box en el buffer
		else
		  Valor_Tmp=0;
		PasarASCII(PtrBox.Txt->TxtDpy,1);             //Mostrar DisplayInf
		PasarASCII(PtrBox.Txt->TxtDpy0[Valor_Tmp],0);  //Mostrar DisplaySup		 

  }


// KeyEdgeS UP Y DOWN
if (Tecla=='u' || Tecla=='d'){					 // Se Presiono la Tecla UP o Down??
	/* proceso Tecla UP */
	if (Tecla=='u'){												 // Se Presiono la Tecla UP??
		if (++Valor_Tmp>=*(PtrBox.Txt->sDato->Sup))		 // Aumentar Buffer, no esta dentro de los limites???
		Valor_Tmp=*(PtrBox.Txt->sDato->Sup)-1;				 // Poner en el maximo valor
		}
	/* proceso Tecla down */
	else if (Tecla=='d'){										 // Se Presiono la Tecla Down???
		if (Valor_Tmp>0)													 // el Buffer es mayor que 0???
			Valor_Tmp--;														 // Decrementar Buffer
		else Valor_Tmp=0;												 // si no poner Buffer en 0
		
		};
    PasarASCII(PtrBox.Txt->TxtDpy0[Valor_Tmp],0);     // Mostrar DisplaySup

		save=TRUE;																				 // Activar Flag de guardar valor
		//Tecla=' ';																		 // Borrar Tecla presionada
}

// T0QUE RÁPIDO O MANTENIDO

if (Tecla=='r' || Tecla=='f')				
	{PunteroF Proximo;
					
   
  if (Tecla=='r'){
    if (save){
      
    	Escribir(PtrBox.Txt->sDato,Valor_Tmp);																			 
      bufferNro= Valor_Tmp;
  		buffersDato= PtrBox.Num->sDato;
  }
	  if (PtrBox.Txt->VdeNavegacion==NULL) // Existe variable selectora de los Proximos Estados??
 		Proximo=(PunteroF)PtrBox.Txt->Teclar; // Cargar proximo Box por toque rapido
		else 
		Proximo=PtrBox.Txt->Teclar[*(PtrBox.Txt->VdeNavegacion)];	 // Cargar proximo Box por toque sostenido
  }else  Proximo=PtrBox.Txt->Teclaf;  // Cargar proximo Box por toque sostenido
		
		if ((Proximo)!=NULL)  // Se cargo un box existente???
		{ 										
		ResetScroll();
		PtrTmp=Proximo;				//Cargar puntero a la funcion correspondiente (TitleHandler, NumHandler, etc.)
		FstTime=TRUE;					//Setear Primera vez
		};
	
					
	
//	Tecla=' ';						//Borrar Tecla presionada
			
	};

/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') Exit(); // Boton de Exit
}

 /*procesa textos del adquisidor*/
 
#ifdef ADQ 
void TxtHandlerAdqHAbilitacion(void){
// PRIMER INGRESO
 	 is_box_principal=2;
	if (FstTime){					 // es la primera vez que ingresa??
		FstTime=FALSE;			 // sacar primera vez
		save=FALSE; /*Reseteo flag de mandar a grabar*/
		PtrBox.Txt=(Textual*)PtrTmp;	// Poner el puntero PtrBox.Txt con el valor del Box actual	    
	  if(stateAdq == ADQ_FULL){
	    Escribir(PtrBox.Txt->sDato,0);  //me aseguro que al comenzar nuevamente empize en "no"																			 
      bufferNro= 0;
  		buffersDato= PtrBox.Num->sDato; 
	  }
	  if(PtrBox.Txt->sDato->Fdir != NULL){
	    if(stateAdq!= ADQ_FULL)
	      Valor_Tmp = *(PtrBox.Txt->sDato->Fdir); // Cargar el valor de la variable correspondiente al Box en el buffer
	    else
	      Valor_Tmp = 2;
	  }else{
	    if(stateAdq!= ADQ_FULL)
		    Valor_Tmp=0;
	    else
	     Valor_Tmp = 2; 
	  }
		PasarASCII(PtrBox.Txt->TxtDpy,1);             //Mostrar DisplayInf
		PasarASCII(PtrBox.Txt->TxtDpy0[Valor_Tmp],0);  //Mostrar DisplaySup		 

  }


if(stateAdq!= ADQ_FULL){
  
// KeyEdgeS UP Y DOWN
if (Tecla=='u' || Tecla=='d'){					 // Se Presiono la Tecla UP o Down??
	/* proceso Tecla UP */
	if (Tecla=='u'){												 // Se Presiono la Tecla UP??
		if (++Valor_Tmp>=*(PtrBox.Txt->sDato->Sup))		 // Aumentar Buffer, no esta dentro de los limites???
		Valor_Tmp=*(PtrBox.Txt->sDato->Sup)-1;				 // Poner en el maximo valor
		}
	/* proceso Tecla down */
	else if (Tecla=='d'){										 // Se Presiono la Tecla Down???
		if (Valor_Tmp>0)													 // el Buffer es mayor que 0???
			Valor_Tmp--;														 // Decrementar Buffer
		else Valor_Tmp=0;												 // si no poner Buffer en 0
		
		};
    PasarASCII(PtrBox.Txt->TxtDpy0[Valor_Tmp],0);     // Mostrar DisplaySup

		save=TRUE;																				 // Activar Flag de guardar valor
		//Tecla=' ';																		 // Borrar Tecla presionada
}  
}

// T0QUE RÁPIDO O MANTENIDO

if (Tecla=='r' || Tecla=='f')				
	{PunteroF Proximo;
					
   
  if (Tecla=='r'){
    if (save){
      
    	Escribir(PtrBox.Txt->sDato,Valor_Tmp);																			 
      bufferNro= Valor_Tmp;
  		buffersDato= PtrBox.Num->sDato;
  }
	  if (PtrBox.Txt->VdeNavegacion==NULL) // Existe variable selectora de los Proximos Estados??
 		Proximo=(PunteroF)PtrBox.Txt->Teclar; // Cargar proximo Box por toque rapido
		else 
		Proximo=PtrBox.Txt->Teclar[*(PtrBox.Txt->VdeNavegacion)];	 // Cargar proximo Box por toque sostenido
  }else  Proximo=PtrBox.Txt->Teclaf;  // Cargar proximo Box por toque sostenido
		
		if ((Proximo)!=NULL)  // Se cargo un box existente???
		{ 										
		ResetScroll();
		PtrTmp=Proximo;				//Cargar puntero a la funcion correspondiente (TitleHandler, NumHandler, etc.)
		FstTime=TRUE;					//Setear Primera vez
		};
	
					
	
//	Tecla=' ';						//Borrar Tecla presionada
			
	};

/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') Exit(); // Boton de Exit
}
 
 
 
void TxtHandlerAdq(void){
// PRIMER INGRESO
 	 is_box_principal=2;
	if (FstTime){					 // es la primera vez que ingresa??
		FstTime=FALSE;			 // sacar primera vez
		save=FALSE; /*Reseteo flag de mandar a grabar*/
		PtrBox.Txt=(Textual*)PtrTmp;	// Poner el puntero PtrBox.Txt con el valor del Box actual	    
	  if(PtrBox.Txt->sDato->Fdir != NULL)
	    Valor_Tmp = *(PtrBox.Txt->sDato->Fdir); // Cargar el valor de la variable correspondiente al Box en el buffer
		else
		  Valor_Tmp=0;
		PasarASCII(PtrBox.Txt->TxtDpy,1);             //Mostrar DisplayInf
		PasarASCII(PtrBox.Txt->TxtDpy0[Valor_Tmp],0);  //Mostrar DisplaySup		 

  }


// KeyEdgeS UP Y DOWN
if (Tecla=='u' || Tecla=='d'){					 // Se Presiono la Tecla UP o Down??
	/* proceso Tecla UP */
	if (Tecla=='u'){												 // Se Presiono la Tecla UP??
		if (++Valor_Tmp>=*(PtrBox.Txt->sDato->Sup))		 // Aumentar Buffer, no esta dentro de los limites???
		Valor_Tmp=*(PtrBox.Txt->sDato->Sup)-1;				 // Poner en el maximo valor
		}
	/* proceso Tecla down */
	else if (Tecla=='d'){										 // Se Presiono la Tecla Down???
		if (Valor_Tmp>0)													 // el Buffer es mayor que 0???
			Valor_Tmp--;														 // Decrementar Buffer
		else Valor_Tmp=0;												 // si no poner Buffer en 0
		
		}
    PasarASCII(PtrBox.Txt->TxtDpy0[Valor_Tmp],0);     // Mostrar DisplaySup

		save=TRUE;																				 // Activar Flag de guardar valor
		//Tecla=' ';																		 // Borrar Tecla presionada
}

// T0QUE RÁPIDO O MANTENIDO

if (Tecla=='r' || Tecla=='f')				
	{PunteroF Proximo;
					
   
  if (Tecla=='r'){
    if (save){
      
    	//Escribir(PtrBox.Txt->sDato,Valor_Tmp);
    	if (PtrBox.Txt->sDato->OnWrite!=NULL)
       (*(PtrBox.Txt->sDato->OnWrite))(Valor_Tmp, PtrBox.Txt->sDato->chan);		/*Ejecuto Accion*/																			 
      //bufferNro= Valor_Tmp;
  		//buffersDato= PtrBox.Num->sDato;
  }
	  if (PtrBox.Txt->VdeNavegacion==NULL) // Existe variable selectora de los Proximos Estados??
 		Proximo=(PunteroF)PtrBox.Txt->Teclar; // Cargar proximo Box por toque rapido
		else 
		Proximo=PtrBox.Txt->Teclar[*(PtrBox.Txt->VdeNavegacion)];	 // Cargar proximo Box por toque sostenido
  }else  Proximo=PtrBox.Txt->Teclaf;  // Cargar proximo Box por toque sostenido
		
		if ((Proximo)!=NULL)  // Se cargo un box existente???
		{ 										
		ResetScroll();
		PtrTmp=Proximo;				//Cargar puntero a la funcion correspondiente (TitleHandler, NumHandler, etc.)
		FstTime=TRUE;					//Setear Primera vez
		};
	
					
	
//	Tecla=' ';						//Borrar Tecla presionada
			
	};

/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') Exit(); // Boton de Exit
}

#endif

/* Procesador del box de Condicion de emergencia general*/
/********************************************************/
#ifdef programador
void CondEGralHandler(void){
  is_box_principal=2;
  TxtHandler();
  if (Tecla=='r'){
  byte i;
  Tinfoprograma BufferGeneral;		//Contiene los datos a grabar
	 
   if (tipo_tolerancia_gral==no){
      next_tiempo=0; //ir a tolerancia(next box)
      if(condicion_emer_gral==no){
        next_prog=next_tol = 0;  // ir a Condicon de emergencia(next box)
      }else {
          next_prog=next_tol = 1;//ir a segmento     
      }
   }
   else if (condicion_emer_gral==no){
        next_prog=0;	 //ir a Condicon de emergencia(next box)
        next_tiempo=1; //ir a Condicon de emergencia(next box)
   }else{
        next_tiempo=2; //ir a segmento
        next_prog=1;	 //ir a segmento
   }

    //grabar
   
   BufferGeneral.temperatura_inicial = temperatura_inicial;
   BufferGeneral.tipo_tolerancia = (byte)tipo_tolerancia_gral;
   BufferGeneral.tolerancia = tolerancia_gral;
   BufferGeneral.condicion_emer = condicion_emer_gral; 

   #ifdef jony_17_08
   for(i=0;i<TAMANIO_GENERAL;i++)
    EscribirWord((word)(&Programa[programa_ingresado].Gral)+i*2,*((word*)&BufferGeneral+i)); 
	 #endif
	 
   SaveNow=TRUE;

  }  
    
}
#endif

/* Procesador de los boxes numéricos*/
/************************************/
void NumHandler(void){
    is_box_principal=2;
// Primer Ingreso	
	if (FstTime){				// es la primera vez que ingresa??
	   
		  FstTime=FALSE;  // sacar primera vez
			save=FALSE; /*Reseteo flag de mandar a grabar*/
			PtrBox.Num=(Numerico*)PtrTmp;	 // Poner el puntero PtrBox.Num con el valor del Box actual
	    if(PtrBox.Num->sDato->Fdir != NULL){
	      
	      Valor_Tmp = *(PtrBox.Num->sDato->Fdir); // Cargar el valor de la variable correspondiente al Box en el buffer
		     
	    }
		  else
		    Valor_Tmp=0;			
// analizo punto decimal
				 
		if ((PtrBox.Num->Dot)>=DECIMALES_CANAL1) // El punto decimal depende del Sensor??
      DotNum[0]=(byte)PRom[R_Decimales+PtrBox.Num->Dot-DECIMALES_CANAL1];
			else											 // sino
			DotNum[0]=PtrBox.Num->Dot; // Cargar PD que indica el Box		
// mando mensajes
						
		  PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
			Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
	}
	
// KeyEdgeS UP O DOWN
if ((Tecla=='u') || (Tecla=='d')){ // Fue presionada una Tecla UP o Down???
  
	/* proceso Tecla UP */
	if (Tecla=='u'){									 // Fue presionada una Tecla UP ???
		    
		    if (Valor_Tmp<(*PtrBox.Num->sDato->Sup)) Valor_Tmp++; // El Buffer es menor que el LimiteSup?? Si-> Incrementar Buffer
				else Valor_Tmp=(*PtrBox.Num->sDato->Sup);						 // No-> Poner Buffer en maximo valor
			
			//	Escribir(PtrBox.Num->sDato,Valor_Tmp);
	};
	
	/* proceso Tecla down */
	if (Tecla=='d'){		  					      // Fue presionada una Tecla Down???
			  
				if (Valor_Tmp>(*PtrBox.Num->sDato->Inf)) Valor_Tmp--;		// El Buffer es mayor que el LimiteInf?? Si-> Decrementar Buffer
				else Valor_Tmp=(*PtrBox.Num->sDato->Inf);	// No-> Poner Buffer en el minimo valor			
		//Escribir(PtrBox.Num->sDato,Valor_Tmp);	
				
		};
	
	 	Pasar_Numero(Valor_Tmp,0,DotNum[0]); // Mostrar DisplaySup
		save=TRUE;							// Grabar parametros
	//	Tecla=' ';							//Borrar Tecla presionada
}

/////////////////////// T0QUE RÁPIDO //////////////////////////

	if (Tecla=='r')	{
 		
		 
		if (save)							
  		{			    		  
  		  Escribir(PtrBox.Num->sDato,Valor_Tmp);
  		  bufferNro = Valor_Tmp;
  		  buffersDato = PtrBox.Num->sDato;
  		 
  		}
        
  Salir_num();		// funcion de salida
			
	}
		
/////////////////////// TOQUE MANTENIDO ////////////////////////

if (Tecla=='f')
  Salir_num();	 // funcion de salida

/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') Exit();	 // Boton de Exit
}


/*Procesador de los boxes numéricos especial*/
void NumHandlerEsp(void){
     is_box_principal=2;
// Primer Ingreso	
	if (FstTime){				// es la primera vez que ingresa??
	   
		  FstTime=FALSE;  // sacar primera vez
			save=FALSE; /*Reseteo flag de mandar a grabar*/
			PtrBox.Num=(Numerico*)PtrTmp;	 // Poner el puntero PtrBox.Num con el valor del Box actual
	    		
// analizo punto decimal
				 
		if ((PtrBox.Num->Dot)>=DECIMALES_CANAL1) // El punto decimal depende del Sensor??
      DotNum[0]=(byte)PRom[R_Decimales+PtrBox.Num->Dot-DECIMALES_CANAL1];
			else											 // sino
			DotNum[0]=PtrBox.Num->Dot; // Cargar PD que indica el Box		
// mando mensajes
		
		 Valor_Tmp=(-(getTempAmbiente()) + PRom[R_ATA]);
			 
		  
		  PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
			Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
	}
	
	Valor_Tmp=(-(getTempAmbiente()) + PRom[R_ATA]);
			 	  
	PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
	Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
// KeyEdgeS UP O DOWN
/*if ((Tecla=='u') || (Tecla=='d')){ // Fue presionada una Tecla UP o Down???
  
	/* proceso Tecla UP */
	/*if (Tecla=='u'){									 // Fue presionada una Tecla UP ???
		    
		    if (Valor_Tmp<(*PtrBox.Num->sDato->Sup)) Valor_Tmp++; // El Buffer es menor que el LimiteSup?? Si-> Incrementar Buffer
				else Valor_Tmp=(*PtrBox.Num->sDato->Sup);						 // No-> Poner Buffer en maximo valor
			
			//	Escribir(PtrBox.Num->sDato,Valor_Tmp);
	};
	
	/* proceso Tecla down */
	/*if (Tecla=='d'){		  					      // Fue presionada una Tecla Down???
			  
				if (Valor_Tmp>(*PtrBox.Num->sDato->Inf)) Valor_Tmp--;		// El Buffer es mayor que el LimiteInf?? Si-> Decrementar Buffer
				else Valor_Tmp=(*PtrBox.Num->sDato->Inf);	// No-> Poner Buffer en el minimo valor			
		//Escribir(PtrBox.Num->sDato,Valor_Tmp);	
				
		};
	
	 	Pasar_Numero(Valor_Tmp,0,DotNum[0]); // Mostrar DisplaySup
		save=TRUE;							// Grabar parametros
	//	Tecla=' ';							//Borrar Tecla presionada
}   */

/////////////////////// T0QUE RÁPIDO //////////////////////////

	if (Tecla=='r')	{
 		
		 
		//if (save)							
  //		{			    		  
  		//  Escribir(PtrBox.Num->sDato,Valor_Tmp);
  		  		  		  																					 
 // 		}
        
  Salir_num();		// funcion de salida
			
	}
		
/////////////////////// TOQUE MANTENIDO ////////////////////////

if (Tecla=='f')
  
  Salir_num();	 // funcion de salida

/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') Exit();	 // Boton de Exit
}



/* Procesador de los boxes numéricos para el luxometro*/
/************************************/
#ifdef INTEGRADOR
void NumHandlerLux(void){
    is_box_principal=2;
// Primer Ingreso	
	if (FstTime){				// es la primera vez que ingresa??
	   
		  FstTime=FALSE;  // sacar primera vez
			save=FALSE; /*Reseteo flag de mandar a grabar*/
			PtrBox.Num=(Numerico*)PtrTmp;	 // Poner el puntero PtrBox.Num con el valor del Box actual
	    if(PtrBox.Num->sDato->Fdir != NULL){
	      
	      Valor_Tmp = *(PtrBox.Num->sDato->Fdir); // Cargar el valor de la variable correspondiente al Box en el buffer
		     
	    }
		  else
		    Valor_Tmp=0;			
// analizo punto decimal
				 
		if ((PtrBox.Num->Dot)>=DECIMALES_CANAL1) // El punto decimal depende del Sensor??
      DotNum[0]=(byte)PRom[R_Decimales+PtrBox.Num->Dot-DECIMALES_CANAL1];
			else											 // sino
			DotNum[0]=PtrBox.Num->Dot; // Cargar PD que indica el Box		
      
      if(PRom[R_RSL]==1 && DotNum[0]>=1)
       	DotNum[0]= DotNum[0]-1;
      
      if(PRom[R_RSL]==2 && DotNum[0]==1)
       	DotNum[0]= DotNum[0]-1;
      
      if(PRom[R_RSL]==2 && DotNum[0]>=2)
       	DotNum[0]= DotNum[0]-2;
      
       
// mando mensajes
						
		  PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
			Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
	}
	
// KeyEdgeS UP O DOWN
if ((Tecla=='u') || (Tecla=='d')){ // Fue presionada una Tecla UP o Down???
  
	/* proceso Tecla UP */
	if (Tecla=='u'){									 // Fue presionada una Tecla UP ???
		    
		    if (Valor_Tmp<(*PtrBox.Num->sDato->Sup)) Valor_Tmp++; // El Buffer es menor que el LimiteSup?? Si-> Incrementar Buffer
				else Valor_Tmp=(*PtrBox.Num->sDato->Sup);						 // No-> Poner Buffer en maximo valor
			
			//	Escribir(PtrBox.Num->sDato,Valor_Tmp);
	};
	
	/* proceso Tecla down */
	if (Tecla=='d'){		  					      // Fue presionada una Tecla Down???
			  
				if (Valor_Tmp>(*PtrBox.Num->sDato->Inf)) Valor_Tmp--;		// El Buffer es mayor que el LimiteInf?? Si-> Decrementar Buffer
				else                    Valor_Tmp=(*PtrBox.Num->sDato->Inf);	// No-> Poner Buffer en el minimo valor			
		//Escribir(PtrBox.Num->sDato,Valor_Tmp);	
				
		};
	
	 	Pasar_Numero(Valor_Tmp,0,DotNum[0]); // Mostrar DisplaySup
		save=TRUE;							// Grabar parametros
	//	Tecla=' ';							//Borrar Tecla presionada
}

/////////////////////// T0QUE RÁPIDO //////////////////////////

	if (Tecla=='r')	{
 		
		 
		if (save)							
  //		{			    		  
  		  Escribir(PtrBox.Num->sDato,Valor_Tmp);
  		  		  		  																					 */
 // 		}
        
  Salir_num();		// funcion de salida
			
	}
		
/////////////////////// TOQUE MANTENIDO ////////////////////////

if (Tecla=='f')
  Salir_num();	 // funcion de salida

/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') Exit();	 // Boton de Exit
}

#endif

/*funcion que procesa el box numerico LOBOV*/

void NumHandlerLOBOV(void){
    
    is_box_principal=2;
// Primer Ingreso	
	if (FstTime){				// es la primera vez que ingresa??
	   
		  FstTime=FALSE;  // sacar primera vez
			save=FALSE; /*Reseteo flag de mandar a grabar*/
			PtrBox.Num=(Numerico*)PtrTmp;	 // Poner el puntero PtrBox.Num con el valor del Box actual
	    if(PtrBox.Num->sDato->Fdir != NULL){
	      
	      Valor_Tmp = *(PtrBox.Num->sDato->Fdir); // Cargar el valor de la variable correspondiente al Box en el buffer
		     
	    }
		  else
		    Valor_Tmp=0;			
// analizo punto decimal
				 
		if ((PtrBox.Num->Dot)>=DECIMALES_CANAL1) // El punto decimal depende del Sensor??
      DotNum[0]=(byte)PRom[R_Decimales+PtrBox.Num->Dot-DECIMALES_CANAL1];
			else											 // sino
			DotNum[0]=PtrBox.Num->Dot; // Cargar PD que indica el Box		
// mando mensajes
						
		  PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
			Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
	}
	
  if ( getEstatusResult(0)==ERR_OK){  							                        // El REsultado de la linealización fue OK??
    DotNum[0]=(byte)PRom[R_Decimales];													// Pongo punto
      #ifdef pirani
      if(PRom[R_Sensor]==SENSOR_PIR)														// Convierto si es Pirani
      Pasar_Numero_Expo(getValCalculado(0),0,DotNum[0]);
      else
      #endif
      #ifdef HOLD
      mantenerIndicacion ();                 
      #elif defined MMAX
      mantenerMaxIndicacion ();
      #else                                                       
      Pasar_Numero(getValCalculado(0),0,DotNum[0]);						        //Mostrar Display sup, Variable del Proceso
      #endif
  }
  else if (getEstatusResult(0)==ERR_UF)					                          // El Resultado de la linealización fue UF??
    PasarASCII(UF,0);							                              // Mostrar UF
  else if (getEstatusResult(0)==ERR_OF)					                          // El REsultado de la linealización fue OF??
    PasarASCII(OF,0);			

/////////////////////// T0QUE RÁPIDO //////////////////////////

	if (Tecla=='r')	{
        
  Salir_num();		// funcion de salida
			
	};
		
/////////////////////// TOQUE MANTENIDO ////////////////////////

if (Tecla=='f')
  Salir_num();	 // funcion de salida

/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') Exit();	 // Boton de Exit
}

/* Procesador de los boxes numéricos*/
/************************************/
/*Esta funcion realiza la misma tarea que "NumHandler" con la diferncia de que graba el valor aunque este 
sea igual al que ya esta en flahs, esto lo ago por que uso una variable en ram para navegar en los boxes*/
void NumHandlervf(void){
    is_box_principal=2;
// Primer Ingreso	
	if (FstTime){				// es la primera vez que ingresa??
	   
		  FstTime=FALSE;  // sacar primera vez
			save=FALSE; /*Reseteo flag de mandar a grabar*/
			PtrBox.Num=(Numerico*)PtrTmp;	 // Poner el puntero PtrBox.Num con el valor del Box actual
	    if(PtrBox.Num->sDato->Fdir != NULL){
	      
	      Valor_Tmp = *(PtrBox.Num->sDato->Fdir); // Cargar el valor de la variable correspondiente al Box en el buffer
		     
	    }
		  else
		    Valor_Tmp=0;			
// analizo punto decimal
				 
	//	if ((PtrBox.Num->Dot)>=DECIMALES_CANAL1) // El punto decimal depende del Sensor??
      DotNum[0]=(byte)(PtrBox.Num->Dot-DECIMALES_CANAL1);
		//	else											 // sino
		//	DotNum[0]=(byte)PtrBox.Num->Dot; // Cargar PD que indica el Box		
// mando mensajes
						
		  PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
			Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
	}
	
// KeyEdgeS UP O DOWN
if ((Tecla=='u') || (Tecla=='d')){ // Fue presionada una Tecla UP o Down???
  
	/* proceso Tecla UP */
	if (Tecla=='u'){									 // Fue presionada una Tecla UP ???
		    if (Valor_Tmp<(*PtrBox.Num->sDato->Sup)) Valor_Tmp++; // El Buffer es menor que el LimiteSup?? Si-> Incrementar Buffer
				else Valor_Tmp=(*PtrBox.Num->sDato->Sup);						 // No-> Poner Buffer en maximo valor
			
			//	Escribir(PtrBox.Num->sDato,Valor_Tmp);
	};
	
	/* proceso Tecla down */
	if (Tecla=='d'){		  					      // Fue presionada una Tecla Down???
				if (Valor_Tmp>(*PtrBox.Num->sDato->Inf)) Valor_Tmp--;		// El Buffer es mayor que el LimiteInf?? Si-> Decrementar Buffer
				else                    Valor_Tmp=(*PtrBox.Num->sDato->Inf);	// No-> Poner Buffer en el minimo valor			
		//Escribir(PtrBox.Num->sDato,Valor_Tmp);	
				
		};
	
	 	Pasar_Numero(Valor_Tmp,0,DotNum[0]); // Mostrar DisplaySup
		save=TRUE;							// Grabar parametros
	//	Tecla=' ';							//Borrar Tecla presionada
}

/////////////////////// T0QUE RÁPIDO //////////////////////////

	if (Tecla=='r')	{
 		  		  
  		  EscribirSiempre(PtrBox.Num->sDato,Valor_Tmp);
  		  flagCartel=1;
  		  PasarASCII("    ",1);   //borro la pantalla 
  		  
        Salir_num();		// funcion de salida
			
	};
		
/////////////////////// TOQUE MANTENIDO ////////////////////////

if (Tecla=='f'){
  
  EscribirSiempre(PtrBox.Num->sDato,Valor_Tmp);
  
  Salir_num();	 // funcion de salida
}
/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') Exit();	 // Boton de Exit
}

/*******************************************************************************/

void NumHandlervfPrincipal(void){
    extern int crearProg;
    extern char CantEtapas;
    extern int nroProgEnAccion;
    
    is_box_principal=1;
// Primer Ingreso	
	if (FstTime){				// es la primera vez que ingresa??
	   
		  FstTime=FALSE;  // sacar primera vez
		  
		  crearProg=0;
			save=FALSE; /*Reseteo flag de mandar a grabar*/
			PtrBox.Num=(Numerico*)PtrTmp;	 // Poner el puntero PtrBox.Num con el valor del Box actual
	    if(PtrBox.Num->sDato->Fdir != NULL){
	      
	      Valor_Tmp = *(PtrBox.Num->sDato->Fdir); // Cargar el valor de la variable correspondiente al Box en el buffer
		     
	    }
		  else
		    Valor_Tmp=0;			
// analizo punto decimal
				 
	//	if ((PtrBox.Num->Dot)>=DECIMALES_CANAL1) // El punto decimal depende del Sensor??
      DotNum[0]=(byte)(PtrBox.Num->Dot-DECIMALES_CANAL1);
		//	else											 // sino
		//	DotNum[0]=(byte)PtrBox.Num->Dot; // Cargar PD que indica el Box		
// mando mensajes
						
		  PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
			Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
	}
	
// KeyEdgeS UP O DOWN
if ((Tecla=='u') || (Tecla=='d')){ // Fue presionada una Tecla UP o Down???
  
	/* proceso Tecla UP */
	if (Tecla=='u'){									 // Fue presionada una Tecla UP ???
		    if (Valor_Tmp<(*PtrBox.Num->sDato->Sup)) Valor_Tmp++; // El Buffer es menor que el LimiteSup?? Si-> Incrementar Buffer
				else Valor_Tmp=(*PtrBox.Num->sDato->Sup);						 // No-> Poner Buffer en maximo valor
			
			//	Escribir(PtrBox.Num->sDato,Valor_Tmp);
	};
	
	/* proceso Tecla down */
	if (Tecla=='d'){		  					      // Fue presionada una Tecla Down???
				if (Valor_Tmp>(*PtrBox.Num->sDato->Inf)) Valor_Tmp--;		// El Buffer es mayor que el LimiteInf?? Si-> Decrementar Buffer
				else                    Valor_Tmp=(*PtrBox.Num->sDato->Inf);	// No-> Poner Buffer en el minimo valor			
		//Escribir(PtrBox.Num->sDato,Valor_Tmp);	
				
		};
	
	 	Pasar_Numero(Valor_Tmp,0,DotNum[0]); // Mostrar DisplaySup
		save=TRUE;							// Grabar parametros
	//	Tecla=' ';							//Borrar Tecla presionada
}

/////////////////////// T0QUE RÁPIDO //////////////////////////

	if (Tecla=='r')	{
 		  		  
  		  EscribirSiempre(PtrBox.Num->sDato,Valor_Tmp);
  		  flagCartel=1;
  		  PasarASCII("    ",1);   //borro la pantalla 
  		  CantEtapas=PRomVF[nroProgEnAccion];
        Salir_num();		// funcion de salida
			
	};
		
/////////////////////// TOQUE MANTENIDO ////////////////////////

if (Tecla=='f'){
  
  EscribirSiempre(PtrBox.Num->sDato,Valor_Tmp);
  
  Salir_num();	 // funcion de salida
}
/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') Exit();	 // Boton de Exit
}



/* Procesador de los boxes numéricos*/
/************************************/

void NumHandlervfInicial(void){
    
    
    extern bool DSave;
    static bool unaVez = FALSE;
    static bool fTimer = FALSE;
    
    is_box_principal=2;
    
   
 if(!unaVez){
    unaVez=TRUE;
    #ifndef HD90
    PasarASCII(tipoEquipo,1);
    #endif
    PasarASCII(numver,0);
    
	  Timer_Run(7000,&fTimer, UNICO_SET);
	  
  }
  
  if(!fTimer)    // espero que termine el cartel inicial
    return;
  

// Primer Ingreso	
	if (FstTime){				// es la primera vez que ingresa??
	   
		  FstTime=FALSE;  // sacar primera vez
			save=FALSE; /*Reseteo flag de mandar a grabar*/
			nroProgEnAccion=0;
			PtrBox.Num=(Numerico*)PtrTmp;	 // Poner el puntero PtrBox.Num con el valor del Box actual
	    if(PtrBox.Num->sDato->Fdir != NULL){
	      
	      Valor_Tmp = *(PtrBox.Num->sDato->Fdir); // Cargar el valor de la variable correspondiente al Box en el buffer
		     
	    }
		  else
		    Valor_Tmp=0;			
// analizo punto decimal
				 
		if ((PtrBox.Num->Dot)>=DECIMALES_CANAL1) // El punto decimal depende del Sensor??
      DotNum[0]=(byte)PRom[R_Decimales+PtrBox.Num->Dot-DECIMALES_CANAL1];
			else											 // sino
			DotNum[0]=PtrBox.Num->Dot; // Cargar PD que indica el Box		
// mando mensajes
						
		  PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
			Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
	    SaveNow=TRUE;
	}
	
  if (getEstatusResult(0)==ERR_OK){  							                        // El REsultado de la linealización fue OK??
    DotNum[0]=(byte)PRom[R_Decimales];													// Pongo punto
      #ifdef pirani
      if(PRom[R_Sensor]==SENSOR_PIR)														// Convierto si es Pirani
      Pasar_Numero_Expo(getValCalculado(0),0,DotNum[0]);
      else
      #endif
      #ifdef HOLD
      mantenerIndicacion ();                 
      #else                                                       
      Pasar_Numero(getValCalculado(0),0,DotNum[0]);						        //Mostrar Display sup, Variable del Proceso
      #endif
  }
  else if (getEstatusResult(0)==ERR_UF)					                          // El Resultado de la linealización fue UF??
    PasarASCII(UF,0);							                              // Mostrar UF
  else if (getEstatusResult(0)==ERR_OF)					                          // El REsultado de la linealización fue OF??
    PasarASCII(OF,0);			

/////////////////////// T0QUE RÁPIDO //////////////////////////

	if (Tecla=='r')	{
        
  Salir_num();		// funcion de salida
			
	};
		
/////////////////////// TOQUE MANTENIDO ////////////////////////

#if(defined(DH102) && !defined(nico_1_10))
  if (Tecla=='t')if(!save_parametros) Salir_num();else SaveNow=DSave=TRUE;

#else
   if (Tecla=='f')if(!save_parametros) Salir_num();else SaveNow=DSave=TRUE;

#endif


/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') Exit();	 // Boton de Exit
}






/* Procesador de los boxes con pantalla exponencial en pirani*/
/*************************************************************/
#ifdef pirani


void NumExpoHandler(void){
// Primer Ingreso	
	if (FstTime){				// es la primera vez que ingresa??
	
		  FstTime=FALSE;  // sacar primera vez
			save=FALSE; /*Reseteo flag de mandar a grabar*/
			PtrBox.Num=(Numerico*)PtrTmp;	 // Poner el puntero PtrBox.Num con el valor del Box actual
	    if(PtrBox.Num->sDato->Fdir != NULL)
	      Valor_Tmp = *(PtrBox.Num->sDato->Fdir); // Cargar el valor de la variable correspondiente al Box en el buffer
		  else
		    Valor_Tmp=0;			
// analizo punto decimal
				 
		if ((PtrBox.Num->Dot)>=DECIMALES_CANAL1) // El punto decimal depende del Sensor??
      DotNum[0]=(byte)PRom[R_Decimales+PtrBox.Num->Dot-DECIMALES_CANAL1];
			else											 // sino
			DotNum[0]=PtrBox.Num->Dot; // Cargar PD que indica el Box		
// mando mensajes
						
		PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf

    #ifdef  pirani
    if(PRom[R_Sensor]==SENSOR_PIR)
      Pasar_Numero_Expo(Valor_Tmp,0,DotNum[0]);
    else
      Pasar_Numero(Valor_Tmp,0,DotNum[0]);						 //Mostrar Variable del Proceso
		#else
      Pasar_Numero(Valor_Tmp,0,DotNum[0]);						 //Mostrar Variable del Proceso
		#endif			
	}
																		
// KeyEdgeS UP O DOWN
if ((Tecla=='u') || (Tecla=='d')){ // Fue presionada una Tecla UP o Down???
int sumador,div;  
    
  #ifdef pirani

	if(PRom[R_Sensor]==SENSOR_PIR){
  for(div=1;(Valor_Tmp/div)>0;div*=10);
  if(div>1)
    div/=10;
  if(div>1)
    div/=10;
  if(((Valor_Tmp)/(div*10))==1 && ((Valor_Tmp)%(div*10))==0 && (div>=10))
    div/=10;
  sumador=div;
	} else
	sumador = 1;





  #endif
  
	/* proceso Tecla UP */
	if (Tecla=='u'){									 // Fue presionada una Tecla UP ???
		    if (Valor_Tmp+sumador<(*PtrBox.Num->sDato->Sup)) Valor_Tmp+=sumador; // El Buffer es menor que el LimiteSup?? Si-> Incrementar Buffer
				else Valor_Tmp=(*PtrBox.Num->sDato->Sup);						 // No-> Poner Buffer en maximo valor
				
	};
	
	/* proceso Tecla down */
	if (Tecla=='d'){		  					      // Fue presionada una Tecla Down???
				if (Valor_Tmp-sumador>(*PtrBox.Num->sDato->Inf)) Valor_Tmp-=sumador;		// El Buffer es mayor que el LimiteInf?? Si-> Decrementar Buffer
				else                    Valor_Tmp=(*PtrBox.Num->sDato->Inf);	// No-> Poner Buffer en el minimo valor			
				
		};
	
	 	if(PRom[R_Sensor]==SENSOR_PIR)
      Pasar_Numero_Expo(Valor_Tmp,0,DotNum[0]);
    else
      Pasar_Numero(Valor_Tmp,0,DotNum[0]);						 //Mostrar Variable del Proceso
		save=TRUE;							// Grabar parametros
	//	Tecla=' ';							//Borrar Tecla presionada
}

/////////////////////// T0QUE RÁPIDO //////////////////////////

	if (Tecla=='r')	{
		if (save)								    		  
  		  Escribir(PtrBox.Num->sDato,Valor_Tmp);
  Salir_num();		// funcion de salida			
	};
		
/////////////////////// TOQUE MANTENIDO ////////////////////////

if (Tecla=='f')
  Salir_num();	 // funcion de salida

/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') Exit();	 // Boton de Exit
}
#endif
/* Procesador de los boxes de tiempo del Segmento corriendo*/
/***********************************************************/
#ifdef programador
void TieSegHandler(void){
bool Fst=FALSE;
byte chan;
TSegmentos * pSeg;
int tipo_seg;

  if (FstTime) Fst=TRUE;
  if(Tecla=='u' || Tecla=='d' || Fst){
		#ifdef jony_15_08
    chan = (((Numerico*)PtrTmp)->sDato)->chan;
    
    if(SegmentoMostrado[chan]>Lim_Segmento2[chan])
      Tecla='r'; //Exit
    pSeg = (TSegmentos *)get_ActualSegment(chan);
    tipo_seg= pSeg->tipo_segmento;
    if(tipo_seg == rMPt || tipo_seg ==MSt || tipo_seg==SteP  )    
      Lim_TieSeg[chan]=pSeg->tiempo;
    else if(tipo_seg ==rMPV){
        Lim_TieSeg[chan]=(pSeg->temperatura_f- (pSeg-1)->temperatura_f)/pSeg->tiempo; // delta temperatura dividido la velocidad  
     
    }
    #endif
      
  }
  NumHandler();
  if (Fst){
    Valor_Tmp= SegundosSegmento[chan]/60;
    Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
  }
}
#endif
/* Procesador de los boxes de Ancho de banda/histeresis*/
/*******************************************************/

void ABHandler(void){
bool Fst=FALSE;
char new_text[5]="H  1";  
  
  if (FstTime) Fst=TRUE;
  NumHandler();
  if(Fst || Tecla=='u' || Tecla=='d'){
    if(Valor_Tmp<0){
    new_text[3]=PtrBox.Num->TxtDpy[3];
    PasarASCII(new_text,1);     //Mostrar DisplayInf  
    } else
      PasarASCII(PtrBox.Num->TxtDpy,1);    
  }
}

void ABAHandler(void){
bool Fst=FALSE;
char new_text[5]="HA 1";  
  
  if (FstTime) Fst=TRUE;
  NumHandler();
  if(Fst || Tecla=='u' || Tecla=='d'){
    if(Valor_Tmp<0){
      new_text[3]=PtrBox.Num->TxtDpy[3];
      PasarASCII(new_text,1);     //Mostrar DisplayInf  
    } else
      PasarASCII(PtrBox.Num->TxtDpy,1);    
  }
}

/* Procesador del box de Codigo*/
/*******************************/
void CodHandler(void){
  NumHandler();
  #ifdef nico_1_10
  if(Tecla=='r'){
    if (!(Valor_Tmp==PRom[R_SetC] || Valor_Tmp==2602))
        PtrTmp=&Principal1.DirProc;  
  }
  #else
  if(Tecla=='f'){
    if (!(Valor_Tmp==PRom[R_SetC] || Valor_Tmp==2602))
        PtrTmp=&Principal1.DirProc;  
  }
  #endif
}

#ifdef programador
/* Procesador del box de elección de programa (Utiliza NumHandler)*/
/******************************************************************/
void ProgramaHandler(void){

  NumHandler();
  if (Tecla=='r'){		
    for(Lim_Segmento1=0;Programa[programa_ingresado].Segmento[Lim_Segmento1].tipo_segmento!=End;Lim_Segmento1++);
    segmento_ingresado=0;
    BoxAnterior =  PROGRAMA;
  }  
}
/* Procesador del box de elección de segmento (Utiliza NumHandler)*/
/******************************************************************/
void SegmentoHandler(void){
extern const PunteroF R_Seg[];
if(FstTime){
byte i;
const TSegmentos * dir;
char end[]="End ";
TSegmentos BufferSegmento;			//Contiene los datos a grabar
  
  if (BoxAnterior==NOPROGRAMA){
//grabar
    dir = &Programa[programa_ingresado].Segmento[0];
    dir+= segmento_ingresado-1;
    
    BufferSegmento.tipo_segmento= tipo_segmento;
    BufferSegmento.condicion_emer = (byte) condicion_emer;
    BufferSegmento.tipo_tolerancia = (byte)tipo_tolerancia;
    if(tipo_segmento!=MSt)
      BufferSegmento.temperatura_f = temperatura_f;
    else
      BufferSegmento.temperatura_f = (dir-1)->temperatura_f;
    BufferSegmento.tiempo = tiempo; 
    BufferSegmento.tolerancia = tolerancia;

    
  
    for(i=0;i<TAMANIO_SEGMENTO;i++)
      EscribirWord((word)dir+i*2,*((word*)&BufferSegmento.tipo_segmento+i)); 
    
    SaveNow=TRUE;
  
//Actualizar
    if (segmento_ingresado==MAX_SEGMENTOS){    
      PasarASCII(end,0);     //DisplaySup
      Lim_Segmento1=segmento_ingresado=Valor_Tmp=MAX_SEGMENTOS-1;
    }
  
  }else BoxAnterior = NOPROGRAMA;
  
} 
NumHandler();
 if(Tecla=='r'){
  if (!segmento_ingresado){                   //Si es igual a 0, cargo la info de esos boxes
    tipo_temperatura_inicial = SEt;
    temperatura_inicial = Programa[programa_ingresado].Gral.temperatura_inicial;
    tipo_tolerancia_gral = Programa[programa_ingresado].Gral.tipo_tolerancia;
    tolerancia_gral = Programa[programa_ingresado].Gral.tolerancia;
    condicion_emer_gral = Programa[programa_ingresado].Gral.condicion_emer; 
  }else{
      if (Programa[programa_ingresado].Gral.tipo_tolerancia==no){
      next_tiempo=0;                          //ir a tolerancia(next box)
        if(Programa[programa_ingresado].Gral.condicion_emer==no){
        next_prog=next_tol = 0;                // ir a Condicon de emergencia(next box)
        }else {
          next_prog=next_tol = 1;              //ir a segmento     
        }
      }
      else if (Programa[programa_ingresado].Gral.condicion_emer==no){
        next_prog=0;	                            //ir a Condicon de emergencia(next box)
        next_tiempo=1;                            //ir a Condicon de emergencia(next box)
      }else{
        next_tiempo=2;                            //ir a segmento
        next_prog=1;	                            //ir a segmento
      }
  }

  tipo_segmento = Programa[programa_ingresado].Segmento[segmento_ingresado].tipo_segmento;
  temperatura_f = Programa[programa_ingresado].Segmento[segmento_ingresado].temperatura_f;
  tiempo = Programa[programa_ingresado].Segmento[segmento_ingresado].tiempo;
  tolerancia =  Programa[programa_ingresado].Segmento[segmento_ingresado].tolerancia;
  tipo_tolerancia = Programa[programa_ingresado].Segmento[segmento_ingresado].tipo_tolerancia;
  condicion_emer = Programa[programa_ingresado].Segmento[segmento_ingresado].condicion_emer;


#ifndef prog_viejo
  if(!segmento_ingresado)
    PtrTmp= R_Seg[1];
#endif  


  segmento_ingresado++;
  Lim_Segmento1++;

} 
}
#endif

/* Procesador del box de texto de eleccion del tipo de segmento*/
/*************************************************************/ 
#ifdef jony_17_08
void TipoSegHandler(void){
  TxtHandler();
  if(Tecla=='r' && Valor_Tmp==End){
    TSegmentos * dir;
    
    dir = (TSegmentos *) &Programa[programa_ingresado].Segmento[0];
    dir+= segmento_ingresado-1;
    
    EscribirWord((word)&dir->tipo_segmento,End);  
    
    
  }
}
#endif
/* Procesador de los boxes numéricos con refresco de pantalla*/
/*************************************************************/ 
void Num_realtime_Handler(void){
static bool refresh;
// Primer Ingreso
	 is_box_principal=2;
	if (FstTime){				// es la primera vez que ingresa??	
		  FstTime=FALSE;  // sacar primera vez
			PtrBox.NumRO=(NumericoRO*)PtrTmp;	 // Poner el puntero PtrBox.Num con el valor del Box actual

		if ((PtrBox.NumRO->Dot)>=DECIMALES_CANAL1) // El punto decimal depende del Sensor??
      DotNum[0]=(byte)PRom[R_Decimales+PtrBox.Num->Dot-DECIMALES_CANAL1];
			else											 // sino
			DotNum[0]=PtrBox.NumRO->Dot; // Cargar PD que indica el Box		
// mando mensajes
		  PasarASCII(PtrBox.NumRO->TxtDpy,1);     //Mostrar DisplayInf
			refresh=TRUE;
	}
	if(refresh){
	 refresh=FALSE;
	 Timer_Run(REFESH_NUMBER,&refresh,UNICO_SET);
	 Pasar_Numero(*(PtrBox.NumRO->Dato),0,DotNum[0]);
	}
/////////////////////// T0QUE RÁPIDO //////////////////////////

	if (Tecla=='r')  Salir_num();		// funcion de salida
			
		
/////////////////////// TOQUE MANTENIDO ////////////////////////

if (Tecla=='f')	Salir_num();	 // funcion de salida

/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') Exit();	 // Boton de Exit
}

/* Procesador del  ingreso de fechas    */
/****************************************/

#if defined(_PRINTER) || defined(ADQ)
DATEREC date;
void YearHandler(void){
  
   if(FstTime){
    FstTime=FALSE;  // sacar primera vez
		save=FALSE; /*Reseteo flag de mandar a grabar*/
		PtrBox.Num=(Numerico*)PtrTmp;	 // Poner el puntero PtrBox.Num con el valor del Box actual
    getFecha(&date);
		Valor_Tmp= date.Year;
		PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
		Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
  }
  
 // KeyEdgeS UP O DOWN
  if ((Tecla=='u') || (Tecla=='d')){ // Fue presionada una Tecla UP o Down???
  
	/* proceso Tecla UP */
	if (Tecla=='u'){									 // Fue presionada una Tecla UP ???
		    
		    if (Valor_Tmp<(*PtrBox.Num->sDato->Sup)) Valor_Tmp++; // El Buffer es menor que el LimiteSup?? Si-> Incrementar Buffer
				else Valor_Tmp=(*PtrBox.Num->sDato->Sup);						 // No-> Poner Buffer en maximo valor
			
			//	Escribir(PtrBox.Num->sDato,Valor_Tmp);
	};
	
	/* proceso Tecla down */
	if (Tecla=='d'){		  					      // Fue presionada una Tecla Down???
			  
				if (Valor_Tmp>(*PtrBox.Num->sDato->Inf)) Valor_Tmp--;		// El Buffer es mayor que el LimiteInf?? Si-> Decrementar Buffer
				else Valor_Tmp=(*PtrBox.Num->sDato->Inf);	// No-> Poner Buffer en el minimo valor			
		//Escribir(PtrBox.Num->sDato,Valor_Tmp);	
				
		};
	
	 	Pasar_Numero(Valor_Tmp,0,DotNum[0]); // Mostrar DisplaySup
		save=TRUE;							// Grabar parametros
	//	Tecla=' ';							//Borrar Tecla presionada
}

/////////////////////// T0QUE RÁPIDO //////////////////////////  
  if(Tecla=='r'){
    if (save){ 
	       setFecha(Valor_Tmp,date.Month,date.Day);
	       Escribir(PtrBox.Num->sDato,Valor_Tmp);
  		   bufferNro = Valor_Tmp;
  		   buffersDato = PtrBox.Num->sDato;
	       }
	        Salir_num();
    return;
  }
  
  /////////////////////// TOQUE MANTENIDO ////////////////////////

if (Tecla=='f')
  Salir_num();	 // funcion de salida

/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') Exit();	 // Boton de Exit
  
  

}

void MonthHandler(void){
 
 if(FstTime){
    FstTime=FALSE;  // sacar primera vez
		save=FALSE; /*Reseteo flag de mandar a grabar*/
		PtrBox.Num=(Numerico*)PtrTmp;	 // Poner el puntero PtrBox.Num con el valor del Box actual
    Valor_Tmp= date.Month;
		PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
		Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
  }
  
 // KeyEdgeS UP O DOWN
  if ((Tecla=='u') || (Tecla=='d')){ // Fue presionada una Tecla UP o Down???
  
	/* proceso Tecla UP */
	if (Tecla=='u'){									 // Fue presionada una Tecla UP ???
		    
		    if (Valor_Tmp<(*PtrBox.Num->sDato->Sup)) Valor_Tmp++; // El Buffer es menor que el LimiteSup?? Si-> Incrementar Buffer
				else Valor_Tmp=(*PtrBox.Num->sDato->Sup);						 // No-> Poner Buffer en maximo valor
			
			//	Escribir(PtrBox.Num->sDato,Valor_Tmp);
	};
	
	/* proceso Tecla down */
	if (Tecla=='d'){		  					      // Fue presionada una Tecla Down???
			  
				if (Valor_Tmp>(*PtrBox.Num->sDato->Inf)) Valor_Tmp--;		// El Buffer es mayor que el LimiteInf?? Si-> Decrementar Buffer
				else Valor_Tmp=(*PtrBox.Num->sDato->Inf);	// No-> Poner Buffer en el minimo valor			
		//Escribir(PtrBox.Num->sDato,Valor_Tmp);	
				
		};
	
	 	Pasar_Numero(Valor_Tmp,0,DotNum[0]); // Mostrar DisplaySup
		save=TRUE;							// Grabar parametros
	//	Tecla=' ';							//Borrar Tecla presionada
}

/////////////////////// T0QUE RÁPIDO //////////////////////////  
  if(Tecla=='r'){
    if (save){ 
	       setFecha(date.Year,Valor_Tmp,date.Day);
	       Escribir(PtrBox.Num->sDato,Valor_Tmp);
  		   bufferNro = Valor_Tmp;
  		   buffersDato = PtrBox.Num->sDato;
	       }
	        Salir_num();
    return;
  }
  
  /////////////////////// TOQUE MANTENIDO ////////////////////////

if (Tecla=='f')
  Salir_num();	 // funcion de salida

/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') Exit();	 // Boton de Exit
  
  
}

void DiaHandler(void){
static byte Max_day;

	
	if (FstTime){
	    
	    int mes = date.Month;
      int anio = date.Year;; 
          
		  FstTime=FALSE;
		  save=FALSE; /*Reseteo flag de mandar a grabar*/
		  Max_day = FechaTiempo_getMaximoDiaDelMes(anio,mes);		
			PtrBox.Num=(Numerico*)PtrTmp;
			
	    //Valor_Tmp=*(PtrBox.Num->sDato->Fdir);
			Valor_Tmp= date.Day;
			DotNum[0]=PtrBox.Num->Dot;		
		  
		  PasarASCII(PtrBox.Num->TxtDpy,1);     //DisplayInf
			Pasar_Numero(Valor_Tmp,0,0);		           //DisplaySup
					
	}
	
	if (Tecla=='u' || Tecla=='d'){
	if (Tecla=='u'){
  		if (Valor_Tmp < Max_day)
		    Valor_Tmp++;
 	    else Valor_Tmp=Max_day; 
	} else if (Tecla=='d'){
	if (Valor_Tmp > 1)
		Valor_Tmp--;
	else Valor_Tmp=1;
	
	}
	Pasar_Numero(Valor_Tmp,0,0);		           //DisplaySup
  save=TRUE;
}
	
/////////////////////// T0QUE RÁPIDO //////////////////////////

	if (Tecla=='r'){
	      if (save){ 
	       setFecha(date.Year,date.Month,Valor_Tmp);
	     
	       }
	        Salir_num();
	      
	}
		
/////////////////////// TOQUE MANTENIDO ////////////////////////

if (Tecla=='f')	Salir_num();
/////////////////////// EXIT //////////////////////////

if (Tecla== 'k') Exit();

}

/*   Procesador del  ingreso de hora    */
/****************************************/

void HoraHandler (void) {
  if (Tecla=='u' || Tecla=='d'){
	if (Tecla=='u'){
  		if (Valor_Tmp < 2359){
  		   Valor_Tmp++;
  			 if (((Valor_Tmp%100)%60)==0 && Valor_Tmp!=0) Valor_Tmp +=40;
  		}
 	    else Valor_Tmp=0; 
	} else if (Tecla=='d'){
	if (Valor_Tmp > 0){
	  
		if (((Valor_Tmp%100)%60)==0) Valor_Tmp-=40;
		Valor_Tmp--; 
	}
	else Valor_Tmp=2359;
	
	}
	Pasar_Numero(Valor_Tmp,0,2);		           //DisplaySup
  save=TRUE;
 // Tecla=' ';
}
	
	if (FstTime){
	    TIMEREC time;
	    
		  FstTime=FALSE;	
			
			PtrBox.Num=(Numerico*)PtrTmp;
			 
			
	    getTiempo(&time);
			Valor_Tmp = time.Hour*100+time.Min;
			DotNum[0]=PtrBox.Num->Dot;		
		  
		  PasarASCII(PtrBox.Num->TxtDpy,1);     //DisplayInf
			Pasar_Numero(Valor_Tmp,0,2);		           //DisplaySup
					
	}
	
/////////////////////// T0QUE RÁPIDO //////////////////////////

	if (Tecla=='r'){
	if (save){ 
	  setTiempo(Valor_Tmp/100,Valor_Tmp%100,0);
	  bufferNro= Valor_Tmp;
    buffersDato= PtrBox.Num->sDato;
	  /*
	  *(PtrBox.Num->Fdir)=Valor_Tmp;
	  if (Estado_Adquisicion!=1){
	    (void)TmDt1_SetTime((byte)(Valor_Tmp/100),(byte)(Valor_Tmp%100));
	    TmDt1_Enable(TRUE);
	  }
	  */
	}
	
	Salir_num();
	}
		
/////////////////////// TOQUE MANTENIDO ////////////////////////

if (Tecla=='f')	Salir_num();
/////////////////////// EXIT //////////////////////////

if (Tecla== 'k') Exit();


}

#endif

     
/* Procesador del box del titulo del Ptogramador viejo */
/*******************************************************/

#ifdef prog_viejo
void TitleProgHandler(void){
  TitleHandler();
  if (Tecla=='r'){ 
  programa_ingresado=0;
  segmento_ingresado=0;
  temperatura_f=Programa[0].Gral.temperatura_inicial;
  condicion_emer= Programa[0].Segmento[0].condicion_emer;
  tolerancia= Programa[0].Segmento[0].tolerancia;
  if(Programa[0].Segmento[0].tipo_tolerancia==Lo)
    tolerancia*=-1;  
  tiempo= Programa[0].Segmento[0].tiempo;
  }  
}									

/* Procesador de las pantallas del programador viejo */
/*****************************************************/
void ProgHandler(void){
bool Fst;
char new_text[5];

  if(FstTime) 
    Fst=TRUE;
  
  NumHandler();
  
  if (Fst==TRUE){
    new_text[0]=programa_ingresado+'1';
    new_text[1]=PtrBox.Num->TxtDpy[1];
    if((segmento_ingresado+1)/10){
      new_text[2]=(segmento_ingresado+1)/10+'0'; 
      new_text[3]=(segmento_ingresado+1)%10+'0';
    } else  {
      new_text[2]=' ';  
      new_text[3]=(segmento_ingresado)+'1';
    }
    new_text[4]='\0';
    PasarASCII(new_text,1);
  }
}


/* Procesador del box de temperatura del segmento 1º del programador viejo */
/***************************************************************************/
void Temperatura1Handler(void){
  ProgHandler();
  if(Tecla=='r'){
    Tinfoprograma BufferGeneral;		//Contiene los datos a grabar
    byte i;
    BufferGeneral.temperatura_inicial = Valor_Tmp;
    BufferGeneral.tipo_tolerancia = no;
    BufferGeneral.tolerancia = 0;
    BufferGeneral.condicion_emer = no; 

    #ifdef jony_17_08
    for(i=0;i<TAMANIO_GENERAL;i++)
      EscribirWord(((word)&Programa[programa_ingresado].Gral)+i*2,*((word*)&BufferGeneral+i)); 
    #endif
  }    
}

/* Procesador del box de temperatura de los segmentos (no el 1º) del programador viejo */
/***************************************************************************************/
void Temperatura2Handler(void){
  ProgHandler();
  if(Tecla=='r'){
    #ifndef jony_06_07	
    Tecla++;
    #endif
    if(save)
      EscribirWord((word)(&Programa[programa_ingresado].Segmento[segmento_ingresado-1].temperatura_f),Valor_Tmp);     
  }
}
/* Procesador del box de condición de emergencia */
/*************************************************/
void CondEHandler(void){
bool Fst=FALSE;
char new_text[5];

  if(FstTime)
    Fst=TRUE;
  

  
  TxtHandler();
  
  
  if (Fst==TRUE){
    new_text[0]=programa_ingresado+'1';
    new_text[1]=PtrBox.Txt->TxtDpy[1];
    if((segmento_ingresado+1)/10){
      new_text[2]=(segmento_ingresado+1)/10+'0'; 
      new_text[3]=(segmento_ingresado+1)%10+'0';
    } else  {
      new_text[2]=' ';  
      new_text[3]=(segmento_ingresado)+'1';
    }
    new_text[4]='\0';
    PasarASCII(new_text,1);
  }
  
  if(Tecla=='r'){
  const TSegmentos * dir;
  byte tSegmento;			//Contiene los datos a grabar
  
   
    if(Valor_Tmp==4 || segmento_ingresado==MAX_SEGMENTOS-1) /* si se ingresó End */
      tSegmento= End;
    else  
      tSegmento= rMPt;
    
    dir = &Programa[programa_ingresado].Segmento[0];
    dir+= segmento_ingresado;
    
    EscribirByte((word)&(dir->condicion_emer),Valor_Tmp); 
    EscribirWord((word)&(dir->tipo_segmento),tSegmento);
  
    /* Proximas pantallas:*/
    if(Valor_Tmp==4 || segmento_ingresado==MAX_SEGMENTOS-1){
      /* si se ingresó End */
      PtrTmp=R_Emergencia[1];
      programa_ingresado++;
      segmento_ingresado=0;
      SaveNow=TRUE;
      temperatura_f=Programa[programa_ingresado].Gral.temperatura_inicial;
			condicion_emer= Programa[programa_ingresado].Segmento[segmento_ingresado].condicion_emer;
      tolerancia= Programa[programa_ingresado].Segmento[segmento_ingresado].tolerancia;
      if(Programa[programa_ingresado].Segmento[segmento_ingresado].tipo_tolerancia==Lo)
        tolerancia*=-1;
      tiempo= Programa[programa_ingresado].Segmento[segmento_ingresado].tiempo;

    }
  }
}

/* Procesador del box de tiempo del segmento*/
/********************************************/
void TiempoHandler(void){
  ProgHandler();
  if(Tecla=='r') {
    if(save)
      EscribirWord((word)(&Programa[programa_ingresado].Segmento[segmento_ingresado].tiempo),Valor_Tmp);  
  }
    
}

/* Procesador del box de tolerancia del segmento*/
/************************************************/
void ToleranciaHandler(void){
byte Tipo_tol;
const TSegmentos * dir;
  
  ProgHandler();
  if(Tecla=='r'){
    if(Valor_Tmp<0){
      Valor_Tmp=-Valor_Tmp;
      Tipo_tol=Lo;  
    }
    else
      Tipo_tol=Hi;
    
    dir = &Programa[programa_ingresado].Segmento[0];
    dir+= segmento_ingresado;
    
    EscribirWord((word)&(dir->tolerancia),Valor_Tmp);
    EscribirByte((word)&(dir->tipo_tolerancia),Tipo_tol);
    /* Cargo para el próximo segmento */
    segmento_ingresado++;
    dir++;
    temperatura_f=(dir-1)->temperatura_f;  /*Esta desfesado 1 con respecto al nuevo*/
    condicion_emer= (dir)->condicion_emer;
    tolerancia= Programa[programa_ingresado].Segmento[segmento_ingresado].tolerancia;
    if(Programa[programa_ingresado].Segmento[segmento_ingresado].tipo_tolerancia==Lo)
        tolerancia*=-1;
		tiempo= Programa[programa_ingresado].Segmento[segmento_ingresado].tiempo;
  }    
}
#endif



/* Procesador de la pantalla principal */
/***************************************/
void MainHandler(void){


extern word screen_cont;

extern bool DSave;

extern bool Mostrar_Proc;

extern bool estado_dobleSp;
extern int nroProgEnAccion;
extern int crearProg;
unsigned char i;
static bool unaVez = FALSE;
static bool fTimer = FALSE;
static bool resetScrollUnaVez = FALSE;

#ifdef VF_PROG
is_box_principal=3;	
#else
is_box_principal=1;	
#endif

#if  CANTIDAD_CANALES == 2
//unsigned char i;
#endif

#ifndef VF_PROG
/* ejecuto si es la primera vez que se llama al manejador */
  if(!unaVez){
    unaVez=TRUE;
    #ifndef HD90
    PasarASCII(tipoEquipo,1);
    #endif
    PasarASCII(numver,0);
    #ifdef programador
	  Timer_Run(5000,&fTimer, UNICO_SET);
	  #else
	  Timer_Run(7000,&fTimer, UNICO_SET);
	  #endif
  }
  

  if(!fTimer)    // espero que termine el cartel inicial
    return;
  
#endif 
  
  if(!resetScrollUnaVez){
   resetScrollUnaVez = TRUE;
   ResetScroll();
  }
  
	if (FstTime){													 // es la primera vez que ingresa??
	  FstTime=FALSE; 												 // sacar primera vez
    DotNum[0]=(byte)PRom[R_Decimales];
    
  #if  CANTIDAD_CANALES == 1
    DotNum[1]=(byte)PRom[R_Decimales];
  #elif  CANTIDAD_CANALES == 2
    DotNum[1]=(byte)PRom[R_Decimales+1];
  #endif

	PtrBox.Num=(Numerico*)PtrTmp;					 // Poner el puntero PtrBox.Num con el valor del Box actual
    
    	
  #if  CANTIDAD_CANALES == 1 
   
	  
	  if((int *)DirPar == &SetPoint[R_SetPoint+0]){
	    Valor_Tmp = *(PtrBox.Num->sDato->Fdir);
	    
	  }
	  else{
	    
	    // *(int *)DirPar = *(PtrBox.Num->sDato->Fdir);
	     Valor_Tmp = *(int *)DirPar;
	       
	  }
  #endif
  
  #ifdef VF_PROG
  crearProg=nroProgEnAccion;	
  #endif
  
  Mostrar_Proc=TRUE;
	}




#if  CANTIDAD_CANALES == 1 && !defined(HD90)  && !defined(DOBLE_SP) && !defined(VF) && !defined(INTEGRADOR)

    if ((int *)DirPar == &SetPoint[R_SetPoint+0] &&((Tecla=='u' || Tecla=='d'))){ // Fue presionada una Tecla UP o Down???
  
	 #ifndef pirani
	  /* proceso Tecla UP */
	    if (Tecla=='u'){									 // Fue presionada una Tecla UP ???
		      
		      if (Valor_Tmp<(*PtrBox.Num->sDato->Sup)) Valor_Tmp++; // El Buffer es menor que el LimiteSup?? Si-> Incrementar Buffer
  				else Valor_Tmp=(*PtrBox.Num->sDato->Sup);						 // No-> Poner Buffer en maximo valor
  		};
  	/* proceso Tecla down */
  	  if (Tecla=='d'){		  					      // Fue presionada una Tecla Down???
	  			if (Valor_Tmp>(*PtrBox.Num->sDato->Inf)) Valor_Tmp--;		// El Buffer es mayor que el LimiteInf?? Si-> Decrementar Buffer
	  			else Valor_Tmp=(*PtrBox.Num->sDato->Inf);	// No-> Poner Buffer en el minimo valor				
  		};

      #ifdef LOBOV
      Pasar_Numero(Valor_Tmp,0,DotNum[1]); // Mostrar DisplaySup
      #else
	    Pasar_Numero(Valor_Tmp,1,DotNum[1]); // Mostrar DisplayInf
      #endif
  
	 #else
	  int sumador,div;  
    
	    if(PRom[R_Sensor]==SENSOR_PIR){
        for(div=1;(Valor_Tmp/div)>0;div*=10);
        if(div>1)
          div/=10;
        if(div>1)
          div/=10;
        if(((Valor_Tmp)/(div*10))==1 && ((Valor_Tmp)%(div*10))==0 && (div>=10) && Tecla=='d')
          div/=10;
        sumador=div;
	    } else
	    sumador = 1;


	    /* proceso Tecla UP */
	    if (Tecla=='u'){									 // Fue presionada una Tecla UP ???
		    
		    if (Valor_Tmp+sumador<(*PtrBox.Num->sDato->Sup)) Valor_Tmp+=sumador; // El Buffer es menor que el LimiteSup?? Si-> Incrementar Buffer
				else Valor_Tmp=(*PtrBox.Num->sDato->Sup);						 // No-> Poner Buffer en maximo valor
	    };
	    /* proceso Tecla down */
	    if (Tecla=='d'){		  					      // Fue presionada una Tecla Down???
			
				if (Valor_Tmp-sumador>(*PtrBox.Num->sDato->Inf)) Valor_Tmp-=sumador;		// El Buffer es mayor que el LimiteInf?? Si-> Decrementar Buffer
				else  Valor_Tmp=(*PtrBox.Num->sDato->Inf);	// No-> Poner Buffer en el minimo valor			
		  };
			if(PRom[R_Sensor]==SENSOR_PIR)
        Pasar_Numero_Expo(Valor_Tmp,1,DotNum[0]);
      else
        Pasar_Numero(Valor_Tmp,1,DotNum[0]);						 //Mostrar Variable del Proceso
   
   
   #endif


	  DSave=FALSE;
	  Timer_Run(4000,&DSave,UNICO_SET);							// Grabar parametros con delay
	  
	  Escribir(PtrBox.Num->sDato,Valor_Tmp);
	  bufferNro= Valor_Tmp;
  	buffersDato= PtrBox.Num->sDato;
	  	  
	 // Tecla=' ';							//Borrar Tecla presionada



    }
  //#ifndef programador  
   //}
  //#endif
 #ifdef CCAL

  	  if (Tecla=='k'){		  					      // Fue presionada una Tecla Down???
				if(PRom[R_Tsalcont]==_man){
	        EscribirWord((word)&PRom[R_Tsalcont],_cal);
				}
				else{
	        EscribirWord((word)&PRom[R_Tsalcont],_man);
				}
  		};

#endif

  

  
/*    
    
HAY QUE PONERLO EN OTRA PARTE!!!!!!!!!!!!!!!!


    else if (show_main_text==TRUE){
      if(screen_cont<=MAIN_TEXT_TIME){  
      PasarASCII(main_text,1);  // Mostrar DisplayInf
      }else {
        DotNum[1]=(byte)PRom[R_Decimales];
        #ifdef jony_15_08
          #ifndef pirani
        if((int *)DirPar!=&SetPoint[0] || PRom[R_Programa]!=NO_PROGRAMA)
          Pasar_Numero(*(int *)DirPar,1,DotNum[1]); // Mostrar DisplayInf
        else
          Pasar_Numero(Valor_Tmp,1,DotNum[1]); // Mostrar DisplayInf
          #endif
        #else
        Pasar_Numero(Valor_Tmp,1,DotNum[1]); // Mostrar DisplayInf 
        #endif
      }
    }

*/
  

#endif


/* muestro el valor del proceso en cada pantalla */		

 if (Mostrar_Proc){										                      // Se actualizaron los valores???
#if CANTIDAD_CANALES == 2  
 
  for(i=0;i<CANTIDAD_CANALES;i++){							            // Recorrer todos los canales
    if (getEstatusResult(i)==ERR_OK){  							                  // El REsultado de la linealización fue OK??
        DotNum[i]=PRom[R_Decimales+i];											// pongo decimales
      #ifdef pirani
        if(PRom[R_Sensor+i]==SENSOR_PIR)										// si es pirani
          Pasar_Numero_Expo(getValCalculado(i),i,DotNum[i]);				// paso expo
        else
      #endif

      #ifdef SP_EXTERNO																			// si es sp externo analizo el loop
        if(i==0)
          Pasar_Numero(getValCalculado(i),i,DotNum[i]);	          // muestro canal 1
				if(i==1)
			  	Pasar_Numero(getSetPoint(0),i,DotNum[i]);			      // y el sp combinado
      #else
        #ifdef SENS_HUMEDAD
          if(i==0)                                    // canal 1 ? 
             Pasar_Numero(getValCalculado(i),i,DotNum[i]);   //muestro la temp bs 
          if(i==1){                                   // canal 2 ?
            if(PRom[R_Sensor+1]==SENSOR_HUM)          //humedad ?
              Pasar_Numero(Humedad,i,DotNum[i]);      //muestro la humedad  
            else
              Pasar_Numero(getValCalculado(i),i,DotNum[i]);  // si no, muestro la temp bh 
          }
        #else
          Pasar_Numero(getValCalculado(i),i,DotNum[i]);			// si no es, muestro normal
        #endif
      #endif

    }

    else if (getEstatusResult(i)==ERR_UF)					                    // El REsultado de la linealización fue UF??
        PasarASCII((char *)UF,i);							              // Mostrar UF
    else if (getEstatusResult(i)==ERR_OF)					                    // El REsultado de la linealización fue OF??
        PasarASCII((char *)OF,i);							              // Mostrar OF
        
    if (show_main_text==TRUE)
       if(screen_cont<=MAIN_TEXT_TIME) 
          if (PRom[R_Stn+i]== Stc)
             PasarASCII(main_text,i);                       // Mostrar DisplayInf
  }																													// fin del for de canales



 Mostrar_Proc=FALSE;  								                      // Sacar el flag de mostrar
}																														// fin del if de mostrar


            
/***********************************************************************************************************/
/*MANEJO DE LA PANTALLA PRINCIPAL EN APARATOS DE UN CANAL*/
/***********************************************************************************************************/

/*El display superior*/

#elif CANTIDAD_CANALES == 1 
 
 #ifdef LOBOV
  
   Pasar_Numero(Valor_Tmp,0,DotNum[0]);
 
 #else 
  if (getEstatusResult(0)==ERR_OK){  							                        // El REsultado de la linealización fue OK??
    DotNum[0]=(byte)PRom[R_Decimales];													// Pongo punto
      #ifdef pirani
      if(PRom[R_Sensor]==SENSOR_PIR)														// Convierto si es Pirani
      Pasar_Numero_Expo(getValCalculado(0),0,DotNum[0]);
      else
      #endif
      #ifdef HOLD
      mantenerIndicacion ();                 
      #elif defined(LUX) 
      if (PRom[R_Sensor]==SENSOR_LIN2 || PRom[R_Sensor]==SENSOR_LIN3 || PRom[R_Sensor]==SENSOR_LIN4 || PRom[R_Sensor]==SENSOR_JB || PRom[R_Sensor]==SENSOR_PT){
         if(PRom[R_RSL]==0)
          Pasar_Numero(valorAmostrar[0],0,DotNum[0]);
         else if(PRom[R_RSL]==1 && DotNum[0]>=1)
           Pasar_Numero(valorAmostrar[0],0,(DotNum[0]-1));
         else if(PRom[R_RSL]==2 && DotNum[0]==1)
           Pasar_Numero(valorAmostrar[0],0,(DotNum[0]-1));
         else if(PRom[R_RSL]==2 && DotNum[0]>=2)
           Pasar_Numero(valorAmostrar[0],0,(DotNum[0]-2)); 
      }else
         Pasar_Numero(valorAmostrar[0],0,DotNum[0]);
      
      #elif defined MMAX
      mantenerMaxIndicacion ();
      #else                                                                               
      Pasar_Numero(getValCalculado(0),0,DotNum[0]);						        //Mostrar Display sup, Variable del Proceso
      #endif
  }
  else if (getEstatusResult(0)==ERR_UF)					                          // El Resultado de la linealización fue UF??
    PasarASCII(UF,0);							                              // Mostrar UF
  else if (getEstatusResult(0)==ERR_OF)					                          // El REsultado de la linealización fue OF??
    PasarASCII(OF,0);							                              // Mostrar OF
 #endif

/*El display inferior*/


     
 #if(defined(VF) && !defined(VF_BKR))
 
 if(PRom[R_MVF]==_VF) {
  
  #ifdef VF_PROG
  flagCartel=1;
  #else
  flagCartel=cartelesHandler();
  #endif
  
  if(flagCartel!=0){
    
   if(VFstatus == ENDVF){
    set_MainText("Fin "); 
    show_main_text=TRUE;
    screen_cont=0;
   }else{
    show_main_text=FALSE;
    vitroFusionHandler();
   }
  }
 }else{
  if(VFstatus == ENDVF){
    set_MainText("Fin "); 
    show_main_text=TRUE;
    screen_cont=0;
   }else{
    show_main_text=FALSE;
   }
 }
 
#elif defined(VF_BKR)

 if(PRom[R_MVF]==_VF) {
 
   flagCartel=1;
   if(VFstatus == ENDVF){
    set_MainText("Fin "); 
    show_main_text=TRUE;
    screen_cont=0;
   }else{
    show_main_text=FALSE;
    vitroFusionHandler();
   }
 }
#endif

#ifdef pirani																										// Si es Pirani
  if(PRom[R_Sensor]==SENSOR_PIR)																// hago la conversion
    Pasar_Numero_Expo(Valor_Tmp,1,DotNum[1]);
  else
    Pasar_Numero(Valor_Tmp,1,DotNum[1]);                        // Mostrar DisplayInf

#elif defined (LOBOV) 
 PasarASCII("SP 1",1);
#else   

        																									// y nada mas
  if (show_main_text==TRUE&&screen_cont<=MAIN_TEXT_TIME ){  // Hay texto para mostrar?, es tiempo de mostrar?
        PasarASCII(main_text,1);                                // Lo muestro en DisplayInf y chau
        
  }
  
  else{
      #ifdef programador
      if((int *)DirPar!=&SetPoint[R_SetPoint+0] || PRom[R_Programa]!=NO_PROGRAMA)													  // El amod es distinto del SP?
      #else
      if((int *)DirPar!=&SetPoint[R_SetPoint+0])
      #endif  
        Pasar_Numero(*(int *)DirPar,1,DotNum[1]);               // Lo muestro en DisplayInf y chau
      else if(PRom[R_Ver]==POT)        
            Pasar_Numero(dutytmp,1,1);                  //presento el valor de la potencia 
           else 
              #ifdef DOBLE_SP
                  Pasar_Numero(getSetPoint(0),1,DotNum[1]);
              #elif defined (VF)
                 if(PRom[R_MVF]==_SP)
                   Pasar_Numero(getSetPoint(0),1,DotNum[1]);                 // si nada de lo anterior,Mostrar Setpoint
              #elif defined (LUX) && defined (INTEGRADOR)
                   Pasar_Numero(getValCalculado(0),1,DotNum[1]);
              #elif defined (INTEGRADOR)
                  if (PRom[R_Sensor]==SENSOR_LIN2 || PRom[R_Sensor]==SENSOR_LIN3 || PRom[R_Sensor]==SENSOR_LIN4 || PRom[R_Sensor]==SENSOR_JB || PRom[R_Sensor]==SENSOR_PT){
                    if(PRom[R_RSL]==0)
                      Pasar_Numero(valorAmostrar[0],1,DotNum[1]);
                     else if(PRom[R_RSL]==1 && DotNum[0]>=1)
                      Pasar_Numero(valorAmostrar[0],1,(DotNum[1]-1));
                     else if(PRom[R_RSL]==2 && DotNum[0]==1)
                      Pasar_Numero(valorAmostrar[0],1,(DotNum[1]-1));
                     else if(PRom[R_RSL]==2 && DotNum[0]>=2)
                      Pasar_Numero(valorAmostrar[0],1,(DotNum[1]-2));
                  } else
                      Pasar_Numero(valorAmostrar[0],1,DotNum[1]);
              
              #else 
                           
                   Pasar_Numero(Valor_Tmp,1,DotNum[1]);                 // si nada de lo anterior,Mostrar Setpoint
                 
              #endif
  }
 
    
  if(PRom[R_Stn]!=SINTONIA)                   
        show_main_text=FALSE;
  
   if(PRom[R_Ver]==POT && PRom[R_Stn]!=SINTONIA){
      
      flagpote=1;
      set_MainText("PotE");        //muestro "pote" en el dyp inferior
      
   } else if(flagpote==1){
      flagpote=0;
      set_MainText("");
      
     }  
     
  
#endif

    Mostrar_Proc=FALSE;  								                        // Sacar el flag de mostrar	 variables
}
#endif
  

/***********************************************************************************************************/

#ifdef VF  
 if(flagCartel!=0 || PRom[R_MVF]==_SP){
  
// T0QUE RÁPIDO
  if (Tecla=='r'){
    flagCartel=0;
    if(!save_parametros){
      Salir_num();
    }else SaveNow=DSave=TRUE;				
  }			
 }
#else
  if (Tecla=='r')
    if(!save_parametros){
      Salir_num();
      #ifdef programador
      if(/*PtrTmp==&C1SP &&*/ PRom[R_Programa]!=NO_PROGRAMA)
        PtrTmp=&Segmento1.DirProc; 
      #endif
    }else SaveNow=DSave=TRUE;				
#endif

#if (defined(VF) && !defined(VF101)) 

// T0QUE MANTENIDO
 if(flagCartel!=0 || PRom[R_MVF]==_SP)
  if (Tecla=='t')if(!save_parametros) Salir_num();else SaveNow=DSave=TRUE;
 
#elif(defined(VF) && defined(VF101))   
  if (Tecla=='f')if(!save_parametros) Salir_num();else SaveNow=DSave=TRUE; 


#elif(defined(DH102) && !defined(nico_1_10))
  if (Tecla=='t')if(!save_parametros) Salir_num();else SaveNow=DSave=TRUE;

#else
   if (Tecla=='f')if(!save_parametros) Salir_num();else SaveNow=DSave=TRUE;

#endif

#ifdef VF_PROG

	if (Tecla== 'k' || VFstatus == ENDVF) Exit();	 // Boton de Exit
#endif	
}

/////////////////// Funcion del Main (Otra razon para usar Clases)///////////

#ifdef jony_28_06
void set_MainText(const char * str){								// Pongo un string en MainText
byte i;																							// y levanto flag para avisar que hay texto

  for(i=0;i<MAX_MAIN_TEXT;i++)											// y apago poniendo un string vacio
		main_text[i] = 0;



  for(i=0;(str[i]!='\0')&&i<MAX_MAIN_TEXT;i++)			// y apago poniendo un string vacio
    main_text[i]=str[i];
  if(i==0)
    show_main_text=FALSE;                           // Si string vacio, apago flag
  else{
    show_main_text=TRUE;
    main_text[i+1]='\0';                            // Si string lleno, prendo flag
  }
}

#endif
///////////////////////////Funciones internas /////////////
///////////////////////////////////////////////////////////
void Exit(void){	// Funcion Boton de Exit 
		ResetScroll();
	//	Tecla=' ';
		Flag1=TRUE;
}

void Salir_num(void){		// Funcion de Salida por toque mantenido o rapido
 PunteroF Proximo;	/* Proximo estado con toque sostenido */
 
  

 if(Tecla=='r'){  
  if (PtrBox.Num->VdeNavegacion== NULL)// Existe variable selectora de los Proximos Estados??
		 Proximo=(PunteroF)PtrBox.Num->Teclar; // Cargar proximo Box por toque sostenido
	else  Proximo=PtrBox.Num->Teclar[*(PtrBox.Num->VdeNavegacion)];		 // Cargar proximo Box por toque rapido
 }else  Proximo=PtrBox.Num->Teclaf;  // Cargar proximo Box por toque sostenido
  
         
	if (Proximo!=NULL) {	// Se cargo un box existente???  
		  {
		  #ifdef VF_PROG
		  if(Proximo==&Principal1.DirProc && VFstatus != ENDVF)
		    Proximo=&Principal1_VF_PROG.DirProc;
		  #endif
		  ResetScroll();
		  PtrTmp=Proximo;  //Cargar puntero a la funcion correspondiente (TitleHandler, NumHandler, etc.)
		  FstTime=TRUE;		 //Setear Primera vez
		 
		  }
	
	}

}



//////////////////////////////funciones de accion para el ingreso en los boxes/////////

// VARIABLES QUE UTILIZAN LOS BOXES
//////////////////////////////////

/* Funcion de accion para Correr un programa*/
/********************************************/

byte CorrerPrograma(int iPrograma,byte chan){
#ifdef programador
  #ifdef prog_viejo
    runPrograma(chan,(byte)iPrograma-1,0,0);
  #else
    runPrograma(chan,(byte)iPrograma,0,0);
  #endif		    
//  #ifdef jony_18_07
	Lim_Segmento2[chan]=get_Lim_Seg(iPrograma-1);
	#ifdef prog_viejo
  Lim_Segmento2[chan]++;
  #endif														 
  if (chan == 0 && iPrograma==NO_PROGRAMA)
    set_MainText("");
#endif
  return ERR_OK;
}
 
byte CambiarSegmento(int iSegmento,byte chan){
#ifdef programador
 if(iSegmento>0)
 #ifdef prog_viejo
    runPrograma(chan,PRom[R_Programa+chan]-1,iSegmento-OFFSET_SEG1,0);
  #else
    runPrograma(chan,PRom[R_Programa+chan],iSegmento-OFFSET_SEG1,0);
  #endif
#endif
return ERR_OK;
}


byte CambiarTiempo(int iTiempo,byte chan){
extern int SegmentoMostrado[];  
  #if defined(programador) && defined(jony_15_08)
//  SegundosSegmento[chan]= iTiempo*60;
  if(SegmentoActual[chan]<Lim_Segmento2[chan])
   #ifdef prog_viejo
    runPrograma(chan,PRom[R_Programa+chan]-1,SegmentoMostrado[chan]-OFFSET_SEG1, (long)iTiempo*60);
   #else
    runPrograma(chan,PRom[R_Programa+chan],SegmentoMostrado[chan]-OFFSET_SEG1, (long)iTiempo*60);
   #endif

  #endif
  return ERR_OK;
}

/* Funcion de accion al cambiar los decimales*/
/*********************************************/
byte Sensor_dec[CANTIDAD_CANALES];  		    

byte A_Decimales(int decimales,byte chan){			  
 byte decimal_chan;
 byte sensor= Sensor_dec[chan];
 decimal_chan = SENSOR_Decimales(sensor);

  if((sensor!=SENSOR_LIN1) && (sensor!=SENSOR_LIN2) &&(sensor!=SENSOR_LIN3) &&(sensor!=SENSOR_LIN4) ){
    Lim_Inf_ch[chan]= (long)SENSOR_Minimo(sensor)/(long)pow10(decimal_chan-decimales);
    Lim_Sup_ch[chan]= (long)SENSOR_Maximo(sensor)/(long)pow10(decimal_chan-decimales);
  }else {
    Lim_Inf_ch[chan]= SENSOR_Minimo(sensor);
    Lim_Sup_ch[chan]= SENSOR_Maximo(sensor);
  }
  
  return ERR_OK;
}


/* Funcion de accion para Tipo de Sensor*/
/****************************************/

byte A_Sensor(int sensor,byte chan){			  
char i;

  #ifdef SENS_HUMEDAD
  if (sensor==SENSOR_PT || sensor==SENSOR_HUM) 
    PTSL_PutBit(chan,TRUE);
	else PTSL_PutBit(chan,FALSE);
	#else
	if (sensor==SENSOR_PT) 
    PTSL_PutBit(chan,TRUE);
	else PTSL_PutBit(chan,FALSE);
	
	#endif  		    
  
  Sensor_dec[chan]= sensor;
  
  Lim_Dec[chan]= SENSOR_Decimales(sensor);

  Escribir(&Parametros[R_Decimales+chan],SENSOR_Decimales_Mostrar(sensor));

  if(Lim_Inf_ch[chan]>PRom[R_Lim_Inf+chan])
    Escribir(&Parametros[R_Lim_Inf+chan],Lim_Inf_ch[chan]);
  if(Lim_Sup_ch[chan]<PRom[R_Lim_Sup+chan])
    Escribir(&Parametros[R_Lim_Sup+chan],Lim_Sup_ch[chan]);
  
  for(i=0;i<CANTIDAD_CANALES;i++)
    bufferSensor[i]=PRom[R_Sensor+i];
    
  return ERR_OK;
}

void setLimitesRPM (){
   Lim_Inf_ch[0]= 9999;
   Lim_Sup_ch[0]= 9999;
   if(Lim_Inf_ch[0]>PRom[R_Lim_Inf])
    Escribir(&Parametros[R_Lim_Inf],Lim_Inf_ch[0]);
  if(Lim_Sup_ch[0]<PRom[R_Lim_Sup])
    Escribir(&Parametros[R_Lim_Sup],Lim_Sup_ch[0]);
}


byte A_Selftun(int valor, byte salida){
  if(valor!=0){   
    St_Step[salida]=0;
    St_Counter[salida]=0;
  #ifdef jony_28_06
    main_text[0]='S';
    main_text[1]='t';
    main_text[2]=' ';
    main_text[3]='0';
    show_main_text=TRUE;
  } else 
    show_main_text=FALSE;
  #else
    set_MainText("St 0");
  } else set_MainText("");
  #endif
  return ERR_OK;		 
};

