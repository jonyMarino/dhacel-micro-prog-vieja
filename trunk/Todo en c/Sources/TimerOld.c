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
#include "StaticTimer.h"
#include "SelfTune.h"
#include "PID.h"
#include "Alarmas.h"
#include "TeclasYDisplay.h"

const int ADTIME = AD_TIME_EN_MILI_SEGUNDOS;

unsigned int CycleCont=0;
byte pwsn=0;
bool PwsnError=TRUE;	 //al iniciar no se permite grabar en la FLASH
bool DSave=TRUE;
bool SaveNow;


/*																			 
** ===================================================================
**     Method      :  HabilitarAccionPagApagado 
** ===================================================================
*/
void HabilitarAccionPagApagado(void){
  PwsnError=FALSE;  
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
  byte i;

//Verificacion tiempo de converción del ADC 
   
    if (++CycleCont >= ADTIME){		                         // Llego el Contador del Ciclo al tiempo de conversion???
      #ifndef RPM
        ADC_Start(0);                                        //Empieza a hacer los samples
      #endif
      CycleCont =0;
    }	 
    else if (CycleCont==(ADTIME - TIEMPO_GRABACION)){
      
    #ifdef programador  
      if(DSave && save_parametros && (*PtrTmp==MainHandler || *PtrTmp==TitleHandler || *PtrTmp==ProgramaHandler || SaveNow))
        TI1_SetPeriodMode(TI1_Pm_40ms);  
    #else 
      if(DSave && save_parametros && (*PtrTmp==MainHandler || *PtrTmp==TitleHandler || SaveNow))
        TI1_SetPeriodMode(TI1_Pm_40ms);                    //Next interrupt is 40ms length
    #endif
    }
    
    executeMethods(ListaInt1ms);
 
    #ifdef SIMCIC  
 
      if(PWSN_GetVal() != 0)                                   //Pwsn is high??
		    pwsn++;
      else
        pwsn = 0; 

      if(pwsn==25){
        Cpu_DisableInt();
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
      
        O2_PutVal(0);
  			O4_PutVal(0);
  			O6_PutVal(0);
  			O7_PutVal(0);
     
        Display1_PutVal(0);
        TSCR1_TEN=0;		
      

			  #ifdef programador
          GuardarPrograma();
 			  #endif
       
        #ifdef INTEGRADOR
          WriteNWord(2,((word*)Acumulador));
        #endif   
         
        TSCR1_TEN=1;
        PwsnError=TRUE; 
        Cpu_EnableInt();
      }
    
    }else 
      pwsn=0;	   // put counter=0
      
       
 #endif 

  //Aqui actualizo Contadores Estandard de timers 
   HandlerStaticTimer(Base1ms);
   
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
	 #ifdef programador
   ActualizarContadoresProg();
   #endif
   
   //Aqui el display
    if(screen_cont!=MAIN_TEXT_PERIOD)			                 //Contador para mostrar diferentes cosas en pantalla
      screen_cont++;
    else 
      screen_cont=0 ;
  
    //DpyAndSwitch();					                               // refresh LCD y scan switch
  
    handlerTeclasYDisplay(&displays);
    
    //Aqui la vprop
    #ifdef VPROP
      setPWMOut();
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
  
  byte Error,i;
  													
  TI1_SetPeriodMode(TI1_Pm_1ms); //Next interrupt is 1ms length
  CycleCont +=TIEMPO_GRABACION;
    
    if (PRom[R_Stn]==Stc)
      St_Counter[0]+=TIEMPO_GRABACION;  		
    
    if (PRom[R_Stn+1]==Stc)
      St_Counter[1]+=TIEMPO_GRABACION;  		
   
     //Aqui actualizo Contadores Estandard de timers   
      HandlerStaticTimer(Base40ms);
            
      executeMethods(ListaInt40ms);
    
      /* Grabo en la FLASH */
      Error=IFsh10_GrabarFlash();  //GRABACION EN LA FLASH	
      
      if ((PtrBox.Num==(Numerico*)&Principal1.DirProc))
        FstTime = TRUE; 
 
      save_parametros = FALSE;
      SaveNow= FALSE;
    
       if(Error){
  			SaveNow = DSave = save_parametros =TRUE;
        Escribir(buffersDato,bufferNro);  
  			
  			}
        
   

}
