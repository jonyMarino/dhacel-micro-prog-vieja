/* 


PARAMETROS de colada caliente 

Los parametros de colada caliente son:

Pss1      Potencia de soft start en %
Tss1      Tiempo de soft start en segundos
SPlim1    Temperatura de soft start, umbral de soft start

SPlim2    Limite de temperatura al encender para dar soft start

SPst1     Temperatura de selftune, umbral de selftune  

SPsb1     Temperatura  de standby 

Indf      Valor default de integral en caso de falla de la autosintonia
Drdf      Valor default de derivada en caso de falla de la autosintonia
Abdf      Valor default de ancho de banda en caso de falla de la autosintonia

SPss1     Temperatura a la que se realiza el softstart


FUNCIONES de colada caliente

sst_cont    Cuenta demora inicial, mide temp, determina si va o no sst, da la pot de salida, da el sp
            del sst y cuenta tiempo de sst (ES PARTIDA SUAVE POR POT Y POR TEMP)
            
spst_cont   Cambia el setpoint durante el standby

manual_cont Controla el manual por toggle y por sbreak

stn_cont    Controla la autosintonia por start y por demanda




El boton Reset togglea entre manual y cal            

*/
#include "Mydefines.h"
#include "cnfbox.h"
#include "boxes.h"
#include "Programador.h"
#include "PE_Types.h"
#include "Sensores.h"
#include "boxescal.h"
#include "boxescolcal.h"
#include "paramdefines.h"
#include "In1.h"
#include "Parametros.h"

extern bool show_main_text;

int flagsst = 1;


#pragma CONST_SEG DEFAULT 
#pragma STRING_SEG DEFAULT


extern const Titulo Colcal;
extern const Numerico Pss1;
extern const Numerico Tss1;
extern const Numerico SPlim1;
extern const Numerico SPlim2;
extern const Numerico SPst1;
extern const Numerico SPsb1;
extern const Numerico Indf;
extern const Numerico Drdf;
extern const Numerico Abdf;
extern const Textual Ccalstn;
 
/**************************/

#ifdef CCAL

 const Titulo Colcal={ 
      TitleHandler,						                                  // funcion que procesa al box*/
			"ccL ",									                                  // nombre display
			"PArAMEtroS coLcAL  ",									                                  // nombre display
			NULL,							                                        // parametro que modifica.
			(PunteroF*)&SPsb1.DirProc,&Calibracion.DirProc            // Proximos estados
			};

  /* sp stby, setpoint de espera */
const Numerico SPsb1={
      NumHandler,					                                      // funcion que procesa al box*/
      &Parametros[R_SPsb],						                          // direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"SP ESPErA <C   ",							                              // nombre display
			DECIMALES_CANAL1,            									            // punto decimal a enc */
			NULL,					                                            // parametro que modifica.
 			(PunteroF*)&Pss1.DirProc,NULL                             // Proximos estados
			};

 /* pot partida suave*/
const Numerico Pss1={
      NumHandler,					                                      // funcion que procesa al box*/
      &Parametros[R_Pss],						                            // direccion en la E2Prom*/
			"Pot PArt SuAuE   ",							                        // nombre display
			1,									                                      // punto decimal a enc */
			NULL,					                                            // parametro que modifica.
 			(PunteroF*)&Tss1.DirProc,NULL                             // Proximos estados
			};

  /* tie partida suave*/
const Numerico Tss1={
      NumHandler,					                                      /* funcion que procesa al box*/
      &Parametros[R_Tss],						                            /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"tie PArt SuAuE SEG   ",							                        //nombre display
			0,									                                      /* punto decimal a enc */
			NULL,					                                            //parametro que modifica.
 			(PunteroF*)&SPlim1.DirProc,NULL                           //Proximos estados
			};

 /* sp partida suave, umbral*/
const Numerico SPlim1={
      NumHandler,					                                      /* funcion que procesa al box*/
      &Parametros[R_SPlim],						                          /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"SP PArt SuAuE <C   ",						                            //nombre display
			DECIMALES_CANAL1,            									            /* punto decimal a enc */
			NULL,					                                            //parametro que modifica.
 			(PunteroF*)&SPlim2.DirProc,NULL                           //Proximos estados
			};


 /* sp stn */
const Numerico SPlim2={
      NumHandler,					                                      /* funcion que procesa al box*/
      &Parametros[R_SPlimst],						                        /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"PorcEnt SP AutoSint   ",					                        //nombre display
			DECIMALES_CANAL1,            			                        /* punto decimal a enc */
			NULL,					                                            //parametro que modifica.
 			(PunteroF*)&SPst1.DirProc,NULL                            //Proximos estados
			};




  /* umbral selftun */
const Numerico SPst1={
      NumHandler,					                                      /* funcion que procesa al box*/
      &Parametros[R_SPst],						                          /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"uMbrAL AuSint init <C   ",							                  //nombre display
			DECIMALES_CANAL1,            			                        /* punto decimal a enc */
			NULL,					                                            //parametro que modifica.
 			(PunteroF*)&Indf.DirProc,NULL                             //Proximos estados
			};


  /* in def */
const Numerico Indf={
      NumHandler,					                                      /* funcion que procesa al box*/
      &Parametros[R_Indf],						                          /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"in iniciAL   ",							                            //nombre display
			0,            									                          /* punto decimal a enc */
			NULL,					                                            //parametro que modifica.
 			(PunteroF*)&Drdf.DirProc,NULL                             //Proximos estados
			};
  /* dr def */
const Numerico Drdf={
      NumHandler,					                                      /* funcion que procesa al box*/
      &Parametros[R_Drdf],						                          /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"dr iniciAL   ",							                            //nombre display
			0,            									                          /* punto decimal a enc */
			NULL,					                                            //parametro que modifica.
 			(PunteroF*)&Abdf.DirProc,NULL                             //Proximos estados
			};

  /* ab def */
const Numerico Abdf={
      NumHandler,					                                      /* funcion que procesa al box*/
      &Parametros[R_Abdf],						                          /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"Ab iniciAL <C   ",							                          //nombre display
			DECIMALES_CANAL1,            									            /* punto decimal a enc */
			NULL,					                                            //parametro que modifica.
 			(PunteroF*)&Principal1.DirProc,NULL                       //Proximos estados
			};


/* acceso rapido a stn */

 const char * const Selftccal[2]={
      "no  ",									
			"Si  ",
};

const Textual Ccalstn=
      {TxtHandler,						                                  /* funcion que procesa al box*/
			&Parametros[R_Stn],							                          /* direccion en la E2Prom - el EEProm Start, if FALSE no guarda valor*/
			"iniciAr AutoSint   ",									                  //nombre display
			Selftccal,                                                // Array donde estan los textos
			NULL,						                                          //parametro que modifica.
 			(PunteroF*)&C1SP.DirProc,NULL				                      //enter rap,enter mant			
      };

/* funcion que monitorea en colada caliente. Entra 1 vez por segundo */

void monit_colcal(void){


#define DELAYSTART 30

static int timer_ini = 0;
static int timer_sst = 0;



// Esta parte ve la condicion inicial

switch (flagsst) {

// estoy encendiendo, espero 30 segundos
  case _STRT:

    setValCalculado(0,0);
    setEstatusResult(ERR_OK,0);
    
    if(timer_ini<DELAYSTART)                                    
      timer_ini++;
    else															                          // no hago nada durante 30 seg
      flagsst = _TST;																						// y voy a medir la temp inicial
    break;  

// pasaron 30 segundos, tengo que dar sst?
  case _TST:

    if(getValCalculado(0)<PRom[R_SPst] && PRom[R_SPst]!=0){             // voy a tener que dar autosint?																																
      EscribirWord((word)&PRom[R_H1],PRom[R_Abdf]);							// si es que si, pongo valores ini
	    EscribirWord((word)&PRom[R_IN1],PRom[R_Indf]);
	    EscribirWord((word)&PRom[R_DR1],PRom[R_Drdf]);
    }

    if(getValCalculado(0)<PRom[R_SPlim] && PRom[R_SPlim]!=0){
      flagsst = _SST;																						// menor que umbral y !=0, doy part suave
      }
    else 
      flagsst = _TSN;																						// mayor que umbral, veo el stn
    break;
    
// si, tengo que dar sst          
  case _SST:

  //FUNCIONES DEL SST  
    
    if(timer_sst<PRom[R_Tss])																		// cuento tiempo de part suave
      timer_sst++;
    else{
      flagsst = _TSN;																						// termine, voy a ver sst
};
		break;
		
// tengo que dar stn?    
  case _TSN:
    if(getValCalculado(0)<PRom[R_SPst] && PRom[R_SPst]!=0){	            // decido si tengo que dar autosint
  		flagsst = _WSN;                                           // y voy a esperar para autosint
    } 
    else 
    flagsst = _END; 
		break;
		
// si, tengo que dar, espero llegar al valor    
  case _WSN:
    if((long)getValCalculado(0) > (long)PRom[R_SPlimst]*PRom[R_SetPoint]/1000){
         EscribirWord((word)&PRom[R_Stn],Stc);
    		 flagsst = _END;}
   };
    

// esta parte pone manual con sens abierto

if(getEstatusResult(0)==ERR_OF&&PRom[R_Tsalcont]!=_man){
    EscribirWord((word)&PRom[R_Tsalcont],_man);
    };

// y esta saca el stn si pongo manual

if(PRom[R_Tsalcont]==_man)
  if(PRom[R_Stn]!=St_OFF)
		 EscribirWord((word)&PRom[R_Stn],St_OFF);


// esta parte maneja los mensajes

if(flagsst == _STRT)
   set_MainText("iniciAndo   "); 
else if(PRom[R_Tsalcont]==_man){
   flagsst = _END;
   set_MainText("MAnuAL   ");}
else if(flagsst == _SST)
   set_MainText("PArt SuAvE   ");
else if(flagsst == _WSN)
   set_MainText("ESPErA AutoSin  ");
else if (!In1_GetVal())
   set_MainText("ESPErA   "); 
else if(PRom[R_Stn]!= St_OFF) {
}
else   set_MainText("");

}

    
#endif
