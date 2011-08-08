/* archivos include */
#include <string.h>
#include "display.h"
#include "PE_Error.h"
#include "PWM.h"
#include "IFsh10.h"
#include "cnfbox.h"
#include "Programador.h"
#include "Sensores.h"
#include "boxes.h"
#include "boxesprog.h"
#include "boxesset.h"
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
#ifdef INTEGRADOR
#include "Integrador.h"
#endif
#ifdef ADQ
#include "adq.h"
#endif
#include "Mmax.h"
#include "Parametros.h"
#include "Medicion.h"
#include "PID.h"
#include "valorControl.h"
#include "TimeOut.h"
#include "StaticTimer.h"
#include "SelfTune.h"

#define MAX_MAIN_TEXT 25

bool Mostrar_Proc;

#ifdef pirani
static const char UF[]="UF  ";
static const char OF[]="OPEn  ";
#else
static const char UF[]=" UF ";
static const char OF[]="OPEn";
#endif

static char main_text[MAX_MAIN_TEXT]="St  ";

static bool show_main_text;	                                    // Se pone en uno al poner texto en maintext

bool flagpote=0;																																// hay que ponerlo en cero a mano

word screen_cont;

char is_box_principal=0;

bool presentar;

int bufferSensor[CANTIDAD_CANALES];

byte Tecla;

/* variables de uso temporal por los boxes cuando se activan */
int Valor_Tmp;				// Buffer del parametro procesado

int bufferNro;

TDato *buffersDato;

TPtrBox PtrBox; //puntero a los tipos de boxes manipulados

bool save_parametros=FALSE;	/* Ordena salvar el valor en la ram para copy a eeprom cuando set*/

bool save;

bool FstTime;	/* set cuando se ejecuto al menos una vez el procesador */

PunteroF PtrTmp;			                  /* para el manipuleo */

bool flagTimeReturn;

#ifdef programador
static enum {NOPROGRAMA,PROGRAMA}BoxAnterior; // Para saber desde donde se entro al BOx de Segmento
#endif

/* Drivers para simular el hardware */
/************************************/

//////////////Funciones internas//////////////////////
void Exit(void);					 // Salir a pantalla principal
void SalirBox(TipoBox tipo);	 // Salir por toque rapido o sostenido en pantalla numérica


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
}

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
  PtrTmp=&Principal1.DirProc;		      // Empezar en Principal
  flagTimeReturn=FALSE;
  Timer_Run(TRETURN,&flagTimeReturn, UNICO_SET); 
}

/**/
void executeBoxes(){
  (*PtrTmp)();                          // Funcion para el box correspondiente; llama a Num Handler, TextHandler, etc.	
}


/**/
void setBufferSensor(){
  byte i;
  
  for(i=0;i<CANTIDAD_CANALES;i++){
 		A_Sensor(PRom[R_Sensor+i],i);			  //Limites para el sensor y dec y el PTSL
	  bufferSensor[i]=PRom[R_Sensor+i];
 	}
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
		
	  
	  PresentarMsj(&PtrBox.Tit->TxtDpy[0],1);
	  PresentarMsj(&PtrBox.Tit->TxtDpySup[0],0);
		
		//PasarASCII(&PtrBox.Tit->TxtDpy[0],1);    //mostrar display inferior
		//PasarASCII(&PtrBox.Tit->TxtDpySup[0],0); //mostrar display superior	
									 //Borrar Tecla presionada
		}
		
//T0QUE RÁPIDO  O MANTENIDO

	if (!save_parametros) 				// Se presiono Tecla rapida o mantenida??? y no hay nada para guardar??
	  SalirBox(Titulos);	 // funcion de salida 


// EXIT 

	if (Tecla== 'k'){
	   PresentarMsj((char*)"    ",1);
	 	
	
	// PasarASCII("    ",1);   //borro la pantalla una ves 
	 ResetScroll(); 
	 Exit();	 // Boton de Exit
	}

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
		
		PresentarMsj(PtrBox.Txt->TxtDpy,1);
		PresentarMsj(PtrBox.Txt->TxtDpy0[Valor_Tmp],0);
	
		//PasarASCII(PtrBox.Txt->TxtDpy,1);             //Mostrar DisplayInf
		//PasarASCII(PtrBox.Txt->TxtDpy0[Valor_Tmp],0);  //Mostrar DisplaySup		 

  }


  // KeyEdgeS UP Y DOWN
  handlerTeclasUPoDown(Textuales);

  // T0QUE RÁPIDO O MANTENIDO
  SalirBox(Textuales);	 // funcion de salida
 
/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') 
	  Exit(); // Boton de Exit
}

 

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
	AnalisisPuntoDecimal();			 
	
// mando mensajes
						
		 	PresentarMsj(PtrBox.Num->TxtDpy,1);
		  PresentarValor(Valor_Tmp,0);
		  //PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
			//Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
	}
	
  handlerTeclasUPoDown(Numericos);
  
/////////////////////// T0QUE RÁPIDO //////////////////////////
			
  SalirBox(Numericos);		// funcion de salida
		
/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') 
	  Exit();	 // Boton de Exit
}


/*Procesador de los boxes numéricos especial Temp Ambiente*/
void NumHandlerEsp(void){
     is_box_principal=2;
// Primer Ingreso	
	if (FstTime){				// es la primera vez que ingresa??
	   
		  FstTime=FALSE;  // sacar primera vez
			save=FALSE; /*Reseteo flag de mandar a grabar*/
			PtrBox.Num=(Numerico*)PtrTmp;	 // Poner el puntero PtrBox.Num con el valor del Box actual
	    		
// analizo punto decimal
				 
		AnalisisPuntoDecimal();
		
// mando mensajes
		
		 Valor_Tmp=(-(getTempAmbiente()) + PRom[R_ATA]);
			 
		  PresentarMsj(PtrBox.Num->TxtDpy,1);
		  PresentarValor(Valor_Tmp,0);
		  //PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
			//Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
	}
	
	Valor_Tmp=(-(getTempAmbiente()) + PRom[R_ATA]);
			 	  
	PresentarMsj(PtrBox.Num->TxtDpy,1);
	PresentarValor(Valor_Tmp,0);
	//PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
	//Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					

/////////////////////// T0QUE RÁPIDO o MANTENIDO //////////////////////////


  SalirBox(Numericos);		// funcion de salida
			

/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') 
	  Exit();	 // Boton de Exit
}



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
    	PresentarMsj(new_text,1);
    //PasarASCII(new_text,1);     //Mostrar DisplayInf  
    } else
        PresentarMsj(PtrBox.Num->TxtDpy,1);
        //PasarASCII(PtrBox.Num->TxtDpy,1);    
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
      PresentarMsj(new_text,1);
      //PasarASCII(new_text,1);     //Mostrar DisplayInf  
    } else
        PresentarMsj(PtrBox.Num->TxtDpy,1);
        //PasarASCII(PtrBox.Num->TxtDpy,1);    
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








/* Procesador de la pantalla principal */
/***************************************/
void MainHandler(void){

unsigned char i;
static bool unaVez = FALSE;
static bool fTimer = FALSE;
static bool resetScrollUnaVez = FALSE;

#ifdef VF_PROG
is_box_principal=3;	
#else
is_box_principal=1;	
#endif


#ifndef VF_PROG
/* ejecuto si es la primera vez que se llama al manejador */
  if(!unaVez){
    unaVez=TRUE;
    #ifndef HD90
    PresentarMsj(tipoEquipo,1);
    //PasarASCII(tipoEquipo,1);
    #endif
    PresentarMsj(numver,0);
    //PasarASCII(numver,0);
    #ifdef programador
	  InitTimeOut(5000,&fTimer,ONETIME);
	  #else
	  InitTimeOut(7000,&fTimer,ONETIME);
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
    setDecimalesDys(displays,0,(byte)PRom[R_Decimales]);
    setDecimalesDys(displays,1,(byte)PRom[R_Decimales]);
    //DotNum[0]=(byte)PRom[R_Decimales];
    
 
  #if  CANTIDAD_CANALES > 1
     setDecimalesDys(displays,0,(byte)PRom[R_Decimales]);
     setDecimalesDys(displays,1,(byte)PRom[R_Decimales+1]);
    //DotNum[1]=(byte)PRom[R_Decimales+1];
  #endif

	PtrBox.Num=(Numerico*)PtrTmp;					 // Poner el puntero PtrBox.Num con el valor del Box actual
    
    	
  #if  CANTIDAD_CANALES == 1 
   
	  
	  if((int *)DirPar == &SetPoint[R_SetPoint+0]){
	    Valor_Tmp = *(PtrBox.Num->sDato->Fdir);
	    
	  }
	  else{
	    
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
	 
	  handlerTeclasUPoDown(Principal);

      #ifdef LOBOV
      PresentarValor(Valor_Tmp,0);
      //Pasar_Numero(Valor_Tmp,0,DotNum[1]); // Mostrar DisplaySup
      #else
      PresentarValor(Valor_Tmp,1);
	    //Pasar_Numero(Valor_Tmp,1,DotNum[1]); // Mostrar DisplayInf
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
		    
		    if (Valor_Tmp+sumador<(*PtrBox.Num->sDato->Sup)) 
		      Valor_Tmp+=sumador; // El Buffer es menor que el LimiteSup?? Si-> Incrementar Buffer
				else 
				  Valor_Tmp=(*PtrBox.Num->sDato->Sup);						 // No-> Poner Buffer en maximo valor
	    }
	    /* proceso Tecla down */
	    if (Tecla=='d'){		  					      // Fue presionada una Tecla Down???
			
				if (Valor_Tmp-sumador>(*PtrBox.Num->sDato->Inf)) 
				  Valor_Tmp-=sumador;		// El Buffer es mayor que el LimiteInf?? Si-> Decrementar Buffer
				else  
				  Valor_Tmp=(*PtrBox.Num->sDato->Inf);	// No-> Poner Buffer en el minimo valor			
		  }
			
			if(PRom[R_Sensor]==SENSOR_PIR)
        PresentarValorExpo(Valor_Tmp,1);
        //Pasar_Numero_Expo(Valor_Tmp,1,DotNum[0]);
      else
        PresentarValor(Valor_Tmp,1);
        //Pasar_Numero(Valor_Tmp,1,DotNum[0]);						 //Mostrar Variable del Proceso
   
   
   #endif


	  DSave=FALSE;
	  Timer_Run(4000,&DSave,UNICO_SET);
	  
	  Escribir(PtrBox.Num->sDato,Valor_Tmp);
	  bufferNro= Valor_Tmp;
  	buffersDato= PtrBox.Num->sDato;
	  	  
	 
    }
  
    #ifdef CCAL

  	  if (Tecla=='k'){		  					      // Fue presionada una Tecla Down???
				if(PRom[R_Tsalcont]==_man){
	        EscribirWord((word)&PRom[R_Tsalcont],_cal);
				}
				else{
	        EscribirWord((word)&PRom[R_Tsalcont],_man);
				}
  		}

    #endif

#endif
/* muestro el valor del proceso en cada pantalla */		

 if (Mostrar_Proc){										                      // Se actualizaron los valores???
#if CANTIDAD_CANALES == 2  
 
  for(i=0;i<CANTIDAD_CANALES;i++){							            // Recorrer todos los canales
    if (getEstatusResult(i)==ERR_OK){  							                  // El REsultado de la linealización fue OK??
        //DotNum[i]=PRom[R_Decimales+i];
        setDecimalesDys(displays,i,(byte)PRom[R_Decimales+i]);											// pongo decimales
      #ifdef pirani
        if(PRom[R_Sensor+i]==SENSOR_PIR)										// si es pirani
          PresentarValorExpo(getValCalculado(i),i);
          //Pasar_Numero_Expo(getValCalculado(i),i,DotNum[i]);				// paso expo
        else
      #endif

      #ifdef SP_EXTERNO																			// si es sp externo analizo el loop
        if(i==0)
          PresentarValor(getValCalculado(i),i);
          //Pasar_Numero(getValCalculado(i),i,DotNum[i]);	          // muestro canal 1
				if(i==1)
				  PresentarValor(getSetPoint(0),i);
			  	//Pasar_Numero(getSetPoint(0),i,DotNum[i]);			      // y el sp combinado
      #elif defined SENS_HUMEDAD
          if(i==0)                                    // canal 1 ? 
             PresentarValor(getValCalculado(i),i);
             //Pasar_Numero(getValCalculado(i),i,DotNum[i]);   //muestro la temp bs 
          if(i==1){                                   // canal 2 ?
            if(PRom[R_Sensor+1]==SENSOR_HUM)          //humedad ?
              PresentarValor(Humedad,i);
              //Pasar_Numero(Humedad,i,DotNum[i]);      //muestro la humedad  
            else
              PresentarValor(getValCalculado(i),i);
              //Pasar_Numero(getValCalculado(i),i,DotNum[i]);  // si no, muestro la temp bh 
          }
       #else
          PresentarValor(getValCalculado(i),i);
          //Pasar_Numero(getValCalculado(i),i,DotNum[i]);			// si no es, muestro normal
       #endif
      

    }

    else if (getEstatusResult(i)==ERR_UF)					                    // El REsultado de la linealización fue UF??
        PresentarMsj((char *)UF,i);
        //PasarASCII((char *)UF,i);							              // Mostrar UF
    else if (getEstatusResult(i)==ERR_OF)					                    // El REsultado de la linealización fue OF??
        PresentarMsj((char *)OF,i);
        //PasarASCII((char *)OF,i);							              // Mostrar OF
        
    if (show_main_text==TRUE)
       if(screen_cont<=MAIN_TEXT_TIME) 
          if (PRom[R_Stn+i]== Stc)
              PresentarMsj(main_text,i);
             //PasarASCII(main_text,i);                       // Mostrar DisplayInf
  }																													// fin del for de canales



 Mostrar_Proc=FALSE;  								                      // Sacar el flag de mostrar
}																														// fin del if de mostrar


            
/***********************************************************************************************************/
/*MANEJO DE LA PANTALLA PRINCIPAL EN APARATOS DE UN CANAL*/
/***********************************************************************************************************/

/*El display superior*/

#elif CANTIDAD_CANALES == 1 
 
 #ifdef LOBOV
   PresentarValor(Valor_Tmp,0);
   //Pasar_Numero(Valor_Tmp,0,DotNum[0]);
 
 #else 
  if (getEstatusResult(0)==ERR_OK){  							                        // El REsultado de la linealización fue OK??
    setDecimalesDys(displays,0,(byte)PRom[R_Decimales]);
    //DotNum[0]=(byte)PRom[R_Decimales];													// Pongo punto
      #ifdef pirani
      if(PRom[R_Sensor]==SENSOR_PIR)														// Convierto si es Pirani
      PresentarValorExpo(getValCalculado(0),0);
      //Pasar_Numero_Expo(getValCalculado(0),0,DotNum[0]);
      else
      #endif
      
 #ifdef HOLD
      mantenerIndicacion ();                 
      #elif defined(LUX) 
      if (PRom[R_Sensor]==SENSOR_LIN2 || PRom[R_Sensor]==SENSOR_LIN3 || PRom[R_Sensor]==SENSOR_LIN4 || PRom[R_Sensor]==SENSOR_JB || PRom[R_Sensor]==SENSOR_PT){
         if(PRom[R_RSL]==0)
           PresentarValor(valorAmostrar[0],0);
          //Pasar_Numero(valorAmostrar[0],0,DotNum[0]);
         else if(PRom[R_RSL]==1 && getDecimalesDys(displays,0)>=1)
           setDecimalesDys(displays,0,(getDecimalesDys(displays,0)-1));
           PresentarValor(valorAmostrar[0],0);
           //Pasar_Numero(valorAmostrar[0],0,(setDecimalesDys(displays,0,(getDecimalesDys(displays,0)-1))));
         else if(PRom[R_RSL]==2 && getDecimalesDys(displays,0)==1)
           setDecimalesDys(displays,0,(getDecimalesDys(displays,0)-1));
           PresentarValor(valorAmostrar[0],0);
          // Pasar_Numero(valorAmostrar[0],0,(DotNum[0]-1));
         else if(PRom[R_RSL]==2 && getDecimalesDys(displays,0)>=2)
           setDecimalesDys(displays,0,(getDecimalesDys(displays,0)-2));
           PresentarValor(valorAmostrar[0],0);
           //Pasar_Numero(valorAmostrar[0],0,(DotNum[0]-2)); 
      }else
         PresentarValor(valorAmostrar[0],0);
         //Pasar_Numero(valorAmostrar[0],0,DotNum[0]);
      
    #elif defined MMAX
      mantenerMaxIndicacion ();
    #else                                                                               
      PresentarValor(getValCalculado(0),0);
      //Pasar_Numero(getValCalculado(0),0,DotNum[0]);						        //Mostrar Display sup, Variable del Proceso
    #endif
  }
  else if (getEstatusResult(0)==ERR_UF)					                          // El Resultado de la linealización fue UF??
    PresentarMsj((char*)UF,0);
    //PasarASCII(UF,0);							                              // Mostrar UF
  else if (getEstatusResult(0)==ERR_OF)					                          // El REsultado de la linealización fue OF??
    PresentarMsj((char*)OF,0);
    //PasarASCII(OF,0);							                              // Mostrar OF
 #endif

/*El display inferior*/


     
 #if(defined(VF) && !defined(VF_BKR))
 
 if(PRom[R_MVF]==_VF) {
  
  if(getModeVF == ENDVF){
    set_MainText("Fin "); 
    show_main_text=TRUE;
    screen_cont=0;
   }else{
    show_main_text=FALSE;
    vitroFusionHandler();
   }

 }else{
  if(getModeVF == ENDVF){
    set_MainText("Fin "); 
    show_main_text=TRUE;
    screen_cont=0;
   }else{
    show_main_text=FALSE;
   }
 }
 
#elif defined(VF_BKR)

 if(PRom[R_MVF]==_VF) {
 
   if(getModeVF == ENDVF){
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
    PresentarValorExpo(Valor_Tmp,1);
    //Pasar_Numero_Expo(Valor_Tmp,1,DotNum[1]);
  else
    PresentarValor(Valor_Tmp,1);
    //Pasar_Numero(Valor_Tmp,1,DotNum[1]);                        // Mostrar DisplayInf

#elif defined (LOBOV)
PresentarMsj((char*)"SP 1",1);
 //PasarASCII("SP 1",1);
#else   

        																									// y nada mas
  if (show_main_text==TRUE&&screen_cont<=MAIN_TEXT_TIME ){  // Hay texto para mostrar?, es tiempo de mostrar?
        PresentarMsj(main_text,1);
        //PasarASCII(main_text,1);                                // Lo muestro en DisplayInf y chau
        
  }
  
  else{
      
      #ifdef programador
      if((int *)DirPar!=&SetPoint[R_SetPoint+0] || PRom[R_Programa]!=NO_PROGRAMA)													  // El amod es distinto del SP?
      #else
      if((int *)DirPar!=&SetPoint[R_SetPoint+0])
      #endif  
      
        //Pasar_Numero(*(int *)DirPar,1,DotNum[1]);               // Lo muestro en DisplayInf y chau
        PresentarValor(*(int *)DirPar,1);
      else if(PRom[R_Ver]==POT)        
            PresentarValor(dutytmp,1);
            //Pasar_Numero(dutytmp,1,1);                  //presento el valor de la potencia 
           else 
              #ifdef DOBLE_SP
                  PresentarValor(getSetPoint(0),1);
                  //Pasar_Numero(getSetPoint(0),1,DotNum[1]);
              #elif defined (VF)
                 if(PRom[R_MVF]==_SP)
                    PresentarValor(getSetPoint(0),1);
                   //Pasar_Numero(getSetPoint(0),1,DotNum[1]);                 // si nada de lo anterior,Mostrar Setpoint
              #elif defined (LUX) && defined (INTEGRADOR)
                   PresentarValor(getValCalculado(0),1);
                   //Pasar_Numero(getValCalculado(0),1,DotNum[1]);
              #elif defined (INTEGRADOR)
                  if (PRom[R_Sensor]==SENSOR_LIN2 || PRom[R_Sensor]==SENSOR_LIN3 || PRom[R_Sensor]==SENSOR_LIN4 || PRom[R_Sensor]==SENSOR_JB || PRom[R_Sensor]==SENSOR_PT){
                    if(PRom[R_RSL]==0)
                       PresentarValor(valorAmostrar[0],1);
                      //Pasar_Numero(valorAmostrar[0],1,DotNum[1]);
                     else if(PRom[R_RSL]==1 && getDecimalesDys(displays,0)>=1)
                        setDecimalesDys(displays,1,(getDecimalesDys(displays,1)-1));
                        PresentarValor(valorAmostrar[0],1);
                      //Pasar_Numero(valorAmostrar[0],1,(DotNum[1]-1));
                     else if(PRom[R_RSL]==2 && getDecimalesDys(displays,0)==1)
                        setDecimalesDys(displays,1,(getDecimalesDys(displays,1)-1));
                        PresentarValor(valorAmostrar[0],1);
                      //Pasar_Numero(valorAmostrar[0],1,(DotNum[1]-1));
                     else if(PRom[R_RSL]==2 && getDecimalesDys(displays,0)>=2)
                      setDecimalesDys(displays,1,(getDecimalesDys(displays,1)-2));
                        PresentarValor(valorAmostrar[0],1);
                      //Pasar_Numero(valorAmostrar[0],1,(DotNum[1]-2));
                  } else
                        PresentarValor(valorAmostrar[0],1);
                      //Pasar_Numero(valorAmostrar[0],1,DotNum[1]);
              
              #else 
                    
                   PresentarValor(Valor_Tmp,1);        
                   //Pasar_Numero(Valor_Tmp,1,DotNum[1]);                 // si nada de lo anterior,Mostrar Setpoint
                 
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
  
// T0QUE RÁPIDO
  if (Tecla=='r'){
   
    if(!save_parametros){
      SalirBox(Numericos);
    }else 
      SaveNow=DSave=TRUE;				
  }			
 
#else
  if (Tecla=='r')
    if(!save_parametros){
      SalirBox(Numericos);
      #ifdef programador
      if( PRom[R_Programa]!=NO_PROGRAMA)
        PtrTmp=&Segmento1.DirProc; 
      #endif
    }else 
      SaveNow=DSave=TRUE;				
#endif

#if (defined(VF) && !defined(VF101)) 

// T0QUE MANTENIDO
 if(PRom[R_MVF]==_SP)
  if (Tecla=='t')
    if(!save_parametros) 
      SalirBox(Numericos);
     else 
      SaveNow=DSave=TRUE;
 
#elif(defined(VF) && defined(VF101))   
  if (Tecla=='f')
   if(!save_parametros) 
    SalirBox(Numericos);
   else 
    SaveNow=DSave=TRUE; 


#elif(defined(DH102) && !defined(nico_1_10))
  if (Tecla=='t')
    if(!save_parametros) 
      SalirBox(Numericos);
     else SaveNow=DSave=TRUE;

#else
   if (Tecla=='f')
    if(!save_parametros) 
      SalirBox(Numericos);
    else 
      SaveNow=DSave=TRUE;

#endif

#ifdef VF_PROG

	if (Tecla== 'k' || getModeVF == ENDVF) 
	  Exit();	 // Boton de Exit
#endif	
}




////////Funciones para procesar boxes Especiales/////////////////////// 


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
	
	  PresentarMsj(PtrBox.Num->TxtDpy,1);
		PresentarValor(Valor_Tmp,0);
		//PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
		//Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
  }
  
  handlerTeclasUPoDown(Numericos);
 
/////////////////////// T0QUE RÁPIDO //////////////////////////  
  if(Tecla=='r'){
    if (save) 
	       setFecha(Valor_Tmp,date.Month,date.Day);
	       
	   SalirBox(Numericos);
    return;
  }
  
  /////////////////////// TOQUE MANTENIDO ////////////////////////

if (Tecla=='f')
  SalirBox(Numericos);	 // funcion de salida

/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') 
	  Exit();	 // Boton de Exit
  
  

}

void MonthHandler(void){
 
 if(FstTime){
    FstTime=FALSE;  // sacar primera vez
		save=FALSE; /*Reseteo flag de mandar a grabar*/
		PtrBox.Num=(Numerico*)PtrTmp;	 // Poner el puntero PtrBox.Num con el valor del Box actual
    Valor_Tmp= date.Month;
	
	  PresentarMsj(PtrBox.Num->TxtDpy,1);
		PresentarValor(Valor_Tmp,0);
		//PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
		//Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
  }
  
    handlerTeclasUPoDown(Numericos);
	 	
	 
/////////////////////// T0QUE RÁPIDO //////////////////////////  
  if(Tecla=='r'){
    if (save) 
	       setFecha(date.Year,Valor_Tmp,date.Day);
	  SalirBox(Numericos);
    return;
  }
  
  /////////////////////// TOQUE MANTENIDO ////////////////////////

if (Tecla=='f')
  SalirBox(Numericos);	 // funcion de salida

/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') 
	  Exit();	 // Boton de Exit
  
  
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
			setDecimalesDys(displays,0,PtrBox.Num->Dot);
			//DotNum[0]=PtrBox.Num->Dot;		
		  
		  PresentarMsj(PtrBox.Num->TxtDpy,1);
		  PresentarValor(Valor_Tmp,0);
		  //PasarASCII(PtrBox.Num->TxtDpy,1);     //DisplayInf
			//Pasar_Numero(Valor_Tmp,0,0);		           //DisplaySup
					
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
	PresentarValor(Valor_Tmp,0);
	//Pasar_Numero(Valor_Tmp,0,0);		           //DisplaySup
  save=TRUE;
}
	
/////////////////////// T0QUE RÁPIDO //////////////////////////

	if (Tecla=='r'){
	      if (save)
	        setFecha(date.Year,date.Month,Valor_Tmp);
	     SalirBox(Numericos);
	      
	}
		
/////////////////////// TOQUE MANTENIDO ////////////////////////

if (Tecla=='f')
  	SalirBox(Numericos);
/////////////////////// EXIT //////////////////////////

if (Tecla== 'k') 
  Exit();

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
	PresentarValor(Valor_Tmp,0);
	//Pasar_Numero(Valor_Tmp,0,2);		           //DisplaySup
  save=TRUE;
}
	
	if (FstTime){
	    TIMEREC time;
	    
		  FstTime=FALSE;	
			
			PtrBox.Num=(Numerico*)PtrTmp;
			 
			
	    getTiempo(&time);
			Valor_Tmp = time.Hour*100+time.Min;
			setDecimalesDys(displays,0,PtrBox.Num->Dot);
			//DotNum[0]=PtrBox.Num->Dot;		
		  
		  PresentarMsj(PtrBox.Num->TxtDpy,1);
		  setDecimalesDys(displays,0,2);
		  PresentarValor(Valor_Tmp,0);
		  //PasarASCII(PtrBox.Num->TxtDpy,1);     //DisplayInf
			//Pasar_Numero(Valor_Tmp,0,2);		           //DisplaySup
					
	}
	
/////////////////////// T0QUE RÁPIDO //////////////////////////

	if (Tecla=='r'){
	  if (save) 
	    setTiempo(Valor_Tmp/100,Valor_Tmp%100,0);
	  SalirBox(Numericos);
	}
		
/////////////////////// TOQUE MANTENIDO ////////////////////////

if (Tecla=='f')	
  SalirBox(Numericos);
/////////////////////// EXIT //////////////////////////

if (Tecla== 'k') 
  Exit();


}

#endif

     


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
				 
		AnalisisPuntoDecimal();
		
// mando mensajes
		
		PresentarMsj(PtrBox.Num->TxtDpy,1);				
		//PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf

    #ifdef  pirani
    if(PRom[R_Sensor]==SENSOR_PIR)
      PresentarValorExpo(Valor_Tmp,0);
      //Pasar_Numero_Expo(Valor_Tmp,0,DotNum[0]);
    else
      PresentarValor(Valor_Tmp,0);
      //Pasar_Numero(Valor_Tmp,0,DotNum[0]);						 //Mostrar Variable del Proceso
		#else
      PresentarValor(Valor_Tmp,0);
      //Pasar_Numero(Valor_Tmp,0,DotNum[0]);						 //Mostrar Variable del Proceso
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
		    if (Valor_Tmp+sumador<(*PtrBox.Num->sDato->Sup)) 
		      Valor_Tmp+=sumador; // El Buffer es menor que el LimiteSup?? Si-> Incrementar Buffer
				else 
				  Valor_Tmp=(*PtrBox.Num->sDato->Sup);						 // No-> Poner Buffer en maximo valor
				
	};
	
	/* proceso Tecla down */
	if (Tecla=='d'){		  					      // Fue presionada una Tecla Down???
				if (Valor_Tmp-sumador>(*PtrBox.Num->sDato->Inf)) 
				  Valor_Tmp-=sumador;		// El Buffer es mayor que el LimiteInf?? Si-> Decrementar Buffer
				else                    
				  Valor_Tmp=(*PtrBox.Num->sDato->Inf);	// No-> Poner Buffer en el minimo valor						
		}
	
	
	 	if(PRom[R_Sensor]==SENSOR_PIR)
     PresentarValorExpo(Valor_Tmp,0);
     // Pasar_Numero_Expo(Valor_Tmp,0,DotNum[0]);
    else
      PresentarValor(Valor_Tmp,0);
      //Pasar_Numero(Valor_Tmp,0,DotNum[0]);						 //Mostrar Variable del Proceso
	
		save=TRUE;							// Grabar parametros

}

/////////////////////// T0QUE RÁPIDO o MANTENIDO //////////////////////////


  SalirBox(Numericos);		// funcion de salida			

		
/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') 
	  Exit();	 // Boton de Exit
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
    PresentarValor(Valor_Tmp,0);
    //Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
  }
}


/* Procesador del box de Condicion de emergencia general*/
/********************************************************/

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

   
   for(i=0;i<TAMANIO_GENERAL;i++)
    EscribirWord((word)(&Programa[programa_ingresado].Gral)+i*2,*((word*)&BufferGeneral+i)); 
	 
	 
   SaveNow=TRUE;

  }  
    
}



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
      
      PresentarMsj(end,0);
      //PasarASCII(end,0);     //DisplaySup
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


/* Procesador del box de texto de eleccion del tipo de segmento*/
/*************************************************************/ 

void TipoSegHandler(void){
  TxtHandler();
  if(Tecla=='r' && Valor_Tmp==End){
    TSegmentos * dir;
    
    dir = (TSegmentos *) &Programa[programa_ingresado].Segmento[0];
    dir+= segmento_ingresado-1;
    
    EscribirWord((word)&dir->tipo_segmento,End);  
    
    
  }
}

/* Procesador del box del titulo del Ptogramador viejo */
/*******************************************************/


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
    PresentarMsj(new_text,1);
    //PasarASCII(new_text,1);
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

   
    for(i=0;i<TAMANIO_GENERAL;i++)
      EscribirWord(((word)&Programa[programa_ingresado].Gral)+i*2,*((word*)&BufferGeneral+i)); 
   
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
    PresentarMsj(new_text,1);
    //PasarASCII(new_text,1);
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
	  
	  PresentarMsj(PtrBox.Txt->TxtDpy,1);
	  PresentarMsj(PtrBox.Txt->TxtDpy0[Valor_Tmp],0);
		//PasarASCII(PtrBox.Txt->TxtDpy,1);             //Mostrar DisplayInf
		//PasarASCII(PtrBox.Txt->TxtDpy0[Valor_Tmp],0);  //Mostrar DisplaySup		 

  }


if(stateAdq!= ADQ_FULL){
  
  handlerTeclasUPoDown(Textuales);
}

 // T0QUE RÁPIDO O MANTENIDO
 SalirBox(Textuales);	 // funcion de salida


/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') 
	  Exit(); // Boton de Exit
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
		
		PresentarMsj(PtrBox.Txt->TxtDpy,1);
	  PresentarMsj(PtrBox.Txt->TxtDpy0[Valor_Tmp],0);
		//PasarASCII(PtrBox.Txt->TxtDpy,1);             //Mostrar DisplayInf
		//PasarASCII(PtrBox.Txt->TxtDpy0[Valor_Tmp],0);  //Mostrar DisplaySup		 

  }


  // KeyEdgeS UP Y DOWN
  handlerTeclasUPoDown(Textuales);

  // T0QUE RÁPIDO O MANTENIDO
  SalirBox(Textuales);	 // funcion de salida


/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') 
	  Exit(); // Boton de Exit
}

#endif

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
				 
		AnalisisPuntoDecimal();
      
      if(PRom[R_RSL]==1 && getDecimalesDys(displays,0)>=1)
       setDecimalesDys(displays,0,getDecimalesDys(displays,0)-1);
      
      if(PRom[R_RSL]==2 && getDecimalesDys(displays,0)==1)
       	setDecimalesDys(displays,0,getDecimalesDys(displays,0)-1);
      
      if(PRom[R_RSL]==2 && getDecimalesDys(displays,0)>=2)
       	setDecimalesDys(displays,0,getDecimalesDys(displays,0)-2);
      
       
// mando mensajes
						
		  PresentarMsj(PtrBox.Num->TxtDpy,1);
      PresentarValor(Valor_Tmp,0);
	  
		  //PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
			//Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
	}
	
  NumHandler();
}

#endif



#ifdef LOBOV
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
				 
	AnalisisPuntoDecimal();
	
// mando mensajes
			
			PresentarMsj(PtrBox.Num->TxtDpy,1);			
		  PresentarValor(Valor_Tmp,0);
		  //PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
			//Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
	}
	
  if ( getEstatusResult(0)==ERR_OK){  							                        // El REsultado de la linealización fue OK??
      	setDecimalesDys(displays,0,(byte)PRom[R_Decimales]);
   // DotNum[0]=(byte)PRom[R_Decimales];													// Pongo punto
      
      #ifdef pirani
      if(PRom[R_Sensor]==SENSOR_PIR)														// Convierto si es Pirani
        PresentarValorExpo(getValCalculado(0),0);
        //Pasar_Numero_Expo(getValCalculado(0),0,DotNum[0]);
      else
      #endif
      
      #ifdef HOLD
      mantenerIndicacion ();                 
      #elif defined MMAX
      mantenerMaxIndicacion ();
      #else                                                       
      PresentarValor(getValCalculado(0),0);
      //Pasar_Numero(getValCalculado(0),0,DotNum[0]);						        //Mostrar Display sup, Variable del Proceso
      #endif
  }
  else if (getEstatusResult(0)==ERR_UF)					                          // El Resultado de la linealización fue UF??
    PresentarMsj((char*)UF,0);
    //PasarASCII(UF,0);							                              // Mostrar UF
  else if (getEstatusResult(0)==ERR_OF)					                          // El REsultado de la linealización fue OF??
   PresentarMsj((char*)OF,0);
   //PasarASCII(OF,0);			

/////////////////////// T0QUE RÁPIDO o MANTENIDO //////////////////////////

   SalirBox(Numericos);		// funcion de salida

/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') 
	  Exit();	 // Boton de Exit
}

#endif

#ifdef VF
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
				 
AnalisisPuntoDecimal();

// mando mensajes
			
			PresentarMsj(PtrBox.Num->TxtDpy,1);			
		  PresentarValor(Valor_Tmp,0);
		 //PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
		 //Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
	}
	
  handlerTeclasUPoDown(Numericos);

/////////////////////// T0QUE RÁPIDO o MANTENIDO //////////////////////////

	if (Tecla=='r' || Tecla=='f')	{
 		  		  
  		  EscribirSiempre(PtrBox.Num->sDato,Valor_Tmp);
        save = FALSE;
  		  PresentarMsj((char*)"    ",1);
  		 // PasarASCII("    ",1);   //borro la pantalla 
        SalirBox(Numericos);		// funcion de salida
			
	}
		

/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') 
	  Exit();	 // Boton de Exit
}

/*******************************************************************************/

void NumHandlervfPrincipal(void){
    
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
AnalisisPuntoDecimal();

// mando mensajes
						
		  PresentarMsj(PtrBox.Num->TxtDpy,1);
		  PresentarValor(Valor_Tmp,0);
		  //PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
			//Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
	}
	
  handlerTeclasUPoDown(Numericos);

/////////////////////// T0QUE RÁPIDO //////////////////////////

	if (Tecla=='r')	{
 		  		  
  		  EscribirSiempre(PtrBox.Num->sDato,Valor_Tmp);
  		  save = FALSE; 
  		  PresentarMsj((char*)"    ",1);
  		  //PasarASCII("    ",1);   //borro la pantalla 
  		  getCantidadEtapasVF=PRomVF[getNProgVF];
        SalirBox(Numericos);		// funcion de salida
			
	}
		
/////////////////////// TOQUE MANTENIDO ////////////////////////

if (Tecla=='f'){
  
  EscribirSiempre(PtrBox.Num->sDato,Valor_Tmp);
  save = FALSE;
  SalirBox(Numericos);	 // funcion de salida
}
/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') 
	  Exit();	 // Boton de Exit
}



/* Procesador de los boxes numéricos*/
/************************************/

void NumHandlervfInicial(void){
    
    
    static bool unaVez = FALSE;
    static bool fTimer = FALSE;
    
    is_box_principal=2;
    
   
 if(!unaVez){
    unaVez=TRUE;
    #ifndef HD90
    PresentarMsj(tipoEquipo,1);
    //PasarASCII(tipoEquipo,1);
    #endif
    PresentarMsj(numver,0);
    //PasarASCII(numver,0);
    InitTimeOut(7000,&fTimer,ONETIME);
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
AnalisisPuntoDecimal();

// mando mensajes
			
			PresentarMsj(PtrBox.Num->TxtDpy,1);			
		  PresentarValor(Valor_Tmp,0);
		  //PasarASCII(PtrBox.Num->TxtDpy,1);     //Mostrar DisplayInf
			//Pasar_Numero(Valor_Tmp,0,DotNum[0]);		          //Mostrar DisplaySup
					
	    SaveNow=TRUE;
	}
	
  if (getEstatusResult(0)==ERR_OK){  							                        // El REsultado de la linealización fue OK??
    	setDecimalesDys(displays,0,(byte)PRom[R_Decimales]);
    //DotNum[0]=(byte)PRom[R_Decimales];													// Pongo punto
      
      #ifdef pirani
      if(PRom[R_Sensor]==SENSOR_PIR)														// Convierto si es Pirani
        PresentarValorExpo(getValCalculado(0),0);
        //Pasar_Numero_Expo(getValCalculado(0),0,DotNum[0]);
      else
      #endif
      
      #ifdef HOLD
      mantenerIndicacion ();                 
      #else                                                       
      PresentarValor(getValCalculado(0),0);
      //Pasar_Numero(getValCalculado(0),0,DotNum[0]);						        //Mostrar Display sup, Variable del Proceso
      #endif
  }
  else if (getEstatusResult(0)==ERR_UF)					                          // El Resultado de la linealización fue UF??
   PresentarMsj((char*)UF,0);
   //PasarASCII(UF,0);							                              // Mostrar UF
  else if (getEstatusResult(0)==ERR_OF)					                          // El REsultado de la linealización fue OF??
    PresentarMsj((char*)OF,0);
    //PasarASCII(OF,0);			

/////////////////////// T0QUE RÁPIDO //////////////////////////

	if (Tecla=='r')	{
        
    SalirBox(Numericos);		// funcion de salida
			
	}
		
/////////////////////// TOQUE MANTENIDO ////////////////////////

#if(defined(DH102) && !defined(nico_1_10))
  if (Tecla=='t')
    if(!save_parametros) 
      SalirBox(Numericos);
    else 
      SaveNow=DSave=TRUE;

#else
   if (Tecla=='f')
    if(!save_parametros) 
      SalirBox(Numericos);
      else 
        SaveNow=DSave=TRUE;

#endif


/////////////////////// EXIT //////////////////////////

	if (Tecla== 'k') 
	  Exit();	 // Boton de Exit
}


#endif


/////////////////// Funcion del Main (Otra razon para usar Clases)///////////

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


///////////////////////////Funciones internas /////////////
void AnalisisPuntoDecimal(){
  	
  	setDecimalesDys(displays,1,0);
  	
  	if ((PtrBox.Num->Dot)>=DECIMALES_CANAL1) // El punto decimal depende del Sensor??
      setDecimalesDys(displays,0,(byte)PRom[R_Decimales+PtrBox.Num->Dot-DECIMALES_CANAL1]);
      //DotNum[0]=(byte)PRom[R_Decimales+PtrBox.Num->Dot-DECIMALES_CANAL1];
		else											 // sino
			setDecimalesDys(displays,0,PtrBox.Num->Dot);
			//DotNum[0]=PtrBox.Num->Dot; // Cargar PD que indica el Box		

}



void handlerTeclasUPoDown(TipoBox tipo){
   // KeyEdgeS UP O DOWN
  if ((Tecla=='u') || (Tecla=='d')){ // Fue presionada una Tecla UP o Down???
  
    if(tipo == Numericos){
      /* proceso Tecla UP */
      if (Tecla=='u'){									 // Fue presionada una Tecla UP ???
      	    
      	    if (Valor_Tmp<(*PtrBox.Num->sDato->Sup))
      	      Valor_Tmp++; // El Buffer es menor que el LimiteSup?? Si-> Incrementar Buffer
      			else 
      			  Valor_Tmp=(*PtrBox.Num->sDato->Sup);						 // No-> Poner Buffer en maximo valor	
      }

      /* proceso Tecla down */
      if (Tecla=='d'){		  					      // Fue presionada una Tecla Down???
      		  
      			if (Valor_Tmp>(*PtrBox.Num->sDato->Inf)) 
      			  Valor_Tmp--;		// El Buffer es mayor que el LimiteInf?? Si-> Decrementar Buffer
      			else 
      			  Valor_Tmp=(*PtrBox.Num->sDato->Inf);	// No-> Poner Buffer en el minimo valor			

      	}
      	
      	PresentarValor(Valor_Tmp,0);
      	//Pasar_Numero(Valor_Tmp,0,DotNum[0]); // Mostrar DisplaySup
      	save=TRUE;							// Grabar parametros
	
    }
    
   if(tipo == Textuales){
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
        PresentarMsj(PtrBox.Txt->TxtDpy0[Valor_Tmp],0);
        //PasarASCII(PtrBox.Txt->TxtDpy0[Valor_Tmp],0);     // Mostrar DisplaySup

    		save=TRUE;																				 // Activar Flag de guardar valor
		
   }
   
   if(tipo == Principal){
      /* proceso Tecla UP */
      if (Tecla=='u'){									 // Fue presionada una Tecla UP ???
      	    
      	    if (Valor_Tmp<(*PtrBox.Num->sDato->Sup))
      	      Valor_Tmp++; // El Buffer es menor que el LimiteSup?? Si-> Incrementar Buffer
      			else 
      			  Valor_Tmp=(*PtrBox.Num->sDato->Sup);						 // No-> Poner Buffer en maximo valor	
      }

      /* proceso Tecla down */
      if (Tecla=='d'){		  					      // Fue presionada una Tecla Down???
      		  
      			if (Valor_Tmp>(*PtrBox.Num->sDato->Inf)) 
      			  Valor_Tmp--;		// El Buffer es mayor que el LimiteInf?? Si-> Decrementar Buffer
      			else 
      			  Valor_Tmp=(*PtrBox.Num->sDato->Inf);	// No-> Poner Buffer en el minimo valor			

      	}
      	
      	
    }
   
   
    
}

}

///////////////////////////////////////////////////////////
void Exit(void){	// Funcion Boton de Exit 
		ResetScroll();
		flagTimeReturn=TRUE;
}

void SalirBox(TipoBox tipo){		// Funcion de Salida por toque mantenido o rapido
 PunteroF Proximo;	/* Proximo estado con toque sostenido */

 if (Tecla=='r' || Tecla=='f' || Tecla=='t'){
  
  if(tipo == Numericos){
     if(Tecla=='r'){ 
      	if (save)							
    		{			    		  
    		  Escribir(PtrBox.Num->sDato,Valor_Tmp);
    		  bufferNro = Valor_Tmp;
    		  buffersDato = PtrBox.Num->sDato;
    		}
          
      if (PtrBox.Num->VdeNavegacion== NULL)// Existe variable selectora de los Proximos Estados??
    		 Proximo=(PunteroF)PtrBox.Num->Teclar; // Cargar proximo Box por toque sostenido
    	else  
    	  Proximo=PtrBox.Num->Teclar[*(PtrBox.Num->VdeNavegacion)];		 // Cargar proximo Box por toque rapido
     }else  
      Proximo=PtrBox.Num->Teclaf;  // Cargar proximo Box por toque sostenido
      
    }
    
  if(tipo == Titulos){
    if (Tecla=='r'){
  	    if (PtrBox.Tit->VdeNavegacion==NULL)									// Existe variable selectora del proximo estado	 ???
   		  Proximo=(PunteroF)PtrBox.Tit->Teclar; // Cargar proximo Box por toque rapido
  	    else 										 //no hay variable selectora
  		  Proximo=PtrBox.Tit->Teclar[*(PtrBox.Tit->VdeNavegacion)];	 // Cargar proximo Box por toque rapido
  	  }else
  		Proximo=PtrBox.Tit->Teclaf;	 // Cargar proximo Box por toque sostenido
  }

  if(tipo == Textuales){
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
    }else  
      Proximo=PtrBox.Txt->Teclaf;  // Cargar proximo Box por toque sostenido

  }
    
  	if (Proximo!=NULL) {	// Se cargo un box existente???  
  		  {
  		  #ifdef VF_PROG
  		  if(Proximo==&Principal1.DirProc && getModeVF != ENDVF)
  		    Proximo=&Principal1_VF_PROG.DirProc;
  		  #endif
  		  ResetScroll();
  		  PtrTmp=Proximo;  //Cargar puntero a la funcion correspondiente (TitleHandler, NumHandler, etc.)
  		  FstTime=TRUE;		 //Setear Primera vez
  		 
  		  }
  	
  	}
  }
}



//////////////////////////////funciones de accion para el ingreso en los boxes/////////

// VARIABLES QUE UTILIZAN LOS BOXES
//////////////////////////////////
#ifdef programador
/* Funcion de accion para Correr un programa*/
/********************************************/

byte CorrerPrograma(int iPrograma,byte chan){

  
  #ifdef prog_viejo
    runPrograma(chan,(byte)iPrograma-1,0,0);
  #else
    runPrograma(chan,(byte)iPrograma,0,0);
  #endif		    

	Lim_Segmento2[chan]=get_Lim_Seg(iPrograma-1);
	#ifdef prog_viejo
  Lim_Segmento2[chan]++;
  #endif														 
  if (chan == 0 && iPrograma==NO_PROGRAMA)
    set_MainText("");

  return ERR_OK;
}
 
byte CambiarSegmento(int iSegmento,byte chan){

 if(iSegmento>0)
 #ifdef prog_viejo
    runPrograma(chan,PRom[R_Programa+chan]-1,iSegmento-OFFSET_SEG1,0);
 #else
    runPrograma(chan,PRom[R_Programa+chan],iSegmento-OFFSET_SEG1,0);
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

#endif

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
  
    main_text[0]='S';
    main_text[1]='t';
    main_text[2]=' ';
    main_text[3]='0';
    show_main_text=TRUE;
  } else 
    show_main_text=FALSE;
  
  return ERR_OK;		 
}



void isTimeReturnPrincipal(){
  ///////// veo si vuelvo a la pantalla principal/////////////
	  
	  #ifdef VF_PROG    
	  if(flagTimeReturn && PtrTmp!=&Principal1.DirProc && (getModeVF == ENDVF || PtrTmp!=&Principal1_VF_PROG.DirProc))		// PAsaron 10 segundos fuera de la pantalla principal y estando en FIN??
	  {
	   if(getModeVF != ENDVF){
	     PresentarMsj((char*)"    ",1);
	     //PasarASCII("    ",1);   //borro la pantalla una ves 
	     ResetScroll();
	     PtrTmp=&Principal1_VF_PROG.DirProc;											//Volver a la pantalla principal
	     FstTime=TRUE;
	   }else{
	     PresentarMsj((char*)"    ",1);
	     //PasarASCII("    ",1);   //borro la pantalla una ves 
	     ResetScroll();
	     PtrTmp=&Principal1.DirProc;											//Volver a la pantalla principal
	     FstTime=TRUE;
	   }
	  }
	  #else
	  if(flagTimeReturn && PtrTmp!=&Principal1.DirProc)		// PAsaron 10 segundos fuera de la pantalla principal??
	  { 
	    PtrTmp=&Principal1.DirProc;											//Volver a la pantalla principal
	    FstTime=TRUE;
	  }
	  #endif     	 

}