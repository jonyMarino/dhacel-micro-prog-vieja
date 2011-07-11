#include "Cpu.h"
#include "Events.h"
#include "Mydefines.h"
#include "cnfbox.h"
#include "boxes.h"
#include "ADC.h"
#include "Programador.h"
#include "TimerOld.h"
#include "display.h"
#include "Timer.h"
#include "MethodTimer.h"
#include "Comunicacion.h"
#include "MethodContainer.h"
#include "FuncionVF.h" 
#include "vfboxes.h"
#include "vprop.h"
#include "Parametros.h"
#include "Medicion.h"

#define CUENTA_1SEG 1000

typedef struct{
  word cuenta;
  word tiempo;
  byte tipo;
  bool * flag;
} T_CONTADOR;

const int ADTIME = AD_TIME_EN_MILI_SEGUNDOS;



byte pwsn=0;
int pwsn1=0;
int tstpw=0;

#ifdef VPROP
int vpout;
#endif

word CycleCont;
word screen_cont;
word tiempo_refresco_numeroram;
word TiempoProgramaCh[CANTIDAD_SAL_CONTROL];
#ifdef jony_15_08
bool PwsnError=TRUE;	 //al iniciar no se permite grabar en la FLASH
#endif
bool DSave=TRUE;
extern dword St_Counter[CANTIDAD_SAL_CONTROL+CANTIDAD_SAL_ALARMA];

extern bool save_parametros,SaveNow;
extern bool AS1_Tx;
extern byte ASespera;
extern byte Step;
extern bool led[NUM_SALIDAS];
extern byte KeyEdge;
extern bool AS1_Tx;
extern int duty_vprop;
extern long cnt_duty_vprop;

extern int bufferNro;
extern TDato *buffersDato;

#if !defined (programador) || defined (ADQ)
word contador_1seg=0;
dword rampaTime=0;
long rampa_mestaTime=0;
bool flag_1seg =0;
word countMesetaTime=0;
extern char flagFinMeseta;
extern char flagComCartel;
dword timeCartel=0;
#endif

#ifdef INTEGRADOR
extern long Acumulador[CANTIDAD_CANALES];
extern word parteAlta;
extern long Acumulador[CANTIDAD_CANALES];
unsigned int contadorBorrado=1;
bool flagBorradoInt =TRUE;
#endif

bool SaveNow;
byte nContadores = CONT_INIT;

T_CONTADOR Contador[CANTIDAD_CONTADORES];



#ifdef jony_15_08
/*																			 
** ===================================================================
**     Method      :  HabilitarAccionPagApagado 
** ===================================================================
*/
void HabilitarAccionPagApagado(void){
  PwsnError=FALSE;  
}
#endif
#ifdef INTEGRADOR
/*
Borrar el sector que comienza en FLASH_APAGADO_START 
Este sector se utiliza para salvar valores en el caso de corte de energia imprevisto
Se debe borrar para poder escribir!!
utilizar dentro de la funcion de interuptcion de 1ms 
*/
void BorradoSector (void){
  if(++contadorBorrado>CUENTA_1SEG && flagBorradoInt){   //se accede una sola ves, despues de que paso 1seg
    flagBorradoInt=FALSE;
    EraseSectorInternal(FLASH_APAGADO_START); // borro la posicion de la flash donde voy a salvar el valor acumulado
  }
  

}
#endif
/*
** ===================================================================
**     Method      :  Timer_Run 
** ===================================================================
*/
byte Timer_Run(word tiempo,bool * flag, byte tipo) {
byte i;
  for(i=0;i<nContadores;i++){
    if (Contador[i].flag==flag){
      Contador[i].tiempo=tiempo;
    if(tipo==UNICO_SET)
      Contador[i].cuenta=tiempo;
    Contador[i].tipo = tipo;
    return ERR_OK;
    }
  }

  if (nContadores == CANTIDAD_CONTADORES) return ERR_RANGE;
  
  Contador[nContadores].tiempo=tiempo;
  if(tipo==UNICO_SET)
      Contador[nContadores].cuenta=tiempo;
  Contador[nContadores].tipo = tipo;
  Contador[nContadores].flag=flag; 
  nContadores++;
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  Timer_Terminate 
** ===================================================================
*/

byte Timer_Terminate(bool * flag) {
byte i;
  for(i=0;i<nContadores;i++){
    if (Contador[i].flag==flag){
     Contador[i].flag=NULL;
     Contador[i].tipo=FIN;
     return ERR_OK;
    }
  }
  return ERR_RANGE;
}					 

/***********************************************************************************************************/

/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     From bean   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the bean is enabled - "Enable" and the events are
**         enabled - "EnableEvent").
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/



void TI1ms_OnInterrupt(void)
{
/* Write your code here ... */ 
extern bool ADfinish;
extern PunteroF PtrTmp;
byte i;
static char contTicks=0;

//Nicolas 
/***************************************************/

#ifdef INTEGRADOR
   BorradoSector (); 
#endif 

#if!defined (programador) || defined (ADQ)

if(contador_1seg >= CUENTA_1SEG){
   contador_1seg = 0;
   flag_1seg = TRUE;

#ifdef VF   
   if(flagComCartel==TRUE)
    timeCartel++;

   if(VFstatus == RUNVF  && !spIsMayorMeseta){
    rampa_mestaTime++;
    
   }
#endif
   
} else
    contador_1seg++;

#endif
/**************************************************/


//Verificacion tiempo de converción del ADC 
   
    if (++CycleCont >= ADTIME){		                         // Llego el Contador del Ciclo al tiempo de conversion???
      #ifndef RPM
        ADC_Start(0);                                        //Empieza a hacer los samples
      #endif
      CycleCont =0;
    }	 
    else if (CycleCont==(ADTIME - TIEMPO_GRABACION))
      if(DSave && save_parametros && (*PtrTmp==MainHandler || *PtrTmp==TitleHandler 
      
#ifdef programador
      || *PtrTmp==ProgramaHandler
#endif
      
      || SaveNow))
        TI1_SetPeriodMode(TI1_Pm_40ms);                    //Next interrupt is 40ms length

  #if defined (_PRINTER)||defined(RPM)
 // IncTimers(1);
 //listeners1ms->executeMethods();
 	 
  #endif
  executeMethodsTimer(ListaInt1ms);
 
#ifdef SIMCIC  
 
 if(PWSN_GetVal() != 0)                                   //Pwsn is high??
		 pwsn1++;
 else
     pwsn1 = 0; 

 if(pwsn1==25){
       Cpu_DisableInt();
//        WriteArray(FLASH_APAGADO_START,0, 1, (word*)&ValFinal[0]);		 
//		    WriteWord((word)&PRom[R_SetPoint],1245);		 
     WriteWord(FLASH_APAGADO_START,getValCalculado(0));
     Cpu_EnableInt();
 }
 
 
 
#else
 
    
  // Aqui veo el Power Sense para salvar datos 
    if (PWSN_GetVal() && !PwsnError)		                   //Pwsn is high??
    {		 
    pwsn++;									                               // inc counter
      if (pwsn==30){                                       // is counter = 30?				
      Cpu_DisableInt();
      #ifdef jony_05_07
      O2_PutVal(0);
			O4_PutVal(0);
			O6_PutVal(0);
			O7_PutVal(0);
      #endif

      Display1_PutVal(0);
      #ifdef jony_28_06
      TSCR1_TEN=0;		
      #endif

			#ifdef programador
          GuardarPrograma();
 			#endif
       
      #ifdef INTEGRADOR
        WriteNWord(2,((word*)Acumulador));
      #endif   
         
      #ifdef jony_06_07
      TSCR1_TEN=1;
      #endif
      PwsnError=TRUE; 
      Cpu_EnableInt();
      }
    }else 
      pwsn=0;	   // put counter=0
      
       
 #endif 

//Aqui actualizo Contadores Estandard de timers 

    for(i=0; i<nContadores; i++){
        if (Contador[i].tipo==FIN) continue;
        
        if (Contador[i].cuenta)
          Contador[i].cuenta--;
        else{
          switch(Contador[i].tipo){
            case CONTINUO_SET:
              Contador[i].cuenta= Contador[i].tiempo;
              *(Contador[i].flag) = 1;
              break; 
            case UNICO_SET:
              *(Contador[i].flag) = 1;
              Contador[i].tipo = FIN;
              break;
            case  CONTINUO_TOG:
              Contador[i].cuenta= Contador[i].tiempo;
              *(Contador[i].flag) = ! *(Contador[i].flag);
            
          }
        }
    }

//Aqui Contadores de la comunicacion
   #ifndef _PRINTER 
    if(ASespera)	                                         // evito dar la vuelta
      ASespera--;		                                       //decremento contador de la comunicacion
	  else if (Step)
	    Step=0;                                              // error (recibimiento parcial de un mensaje)
	  else if (AS1_Tx)
	    ModBus_Send();
	  #endif
//Aqui Contadores del stn

    if (PRom[R_Stn]==Stc)
		  St_Counter[0]++;
    
    if (PRom[R_Stn+1]==Stc)
		  St_Counter[1]++;

//Aqui Contadores de los programas

	 #ifndef jony_25_06
	 #ifdef programador
    for(i=0;i<CANTIDAD_SAL_CONTROL;i++){
      if (PRom[R_Programa+i]!=NO_PROGRAMA){
        TiempoProgramaCh[i]++;        
        if (TiempoProgramaCh[i]>=1000){
          ActualizarSetPointPrograma(i);
          TiempoProgramaCh[i]-=1000;
        } 
      }
    }
   #endif
   #endif
    

//Aqui el display

    if(screen_cont!=MAIN_TEXT_PERIOD)			                 //Contador para mostrar diferentes cosas en pantalla
      screen_cont++;
    else 
      screen_cont=0 ;
  
    DpyAndSwitch();					                               // refresh LCD y scan switch

//Aqui la vprop

#ifdef VPROP

vpout = outvprop(duty_vprop, PRom[R_Topn], PRom[R_Dbnd], 1, &cnt_duty_vprop);

if(vpout == OPEN){
  PWM_OnToggle(TRUE,0);
  PWM_OnToggle(FALSE,1);
}
else if(vpout == CLOSE){
  PWM_OnToggle(FALSE,0);
  PWM_OnToggle(TRUE,1);
}
else{
  PWM_OnToggle(FALSE,1);
  PWM_OnToggle(FALSE,0);
}

#endif



}

/*
** ===================================================================
**     Event       :  TI40ms_OnInterrupt (module Events)
**
**     From bean   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the bean is enabled - "Enable" and the events are
**         enabled - "EnableEvent").
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TI40ms_OnInterrupt(void)
{
  /* Write your code here ... */
  extern bool FstTime;
  extern TPtrBox PtrBox;
  #ifdef VF_PROG
  extern const NumericoRO Principal1;
  #else
  extern const Numerico Principal1;
  #endif

  byte Error,i;
  
  
  													
  TI1_SetPeriodMode(TI1_Pm_1ms); //Next interrupt is 1ms length
  CycleCont +=TIEMPO_GRABACION;
    
    if (PRom[R_Stn]==Stc)
      St_Counter[0]+=TIEMPO_GRABACION;  		
    
    if (PRom[R_Stn+1]==Stc)
      St_Counter[1]+=TIEMPO_GRABACION;  		
   
    
    #ifndef jony_25_06
    #ifdef programador
    for(i=0;i<CANTIDAD_SAL_CONTROL;i++)
      if (PRom[R_Programa+i]!=NO_PROGRAMA) TiempoProgramaCh[i]+=TIEMPO_GRABACION;
    #endif
    #endif
//////////////Contador Estandard ////////////////////
    for(i=0; i<nContadores; i++){
        if (Contador[i].tipo==FIN) continue;
        
        if (Contador[i].cuenta>=TIEMPO_GRABACION)
          Contador[i].cuenta-=TIEMPO_GRABACION;
        else{
          Contador[i].cuenta=0;
          switch(Contador[i].tipo){
            case CONTINUO_SET:
              Contador[i].cuenta= Contador[i].tiempo;
              *(Contador[i].flag) = 1;
              break; 
            case UNICO_SET:
              *(Contador[i].flag) = 1;
              Contador[i].tipo = FIN;
              break;
            case  CONTINUO_TOG:
              Contador[i].cuenta= Contador[i].tiempo;
              *(Contador[i].flag) = ! *(Contador[i].flag);
            
          }
        }
      
    }
//////////////////////////////////////////
    /*  Grabo en la FLASH */
    //Display1_PutVal(0);		 /* Borro el display */
      Error=IFsh10_GrabarFlash();  //GRABACION EN LA FLASH	
      if ((PtrBox.Num==(Numerico*)&Principal1.DirProc))FstTime = TRUE; //Modificar!!!!!
      save_parametros = FALSE;
      SaveNow= FALSE;
    if(Error){
  			SaveNow = DSave = save_parametros =TRUE;
        Escribir(buffersDato,bufferNro);  
  			//led[0]=!led[0]; 
  			}
        
    //Timers  
  #if defined (_PRINTER)||defined(RPM)
  //IncTimers(40);
  //listeners40ms->executeMethods(); 
  #endif
   executeMethodsTimer(ListaInt1ms);

}
