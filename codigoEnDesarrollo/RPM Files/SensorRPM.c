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
//#include "funciones.h"
#include "Control.h"
#include "display.h"
#include "ManejadorMemoria.h"
#include "RlxMTimer.h"
//#include "timer_interrupt.h"
#include "TimerOld.h"
#include "Mydefines.h"
#include "cnfbox.h"

#pragma CONST_SEG DEFAULT


#ifdef RPM

#define RPM_MAX_DECIMALES 2
#define RPM_MAX_UF_COUNT 10
#define MAX_BYTE         255

extern volatile const int PRom[R_ATA+1];

/*  Funciones internas  */
void  SenRpm_DefConstruct(struct SensorRpm * self,va_list * args); 
int SenRpm_getValue(struct SensorRpm * self); 
void SenRpm_AddOnNew(struct SensorRpm * self,void(*method)(void*),void*Obj);
void SenRpm_Print(struct SensorRpm * self,uchar num_display); 
int SenRpm_AdaptVal(struct SensorRpm * self,int Value);	 
int SenRpm_getDecimales(void * self); //const struct SensorRpm
void SenRpm_procesarCaptura(void * _self);
void SenRpm_procesar(void* _self);
int SenRpm_getLimSup(void* self);
int SenRpm_getLimInf(void* self);
uint SenRpm_getTiempoMuestreo(void* _self);
int SenRpm_getDifDecView (void *);
/************************/
 long buffer=0;
const struct SensorDecLimClass _SensorRpm={ 
  
  CLASS_INITIALIZATION(SensorDecLimClass,SensorRpm,Sensor,SenRpm_DefConstruct,Object_dtor,Object_differ,Object_puto),
  SenRpm_getValue,     // get_Val
  SenRpm_Print,
  SenRpm_getDesc,
  SenRpm_getState,     // Estado
  SenRpm_getDecimales,	 // Decimales
  SenRpm_getDifDecView,       //Adapta un valor cualquiera
  SenRpm_getLimSup,		 // Limite Superior en formato Print
  SenRpm_getLimInf,			 // Limite inferior en formato print
  SenRpm_getTiempoMuestreo 
};



extern struct ManejadorMemoria *const pFlash;


int SenRpm_getDifDecView (void * _self){
  return 0;
}

SensorRpm::SensorRpm(){
  
}
            
/*
** ===================================================================
**     Method      :  Sensor_Construct 
**     Description :  Constructor del Sensor
** ===================================================================
*/

void  SenRpm_Construct(struct SensorRpm * self,ThreadAttachable::ThreadAttacher* adj,uint tiempoDeMuestreo,struct Capturador * capturador,const SensorRpmConf * conf,const char * desc){
//  while(!_AD_isnew(adc)) //Espera a q haya una conversión
//    WDog1_Clear();
  
  Sensor_constructor(self);
  _SensorVisual_setDescription(self,desc);
  //tiempoDeMuestreo=5*tiempoDeMuestreo;
  self->conf=conf; 
  self->capturador=capturador;
  self->bufferFiltro=0;
 // self->buffer=0;
  Capturador_Comenzar(capturador);
  newAlloced(&self->timerMuestreo,&RlxMTimer,(ulong)tiempoDeMuestreo,SenRpm_procesarCaptura,self);
  //AdjuntadorAHilo_adjuntar(adj,SenRpm_procesar,self);
  struct Method * methodObj=(struct Method *) _new(&Method,SenRpm_procesar,self);
  (adj)->adjuntar(methodObj);
  self->onNewVal=NULL;
  self->state= SENSOR_OK;
  self->ContadorUf=0;
}



/*
** ===================================================================
**     Method      :  Sensor_DefConstruct 
**     Description :  Constructor por defecto del Sensor
** ===================================================================
*/

void  SenRpm_DefConstruct(struct SensorRpm * self,va_list * args){
  SenRpm_Construct(self,va_arg(*args,ThreadAttachable::ThreadAttacher*),va_arg(*args,uint),va_arg(*args,struct Capturador *),va_arg(*args,const SensorRpmConf *),va_arg(*args,char*));  
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
  
  Capturador_Procesar(_s->capturador);
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
 
 // if(_s->procesar==TRUE){
    int filtro = _s->conf->iFiltro;
    ulong valProv=0;
    float val_f=0;
    ulong microseg=0;
    uint pulsos=0;
    uint mult=0;
    uchar err;
 
   if(_s->procesar==TRUE){
    _s->procesar=FALSE;  
    
    microseg=Capturador_getMicroSegundos(_s->capturador);
    pulsos=Capturador_getPulsos(_s->capturador);
    
    
    mult= 600 * pow10(_s->conf->iDecimales);   //600 6000 o 60000
    if(microseg!=0){
      /*
      if(_s->conf->pulsosPorVuelta<3000){
        
        if(microseg<1100000)
          valProv=(ulong)pulsos*mult*100000/(_s->conf->pulsosPorVuelta*microseg);
        else
          goto el;
      }else{
        
      el:  
        valProv=(ulong)pulsos*mult/(_s->conf->pulsosPorVuelta*microseg/100000);
                                                                    /*centirevoluciones por minuto
     																																  no puede haber sobre rango con pulsosPorVuelta*microseg <4*10^9
     																																  en general sera 100 * 10^6 = 10^8.
     																																  Lo mismo para pulsos*6*10^7<4*10^9 ya que pulsos<65536
     																																/
      */
      valProv=(ulong)pulsos*mult;
      val_f=valProv/(((float)_s->conf->pulsosPorVuelta)*microseg/100000);
      valProv = val_f;
     
      if(valProv<65535){     
        valProv= filtrarL (valProv,filtro,10,200,&buffer);
        Cpu_DisableInt();
        
        /*
        if(filtro!=0)
          _s->procVal=(long)(valProv/filtro);
        else
          _s->procVal=(long)valProv;
          */
        
         _s->procVal=(long)valProv; 
         
         buffer=valProv;
        _s->bufferFiltro=valProv;
        _s->state=SENSOR_OK;
        _s->ContadorUf=0;
        Cpu_EnableInt();  
      
      } else{
        Cpu_DisableInt();
        _s->state=SENSOR_OF;
        Cpu_EnableInt();   
      }
    }else {
      if(_s->ContadorUf<RPM_MAX_UF_COUNT){
        _s->ContadorUf++;
        _s->state=SENSOR_OK;
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
    
    
    
    
    //Eventos
    Sensor_notificarListeners(_self);
    //Fin Eventos

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
TSensorState SenRpm_getState(const struct SensorRpm * self){
  return self->state;
}
/*
** ===================================================================
**     Method      :  SenRPM_getDesc 
**     Returns		 :  Obtiene la descripcion del sensor
** ===================================================================
*/
char *  SenRpm_getDesc(void * _self){  
  return _SensorVisual_getDescription(_self);
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
    case SENSOR_OK:
        if(Val>MAX_NUM_DISPLAY)
          PasarASCII(" OF ",num_display);
        else
          Pasar_Numero(Val,num_display,decimales);//(uchar)_getDec(self));
    break;

    case SENSOR_OF:
        PasarASCII(" OF ",num_display);
    break;

 /*   case SENSOR_UF:
        PasarASCII(" UF ",num_display);
 */  
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
  //byte err=_MANEJADOR_MEMORIA_SET_BYTE(pFlash,&self->conf->iDecimales,val);
  self->conf->iDecimales=val;
  self->bufferFiltro=0;														

  return 0;
//  Sensores_OnDecimalesChange(num_canal);
  
 // return err;    
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
  //return _MANEJADOR_MEMORIA_GET_BYTE(pFlash,&self->conf->iDecimales);
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
  //return _MANEJADOR_MEMORIA_SET_BYTE(pFlash,&(self->conf->iFiltro),val);
}

/*
** ===================================================================
**    Method      :   get_LimInf_filtro 
**    Type        :   Static
**    Description :   Lim inf del valor del filtro
** ===================================================================
*/
int SenRpm_getLimInfFiltro(void*){
  return 0;
}

/*
** ===================================================================
**    Method      :   get_LimSup_filtro 
**    Type        :   Static
**    Description :   Lim sup del valor del filtro
** ===================================================================
*/
int SenRpm_getLimSupFiltro(void*){
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
  //return _MANEJADOR_MEMORIA_SET_WORD(pFlash,(uint*)&self->conf->pulsosPorVuelta,val);
}

/*
** ===================================================================
**    Method      :   SenRpm_getLimInfPulsosPorVuelta 
**    Type        :   Static
**    Description :   Lim inf del valor
** ===================================================================
*/
int SenRpm_getLimInfPulsosPorVuelta(void*){
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

#endif
/* END SensorRPM. */
