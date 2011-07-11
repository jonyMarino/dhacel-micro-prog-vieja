/*IMPLEMENTATION MODULE Sensores; */
/*
**     Filename  : SensorTPT_Class.C
**     Project   : Controlador
**     Processor : MC9S12GC32CFU
**     Version   : 0.0.2
**     Compiler  : Metrowerks HC12 C Compiler
**     Date/Time : 30/01/2008, 14:42
**     Abstract  :
**							Sensor de termocupla y PT100
*/
#include "Mydefines.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "limits.h"
#include "ADC.h"
#include "IFsh10.h"
#include "PTSL.h"
#include "Events.h"
#include "Math.h"
#include "Errores.h"
#include "Sensor.h"
#include "Sensores.h"
#include "ClassLongSensor.h"
#include "display.h"
#include "WDog1.h"
#include "SensorTPT_Class.h"
#include "funciones.h"
#include "ManejadorMemoria.h"
#include "Configuracion.h"
#include "OutputStream.h"
#include "str_lib.h"

#pragma CONST_SEG DEFAULT

/*  Funciones internas  */
void  SensorAdaptador_DefConstruct(struct TSensor_TermoPT * self,va_list * args);
TError Sensor_ChangeTipo(const struct TSensor_TermoPT * sensor,int val);
void Sensor_Procesar(struct TSensor_TermoPT * self);  
int SensorAdaptador_getValue(struct TSensor_TermoPT * self); 
void SensorAdaptador_AddOnNew(struct TSensor_TermoPT * self,void(*method)(void*),void*Obj);
void SensorAdaptador_Print(struct TSensor_TermoPT * self,uchar num_display); 
int SensorAdaptador_getDifDecView(struct TSensor_TermoPT * self);
uint SensorAdaptador_getTiempoMuestreo(struct TSensor_TermoPT * self);
/************************/

const struct SensorDecLimClass SensorAdaptador={ 
  &SensorDecLimClass,
  "SensorAdaptador",
  &Sensor,
  sizeof(struct SensorAdaptador),		 //size
  SensorAdaptador_DefConstruct, // constructor
  NULL,                // destructor
  NULL, // differ
  NULL, // puto
  SensorAdaptador_getValue,     // get_Val
  SensorAdaptador_Print,
  SensorAdaptador_getDesc,
//  SensorAdaptador_AddOnNew,        // is_new? 
  SensorAdaptador_getState,     // Estado
  SensorAdaptador_getDecimalesMostrados,	 // Decimales
  SensorAdaptador_getDifDecView,       //Adapta un valor cualquiera
  get_LimSup_Sensor,		 // Limite Superior en formato Print
  get_LimInf_Sensor,			 // Limite inferior en formato print
  SensorAdaptador_getTiempoMuestreo
};

const void * const TSensor_TermoPT = &TableSensorAdaptador;

#pragma CONST_SEG PARAMETERS_PAGE
volatile const int iCompensacionTempAmb; 
#pragma CONST_SEG DEFAULT
            
/*
** ===================================================================
**     Method      :  Sensor_Construct 
**     Description :  Constructor del Sensor
** ===================================================================
*/

void  SensorAdaptador_Construct(struct TSensor_TermoPT * self,struct TAdc* adc,const SensorConf * conf,const char * desc){
  Sensor_constructor(self);
  while(!_AD_isnew(adc)) //Espera a q haya una conversión
    WDog1_Clear();
  _SensorVisual_setDescription(self,desc);
  self->Adc=adc;
  self->conf=conf; 
  set_Sensor(self,get_Sensor(self));  // inicializo Tipo con el valor guardado   
  SensorAdaptador_Handler(self);  // Como hay un valor nuevo lo mando a procesar
}

/*
** ===================================================================
**     Method      :  Sensor_DefConstruct 
**     Description :  Constructor por defecto del Sensor
** ===================================================================
*/

void  SensorAdaptador_DefConstruct(struct TSensor_TermoPT * self,va_list * args){
  SensorAdaptador_Construct(self,va_arg(*args,void*),va_arg(*args,void*),va_arg(*args,char*));  
}

/*
** ===================================================================
**     Method      :  SensorHandler 
**     Description :  Checkea si hay un nuevo valor del AD, 
**                    y si lo hay lo procesa.
** ===================================================================
*/
void SensorAdaptador_Handler(struct TSensor_TermoPT * self){
  if(_AD_isnew(self->Adc))
    Sensor_Procesar(self);
}
/*
** ===================================================================
**     Method      :  SensorAdaptador_getValue 
**     Description :  Devuelve el último valor procesado del AD
** ===================================================================
*/
int SensorAdaptador_getValue(struct TSensor_TermoPT * self){
  int val;
  
  Cpu_DisableInt();
  val= self->ProcVal;
  Cpu_EnableInt();  
  return val;
}

/*
** ===================================================================
**     Method      :  SensorAdaptador_getState 
**     Description :  Indica el estado del sensor(UF,OF,OK)
** ===================================================================
*/
TSensorState SensorAdaptador_getState(const struct TSensor_TermoPT * self){
  return self->state;
}
/*
** ===================================================================
**     Method      :  SensorAdaptador_getDesc 
**     Returns		 :  Obtiene la descripcion del sensor
** ===================================================================
*/
char *  SensorAdaptador_getDesc(void * _self){  
  return _SensorVisual_getDescription(_self);
}
/*
** ===================================================================
**     Method      :  Sensor_Procesar 
**     Description :  Procesa el nuevo valor del AD
**                    y obtiene un nuevo valor del sensor
** ===================================================================
*/
void Sensor_Procesar(struct TSensor_TermoPT * self){
  long ValProv;
  int ValLin;
  TSensorState err; 
  long vx= _Getter_getVal(self->Adc);
  t_sensor sensor=self->conf->eSensor; 
  int ceroTP=self->conf->iCeroTermopar;					 //Cero Termopar
  int ganTP = self->conf->iGananciaTermopar;
  int ceroPT = self->conf->iCeroPT100;
  int ganPT=self->conf->iGananciaPT100;
  int offset= self->conf->iOffset;
  int ganancia = self->conf->iGanancia;
  int filtro = self->conf->iFiltro;
  int CompTempAmb=0;
  
  if (sensor<SENSOR_PT){  
    int Ta_Temp= ADC_getTA()  + iCompensacionTempAmb*10/pow10(SENSOR_Decimales(sensor));  //El menos inicial es porque el diodo esta conectado alreves 
    Sens_getColdComp(Ta_Temp,&CompTempAmb,sensor);
  }
  vx=(vx+ceroTP)*(1000+ganTP)/1000;
  vx = vx*MAXMV/MAXAD;//Convierte de lineal a microvolts para los sensores correspondientes 
  err = Linealizar(vx+CompTempAmb,sensor,&ValLin);
  ValProv=ValLin;  
	if(sensor==SENSOR_PT)
    ValProv = ((ValProv+ceroPT)*(1000+ganPT)/1000);  
	
//  if (!SENSOR_is_Lineal(sensor))
//      ValProv/= pow10(SENSOR_Decimales(sensor)-decimales);  
	ValProv=(ValProv+offset)*ganancia/1000;         //preciso para pasar a long
  ValProv= filtrar ((int)ValProv,filtro,10,200,& self->buffer_fil);
  Cpu_DisableInt();
  self->ProcVal=ValProv;
  self->newVal=TRUE;
  self->state=err;
  Cpu_EnableInt();
  
  //Eventos
  Sensor_notificarListeners(self);
  //Fin Eventos
  
  return;
}

/*
** ===================================================================
**     Method      :  SensorAdaptador_Print 
**     Description :  Imprime el valor del sensor
** ===================================================================
*/
void SensorAdaptador_Print(struct TSensor_TermoPT * self,uchar num_display){
byte decimales=self->conf->iDecimales;
int Val=self->ProcVal;  
t_sensor sensor=self->conf->eSensor; 

  if (!SENSOR_is_Lineal(sensor))												 
      Val/= pow10(SENSOR_Decimales(sensor)-decimales);  			//Ajusto el valor a mostrar por la cantidad de decimales del sensor
  
switch (self->state){
  case SENSOR_OK:
      Pasar_Numero(Val,num_display,decimales);//(uchar)_getDec(self));
  break;

  case SENSOR_OF:
      PasarASCII(" OF ",num_display);
  break;

  case SENSOR_UF:
      PasarASCII(" UF ",num_display);
  break;
}

}

/*
** ===================================================================
**     Method      :  SensorSimulado_Print 
**     Description :  Imprime el valor del sensor
** ===================================================================
*/
void SensorAdaptador_printVal(struct TSensor_TermoPT * self,struct OutputStream* os){
int Val=self->ProcVal;  
											 
  
    
  switch (self->state){
    case SENSOR_OK:
        if(Val>MAX_NUM_DISPLAY)
          write(os," OF ");
        else{
          char str[7];
          FloatToStr(Val,str,6,self->conf->iDecimales);
          write(os,str);
        }
    break;

    case SENSOR_OF:
        write(os," OF ");
    break;

    case SENSOR_UF:
        write(os," UF ");
    break;
  }
    
}

/*
** ===================================================================
**     Method      :  SensorAdaptador_getDifDecView 
**     Description :  Obtiene la diferencia entre los decimales mostrados 
**      y los medidos realmente.
** ===================================================================
*/
int SensorAdaptador_getDifDecView(struct TSensor_TermoPT * self){
t_sensor sensor=self->conf->eSensor;
byte decimales=self->conf->iDecimales;

  return SENSOR_Decimales(sensor)-decimales; 
}






/// FUNCIONES DE SETEO Y OBTENCION DE VARIABLES 
/*
** ===================================================================
**     Method      :  get_LimInf_Sensor 
**     Description :  Limite inferior de temperatura del sensor
** ===================================================================
*/

int get_LimInf_Sensor(struct TSensor_TermoPT * self){
t_sensor sensor=self->conf->eSensor;
int min_sens=SENSOR_Minimo(sensor);      
byte decimal_chan = SENSOR_Decimales(sensor);      
byte decimales=self->conf->iDecimales;

  if(SENSOR_is_Lineal(sensor))
    return min_sens;
  else
    return min_sens/pow10(decimal_chan-decimales);
}

/*
** ===================================================================
**     Method      :  get_LimSup_Sensor 
**     Description :  Limite superior de temperatura del sensor
** ===================================================================
*/

int get_LimSup_Sensor(struct TSensor_TermoPT * self){
t_sensor sensor=self->conf->eSensor;
int max_sens=SENSOR_Maximo(sensor);
byte decimal_chan = SENSOR_Decimales(sensor);      
byte decimales=self->conf->iDecimales;

  if(SENSOR_is_Lineal(sensor))
    return max_sens; 
  else
    return max_sens/pow10(decimal_chan-decimales); 
}



/*  Sensor  */
/*
** ===================================================================
**     Method      :  get_Sensor 
**     Description :  Obtiene el sensor actual(J,K,etc)
** ===================================================================
*/
int get_Sensor(const struct TSensor_TermoPT * self){
  return self->conf->eSensor;
}

/*
** ===================================================================
**     Method      :  set_Sensor 
**     Description :  Setea el sensor (J,K,etc)
** ===================================================================
*/
TError set_Sensor(struct TSensor_TermoPT * self,int sensor){
TError err;
  
  err=_MANEJADOR_MEMORIA_SET_BYTE(pFlash,&(self->conf->eSensor) , (uchar)sensor );
  if(!err){
    err= Sensor_ChangeTipo(self,sensor);
  }
  return err; 
}

/*
** ===================================================================
**     Function      :  get_LimInf_Sensores 
**     Type : Static 
**     Description :  Obtiene el minimo valor correspondiente a un int
**                    en el que se corresponde a un sensor. 
** ===================================================================
*/																				 
int get_LimInf_Sensores(void){
  return 0;
}

/*
** ===================================================================
**     Function      :  get_LimInf_Sensores
**     Type : Static  
**     Description :  Obtiene el minimo valor en integer
**                    en el que se corresponde a un sensor.
** ===================================================================
*/	
int get_LimSup_Sensores(void){
  return NUM_SENSORES-1;  
}


/*
** ===================================================================
**     Method      :  Sensor_ChangeTipo 
**    Description : Cambia el tipo de sensor (K,J,etc.)
** ===================================================================
*/
TError Sensor_ChangeTipo(struct TSensor_TermoPT * self,int val){
byte Lim_Dec;

    if (val==SENSOR_PT) 
      PTSL_PutBit(self->Adc->pin ,TRUE);
    else 
      PTSL_PutBit(self->Adc->pin,FALSE);  		    

    Lim_Dec = SENSOR_Decimales(val);
    
    self->buffer_fil=0; // vuelvo el filtro a 0
    
    #if !defined(_APARATO_VIEJO)
      if ( Lim_Dec < self->conf->iDecimales )
        set_Decimales(Lim_Dec,sensor);
    #else
      SensorAdaptador_setDecimales(self,SENSOR_Decimales_Mostrar(val));
    #endif
    
    return ERR_OK;
      
 //   Sensores_OnSensorChange(num_canal);  
}
/*  Decimales */
/*
** ===================================================================
**     Method      :  SENSOR_setDecimales 
**    Description : Setea la cantidad de decimales
** ===================================================================
*/
byte SensorAdaptador_setDecimales(struct TSensor_TermoPT * self, int val){
  byte err=_MANEJADOR_MEMORIA_SET_BYTE(pFlash,&(self->conf->iDecimales),val);
  														
//  Sensores_OnDecimalesChange(num_canal);
  
  return err;    
}

/*
** ===================================================================
**     Method      :  SENSOR_getDecimales 
**    Description :   Obtiene la cantidad de decimales
** ===================================================================
*/
int SensorAdaptador_getDecimalesMostrados(const struct TSensor_TermoPT * self){
  return _MANEJADOR_MEMORIA_GET_BYTE(pFlash,&self->conf->iDecimales);
}

/*
** ===================================================================
**     Method      :  SENSOR_getDecimales 
**    Description :   Obtiene la cantidad de decimales
** ===================================================================
*/
int SensorAdaptador_getDecimales(const struct TSensor_TermoPT * self){
  t_sensor sensor=self->conf->eSensor; 
											 
  return SENSOR_Decimales(sensor);  			//Ajusto el valor a mostrar por la cantidad de decimales del sensor
}

/*
** ==========================================================================
**     Method      :  get_LimInf_Decimales 
**    Type:   Static
**    Description :   Obtiene el minimo valor de decimales que puede setearse
** ==========================================================================
*/
int get_LimInf_Decimales(void){
  return 0;
}

/*
** ==========================================================================
**     Method      :  get_LimSup_Decimales 
**    Description :   Obtiene el maximo valor de decimales que puede setearse
** ==========================================================================
*/
int get_LimSup_Decimales(const struct TSensor_TermoPT * self){
  return SENSOR_Decimales(self->conf->eSensor);
}

/*  Filtro  */
/*
** ===================================================================
**     Method      :  get_filtro 
**    Description :   Obtiene el valor del filtro
** ===================================================================
*/
int get_filtro(const struct TSensor_TermoPT * self){
  return self->conf->iFiltro;
}

/*
** ===================================================================
**     Method      :  set_filtro 
**    Description :   Setea el valor del filtro
** ===================================================================
*/
byte set_filtro(struct TSensor_TermoPT * self,int val){
  return _MANEJADOR_MEMORIA_SET_BYTE(pFlash,&(self->conf->iFiltro),val);
}

/*
** ===================================================================
**    Method      :   get_LimInf_filtro 
**    Type        :   Static
**    Description :   Lim inf del valor del filtro
** ===================================================================
*/
int get_LimInf_filtro(void){
  return 0;
}

/*
** ===================================================================
**    Method      :   get_LimSup_filtro 
**    Type        :   Static
**    Description :   Lim sup del valor del filtro
** ===================================================================
*/
int get_LimSup_filtro(void){
  return MAX_BYTE;
}

/*  Offset  */
/*
** ===================================================================
**     Method      :  get_offset 
**    Description :   Obtiene el valor del Offset
** ===================================================================
*/
int get_offset(const struct TSensor_TermoPT * self){
  return self->conf->iOffset;
}

/*
** ===================================================================
**     Method      :  set_offset 
**    Description :   Setea el valor del Offset
** ===================================================================
*/
byte set_offset(struct TSensor_TermoPT * self,int val){
  return _MANEJADOR_MEMORIA_SET_WORD(pFlash,&(self->conf->iOffset),val);
}

/*  Ganancia  */
/*
** ===================================================================
**     Method      :  get_offset 
**    Description :   Obtiene el valor de la Ganancia
** ===================================================================
*/
int get_ganancia(const struct TSensor_TermoPT * self){
  return self->conf->iGanancia;
}

/*
** ===================================================================
**     Method      :  set_offset 
**    Description :   Setea el valor de la Ganancia
** ===================================================================
*/
byte set_ganancia(struct TSensor_TermoPT * self,int val){
  return _MANEJADOR_MEMORIA_SET_WORD(pFlash,&(self->conf->iGanancia),val);
}

/*  Cero  Termopar  */
/*
** ===================================================================
**     Method      :  get_CeroTermopar 
**    Description :   Obtiene el valor del ajuste de cero para termopar 
** ===================================================================
*/
int get_CeroTermopar(const struct TSensor_TermoPT * self){
  return  self->conf->iCeroTermopar;
}

/*
** ===================================================================
**     Method      :  set_CeroTermopar 
**    Description :   Setea el valor del ajuste de cero para termopar 
** ===================================================================
*/
byte set_CeroTermopar(struct TSensor_TermoPT * self,int val){
  return _MANEJADOR_MEMORIA_SET_WORD(pFlash,&(self->conf->iCeroTermopar),val);
}

/*  Ganancia  Termopar  */
/*
** ========================================================================
**     Method      :  get_GananciaTermopar 
**    Description :   Obtiene el valor del ajuste de ganancia para termopar 
** ========================================================================
*/
int get_GananciaTermopar(const struct TSensor_TermoPT * self){
  return  self->conf->iGananciaTermopar;
}

/*
** ======================================================================
**     Method      :  set_GananciaTermopar 
**    Description :   Setea el valor del ajuste de ganancia para termopar 
** ======================================================================
*/
byte set_GananciaTermopar(struct TSensor_TermoPT * self,int val){
  return _MANEJADOR_MEMORIA_SET_WORD(pFlash,&(self->conf->iGananciaTermopar),val);
}

/*  Cero  PT100  */
/*
** =====================================================================
**     Method      :  get_CeroPT100 
**    Description :   Obtiene el valor del ajuste de cero para PT100 
** =====================================================================
*/
int get_CeroPT100(const struct TSensor_TermoPT * self){
  return  self->conf->iCeroPT100;
}

/*
** ===================================================================
**     Method      :  set_CeroPT100 
**    Description :   Setea el valor del ajuste de cero para PT100 
** ===================================================================
*/
byte set_CeroPT100(struct TSensor_TermoPT * self,int val){
  return _MANEJADOR_MEMORIA_SET_WORD(pFlash,&(self->conf->iCeroPT100),val);
}

/*  Ganancia  PT100  */
/*
** =====================================================================
**     Method      :  get_GananciaPT100 
**    Description :   Obtiene el valor del ajuste de ganancia para PT100 
** =====================================================================
*/
int get_GananciaPT100(const struct TSensor_TermoPT * self){
  return  self->conf->iGananciaPT100;
}

/*
** =====================================================================
**     Method      :  set_GananciaPT100 
**    Description :   Setea el valor del ajuste de ganancia para PT100 
** =====================================================================
*/
byte set_GananciaPT100(struct TSensor_TermoPT * self,int val){
  return _MANEJADOR_MEMORIA_SET_WORD(pFlash,&(self->conf->iGananciaPT100),val);
}

/*  */
/*
** =====================================================================
**     Method      :  get_AjTempAmb 
**    Description :   Obtiene el valor del Ajuste para compensar 
**                    la temperatura ambiente 
** =====================================================================
*/
int get_AjTempAmb(void){
  return  iCompensacionTempAmb;
}

/*
** =====================================================================
**     Method      :  set_AjTempAmb 
**    Description :   Setea el valor del Ajuste para compensar 
**                    la temperatura ambiente 
** =====================================================================
*/
byte set_AjTempAmb(void*a,int val){
  return _MANEJADOR_MEMORIA_SET_WORD(pFlash,&iCompensacionTempAmb,val);
}

/*
** =====================================================================
**     Method      :  SensorAdaptador_getTiempoMuestreo 
**    Description :   Devuelve el tiempo de mustreo en milisegundos
** =====================================================================
*/
uint SensorAdaptador_getTiempoMuestreo(struct TSensor_TermoPT * self){
  return ADTIME;
}

/* END Sensores. */
