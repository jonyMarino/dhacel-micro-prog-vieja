/*IMPLEMENTATION MODULE SensorRPM; */
/*
**     Filename  : SensorRPM.C
**     Project   : Controlador
**     Processor : MC9S12GC32CFU
**     Version   : 0.0.1
**     Compiler  : Metrowerks HC12 C Compiler
**     Date/Time : 31/03/2008, 14:19
**     Abstract  :
**							Sensor de rpms
*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "limits.h"
#include "IFsh10.h"
#include "PTSL.h"
#include "Math.h"
#include "Errores.h"
#include "display.h"
#include "WDog1.h"
#include "SensorRPM.h"
#include "Control.h"
#include "display.h"
#include "RlxMTimer.h"
#include "TimerOld.h"
#include "Mydefines.h"
#include "cnfbox.h" 
#include "CapturadorPT2.h"
#include "Parametros.h"

#pragma CONST_SEG DEFAULT


#ifdef RPM

#define RPM_MAX_DECIMALES 2
#define RPM_MAX_UF_COUNT 10
#define MAX_BYTE         255

long buffer=0;


void SenRpm_procesarCaptura(void * _self);
void SenRpm_procesar(void* _self);
         
/*
** ===================================================================
**     Method      :  Sensor_Construct 
**     Description :  Constructor del Sensor
** ===================================================================
*/

void  SenRpm_Init(struct SensorRpm * self,uint tiempoDeMuestreo,struct CapturadorPT2 * capturador,const SensorRpmConf * conf){
  
  self->conf=conf; 
  
  self->capturador=capturador;
  
  self->bufferFiltro=0;
 
  CapturadorPT2_Comenzar();
  
  self->timerMuestreo=RlxMTimer_Init(tiempoDeMuestreo,SenRpm_procesarCaptura,self);
 
  self->onNewVal=initMethod(SenRpm_procesar,self); 

  MethodContainer_add(self->onNewVal,ListaPrincipal);
  
  self->state= RPM_OK;
  
  self->ContadorUf=0;
}


/*
** ===================================================================
**     Method      :  SensorHandler 
**     Description :  Checkea si hay un nuevo valor del AD, 
**                    y si lo hay lo procesa.
** ===================================================================
*/

void SenRpm_procesarCaptura(void * _self){
  struct SensorRpm *_s=(struct SensorRpm *)_self;
  
  CapturadorPT2_Procesar(_s->capturador);
  _s->procesar=TRUE;
}

/*
** ===================================================================
**     Method      :  SensorHandler 
**     Description :  Checkea si hay un nuevo valor del AD, 
**                    y si lo hay lo procesa.
** ===================================================================
*/
void SenRpm_procesar(void* _self){
  struct SensorRpm *_s=(struct SensorRpm *)_self;
 
    int filtro = _s->conf->iFiltro;
    ulong valProv=0;
    float val_f=0;
    ulong microseg=0;
    uint pulsos=0;
    uint mult=0;
    uchar err;
 
   if(_s->procesar==TRUE){
    _s->procesar=FALSE;  
    
    microseg=CapturadorPT2_getMicroSegundos(_s->capturador);  //(_s->capturador->ultimoproceso.microsegundos);//
    pulsos=CapturadorPT2_getPulsos(_s->capturador); //_s->capturador->ultimoproceso.pulsos;//
   
    
    mult= 600 * pow10(_s->conf->iDecimales);   //600 6000 o 60000
    if(microseg!=0){
      
      valProv=(ulong)pulsos*mult;
      val_f=valProv/(((float)_s->conf->pulsosPorVuelta)*microseg/100000);
      val_f=(val_f*SenRpm_getAjusteGanancia(_s))/(float)1000;
      valProv = val_f;
     
      if(valProv<65535){     
        valProv= filtrarL (valProv,filtro,10,200,&buffer);
        Cpu_DisableInt();
        
         _s->procVal=(long)valProv; 
         
         buffer=valProv;
        _s->bufferFiltro=valProv;
        _s->state=RPM_OK;
        _s->ContadorUf=0;
        Cpu_EnableInt();  
      
      } else{
        Cpu_DisableInt();
        _s->state=RPM_OF;
        Cpu_EnableInt();   
      }
    }else {
      if(_s->ContadorUf<RPM_MAX_UF_COUNT){
        _s->ContadorUf++;
        _s->state=RPM_OK;
        valProv=(ulong)100*mult/(_s->conf->pulsosPorVuelta*_s->ContadorUf*Timer_getTime(&_s->timerMuestreo)); //calculo cual seria el valor de rpm si hubiese entrado un solo pulso
        
        //Nuevo
        valProv= filtrarL (valProv,filtro,10,200,& _s->bufferFiltro);
        //Fin Nuevo
        if(_s->procVal>valProv)
          _s->procVal=valProv;   //redusco el valor de rpms
      }else{ 
        _s->procVal=filtrarL (0,filtro,10,200,& _s->bufferFiltro);
      }
    }
    

 }
}


/*
** ===================================================================
**     Method      :  SenRpm_getValue 
**     Description :  Devuelve el último valor procesado del AD
** ===================================================================
*/
int SenRpm_getValue(struct SensorRpm * self){
  int val;
  
  Cpu_DisableInt();
  val= self->procVal;
  Cpu_EnableInt();  
  return val;
}

/*
** ===================================================================
**     Method      :  SenRpm_getState 
**     Description :  Indica el estado del sensor(UF,OF,OK)
** ===================================================================
*/
TrPMState SenRpm_getState(const struct SensorRpm * self){
  return self->state;
}

/*
** ===================================================================
**     Method      :  SenRpm_Print 
**     Description :  Imprime el valor del sensor
** ===================================================================
*/
void SenRpm_Print(struct SensorRpm * self,uchar num_display){
  byte decimales=self->conf->iDecimales;
  uint Val=self->procVal;  
											 
    
  switch (self->state){
    case RPM_OK:
        if(Val>MAX_NUM_DISPLAY)
          PasarASCII("OPEn",num_display);
        else
          Pasar_Numero(Val,num_display,decimales);
    break;

    case RPM_OF:
        PasarASCII("OPEn",num_display);
    break;

 
  }

}




/*
** ===================================================================
**     Method      :  SenRpm_AdaptVal 
**     Description :  Adapta el valor que viene con los mismos 
**      decimales con el que se muestra el sensor y lo devuelve con
**      los decimales que se obtienen al hacer un getVal del sensor. 
** ===================================================================
*/
int SenRpm_AdaptVal(struct SensorRpm * self,int Value){
byte decimales=self->conf->iDecimales;

  return Value;// Value * pow10(RPM_MAX_DECIMALES-decimales); 
}

/// FUNCIONES DE SETEO Y OBTENCION DE VARIABLES 
/*  Decimales */
/*
** ===================================================================
**     Method      :  SENSOR_setDecimales 
**    Description : Setea la cantidad de decimales
** ===================================================================
*/
byte SenRpm_setDecimales(struct SensorRpm * self, int val){
  
  self->conf->iDecimales=val;
  self->bufferFiltro=0;														

  return 0;

}

/*
** ===================================================================
**     Method      :  SENSOR_getDecimales 
**    Description :   Obtiene la cantidad de decimales
** ===================================================================
*/
int SenRpm_getDecimales(void * _self){
  const struct SensorRpm * self=(const struct SensorRpm *)_self;
  return self->conf->iDecimales;
 
}

/*
** ==========================================================================
**     Method      :  get_LimInf_Decimales 
**    Type:   Static
**    Description :   Obtiene el minimo valor de decimales que puede setearse
** ==========================================================================
*/
int SenRpm_getLimInfDecimales(void){
  return 0;
}

/*
** ==========================================================================
**     Method      :  get_LimSup_Decimales 
**    Description :   Obtiene el maximo valor de decimales que puede setearse
** ==========================================================================
*/
int SenRpm_getLimSupDecimales(const struct SensorRpm * self){
  return RPM_MAX_DECIMALES;
}



/*  Filtro  */
/*
** ===================================================================
**     Method      :  get_filtro 
**    Description :   Obtiene el valor del filtro
** ===================================================================
*/
int SenRpm_getFiltro(const struct SensorRpm * self){
  return self->conf->iFiltro;
}

/*
** ===================================================================
**     Method      :  set_filtro 
**    Description :   Setea el valor del filtro
** ===================================================================
*/
byte SenRpm_setFiltro(struct SensorRpm * self,int val){
  self->conf->iFiltro=val;
  return 0;
  
}

/*
** ===================================================================
**    Method      :   get_LimInf_filtro 
**    Type        :   Static
**    Description :   Lim inf del valor del filtro
** ===================================================================
*/
int SenRpm_getLimInfFiltro(void){
  return 0;
}

/*
** ===================================================================
**    Method      :   get_LimSup_filtro 
**    Type        :   Static
**    Description :   Lim sup del valor del filtro
** ===================================================================
*/
int SenRpm_getLimSupFiltro(void){
  return MAX_BYTE;
}

/*  Pulsos por vuelta  */
/*
** ===================================================================
**     Method      :  SenRpm_getPulsosPorVuelta 
**    Description :   Obtiene el valor 
** ===================================================================
*/
int SenRpm_getPulsosPorVuelta(const struct SensorRpm * self){
  return self->conf->pulsosPorVuelta;
}

/*
** ===================================================================
**     Method      :  SenRpm_setPulsosPorVuelta 
**    Description :   Setea el valor 
** ===================================================================
*/
byte SenRpm_setPulsosPorVuelta(struct SensorRpm * self,int val){
    self->conf->pulsosPorVuelta=val;
    return 0;
 
}

/*
** ===================================================================
**    Method      :   SenRpm_getLimInfPulsosPorVuelta 
**    Type        :   Static
**    Description :   Lim inf del valor
** ===================================================================
*/
int SenRpm_getLimInfPulsosPorVuelta(void){
  return 1;
}

/*
** ===================================================================
**    Method      :   SenRpm_getLimSup 
**    Description :   Lim Superior del valor que puede mostrar el sensor
**    en el formato en que se imprime el sensor
** ===================================================================
*/

int SenRpm_getLimSup(void* _self){
  struct SensorRpm *_s=(struct SensorRpm *)_self;
  return 9999;
}
/*
** ===================================================================
**    Method      :   SenRpm_getLimSup 
**    Description :   Lim Inferior del valor que puede mostrar el sensor
**    en el formato en que se imprime el sensor
** ===================================================================
*/
int SenRpm_getLimInf(void* _self){
  struct SensorRpm *_s=(struct SensorRpm *)_self;
  return 0;
}

/*
** ===================================================================
**    Method      :   SenRpm_getTiempoMedicion 
**    Description :   Obtiene el periodo en que se toman las muestras
** ===================================================================
*/
uint SenRpm_getTiempoMuestreo(void* _self){
  struct SensorRpm *_s=(struct SensorRpm *)_self;
  return Timer_getTime(&_s->timerMuestreo);
}


/*
** ===================================================================
**     Method      :  SenRpm_getAjusteGanancia
**    Description :   Obtiene el valor 
** ===================================================================
*/
word SenRpm_getAjusteGanancia(const struct SensorRpm * self){
  return self->conf->AjusteGan;
}

/*
** ===================================================================
**     Method      :  SenRpm_setAjusteGanancia 
**    Description :   Setea el valor 
** ===================================================================
*/
byte SenRpm_setAjusteGanancia(struct SensorRpm * self,int val){
    self->conf->AjusteGan=val;
    return 0;
 
}


#endif
/* END SensorRPM. */
