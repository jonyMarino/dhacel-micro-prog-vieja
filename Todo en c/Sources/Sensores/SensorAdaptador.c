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
#include "PTSL.h"
#include "Events.h"
#include "Math.h"
#include "Sensor.h"
#include "Sensores.h"
//#include "ClassLongSensor.h"
#include "display.h"
#include "WDog1.h"
#include "SensorAdaptador.h"
#include "OutputStream.h"
#include "str_lib.h"
#include "cnfbox.h"

extern volatile const int PRom[PARAMETROS];

#pragma CONST_SEG DEFAULT

/*  Funciones internas  */
void  SensorAdaptador_DefConstruct(struct SensorAdaptador * self,va_list * args);
uchar Sensor_ChangeTipo(const struct SensorAdaptador * sensor,int val);
void Sensor_Procesar(struct SensorAdaptador * self);  
int SensorAdaptador_getValue(struct SensorAdaptador * self); 
void SensorAdaptador_AddOnNew(struct SensorAdaptador * self,void(*method)(void*),void*Obj);
void SensorAdaptador_Print(struct SensorAdaptador * self,uchar num_display); 
int SensorAdaptador_getDifDecView(struct SensorAdaptador * self);
uint SensorAdaptador_getTiempoMuestreo(struct SensorAdaptador * self);
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

#pragma CONST_SEG PARAMETERS_PAGE
volatile const int iCompensacionTempAmb; 
#pragma CONST_SEG DEFAULT
            
/*
** ===================================================================
**     Method      :  Sensor_Construct 
**     Description :  Constructor del Sensor
** ===================================================================
*/

void  SensorAdaptador_Construct(struct SensorAdaptador * self,byte numSensor,const char * desc){
  Sensor_constructor(self);
  _SensorVisual_setDescription(self,desc);
  self->numSensor=numSensor;
}

/*
** ===================================================================
**     Method      :  Sensor_DefConstruct 
**     Description :  Constructor por defecto del Sensor
** ===================================================================
*/

void  SensorAdaptador_DefConstruct(struct SensorAdaptador * self,va_list * args){
  SensorAdaptador_Construct(self,va_arg(*args,int),va_arg(*args,char*));  
}

/*
** ===================================================================
**     Method      :  SensorAdaptador_getValue 
**     Description :  Devuelve el último valor procesado del AD
** ===================================================================
*/
int SensorAdaptador_getValue(struct SensorAdaptador * self){
  extern int ValFinal[4];
  return ValFinal[self->numSensor];
}

/*
** ===================================================================
**     Method      :  SensorAdaptador_getState 
**     Description :  Indica el estado del sensor(UF,OF,OK)
** ===================================================================
*/
TSensorState SensorAdaptador_getState(const struct SensorAdaptador * self){
  extern word Result[4];	
  return Result[self->numSensor];
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
**     Method      :  SensorAdaptador_Print 
**     Description :  Imprime el valor del sensor
** ===================================================================
*/
void SensorAdaptador_Print(struct SensorAdaptador * self,uchar num_display){

int Val=SensorAdaptador_getValue(self); 
int i = self->numSensor; 
t_sensor sensor=PRom[R_Sensor+i]; 
byte decimales=SENSOR_Decimales_Mostrar(sensor);


  if ((sensor!=SENSOR_LIN1) &&(sensor!=SENSOR_LIN2) && (sensor!=SENSOR_LIN3) && (sensor!=SENSOR_LIN4))      												 
      Val/= pow10(SENSOR_Decimales(sensor)-decimales);  			//Ajusto el valor a mostrar por la cantidad de decimales del sensor
  
switch (SensorAdaptador_getState(self)){
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
void SensorAdaptador_printVal(struct SensorAdaptador * self,struct OutputStream* os){
byte decimales=_getDec(self);
int Val=SensorAdaptador_getValue(self); 
											 
  
    
  switch (SensorAdaptador_getState(self)){
    case SENSOR_OK:
        if(Val>9999)
          write(os," OF ");
        else{
          char str[7];
          FloatToStr(Val,str,6,decimales);
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
int SensorAdaptador_getDifDecView(struct SensorAdaptador * self){
byte decimales=_getDec(self);
int i = self->numSensor; 
t_sensor sensor=PRom[R_Sensor+i]; 


  return SENSOR_Decimales(sensor)-decimales; 
}






/// FUNCIONES DE SETEO Y OBTENCION DE VARIABLES 
/*
** ===================================================================
**     Method      :  get_LimInf_Sensor 
**     Description :  Limite inferior de temperatura del sensor
** ===================================================================
*/

int get_LimInf_Sensor(struct SensorAdaptador * self){
int i = self->numSensor; 
t_sensor sensor=PRom[R_Sensor+i];
int min_sens=SENSOR_Minimo(sensor);      
byte decimal_chan = SENSOR_Decimales(sensor);      
byte decimales=SENSOR_Decimales_Mostrar(sensor);

  if((sensor!=SENSOR_LIN1) &&(sensor!=SENSOR_LIN2) && (sensor!=SENSOR_LIN3) && (sensor!=SENSOR_LIN4))
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

int get_LimSup_Sensor(struct SensorAdaptador * self){
int i = self->numSensor; 
t_sensor sensor=PRom[R_Sensor+i];
int max_sens=SENSOR_Maximo(sensor);
byte decimal_chan = SENSOR_Decimales(sensor);      
byte decimales=SensorAdaptador_getDecimalesMostrados(self);

  if((sensor!=SENSOR_LIN1) &&(sensor!=SENSOR_LIN2) && (sensor!=SENSOR_LIN3) && (sensor!=SENSOR_LIN4))
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
int get_Sensor(const struct SensorAdaptador * self){
  int i = self->numSensor; 
  t_sensor sensor=PRom[R_Sensor+i];
  return sensor;
}

/*
** ===================================================================
**     Method      :  set_Sensor 
**     Description :  Setea el sensor (J,K,etc)
** ===================================================================
*/
uchar set_Sensor(struct SensorAdaptador * self,int sensor){

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
uchar Sensor_ChangeTipo(struct SensorAdaptador * self,int val){
 
}
/*  Decimales */
/*
** ===================================================================
**     Method      :  SENSOR_setDecimales 
**    Description : Setea la cantidad de decimales
** ===================================================================
*/
byte SensorAdaptador_setDecimales(struct SensorAdaptador * self, int val){
  
}

/*
** ===================================================================
**     Method      :  SENSOR_getDecimales 
**    Description :   Obtiene la cantidad de decimales
** ===================================================================
*/
int SensorAdaptador_getDecimalesMostrados(const struct SensorAdaptador * self){
    int i = self->numSensor; 
  t_sensor sensor=PRom[R_Sensor+i];
  return SENSOR_Decimales_Mostrar(sensor);
}

/*
** ===================================================================
**     Method      :  SENSOR_getDecimales 
**    Description :   Obtiene la cantidad de decimales
** ===================================================================
*/
int SensorAdaptador_getDecimales(const struct SensorAdaptador * self){
										 
  return SENSOR_Decimales(get_Sensor(self));  			//Ajusto el valor a mostrar por la cantidad de decimales del sensor
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
int get_LimSup_Decimales(const struct SensorAdaptador * self){
  return SENSOR_Decimales(get_Sensor(self));
}

/*  Filtro  */
/*
** ===================================================================
**     Method      :  get_filtro 
**    Description :   Obtiene el valor del filtro
** ===================================================================
*/
int get_filtro(const struct SensorAdaptador * self){
  return PRom[R_Filtro1+self->numSensor];
}

/*
** ===================================================================
**     Method      :  set_filtro 
**    Description :   Setea el valor del filtro
** ===================================================================
*/
byte set_filtro(struct SensorAdaptador * self,int val){
//  return _MANEJADOR_MEMORIA_SET_BYTE(pFlash,&(self->conf->iFiltro),val);
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
  return 255;
}

/*  Offset  */
/*
** ===================================================================
**     Method      :  get_offset 
**    Description :   Obtiene el valor del Offset
** ===================================================================
*/
int get_offset(const struct SensorAdaptador * self){
  return PRom[R_Offset+self->numSensor];
}

/*
** ===================================================================
**     Method      :  set_offset 
**    Description :   Setea el valor del Offset
** ===================================================================
*/
byte set_offset(struct SensorAdaptador * self,int val){
//  return _MANEJADOR_MEMORIA_SET_WORD(pFlash,&(self->conf->iOffset),val);
}

/*  Ganancia  */
/*
** ===================================================================
**     Method      :  get_offset 
**    Description :   Obtiene el valor de la Ganancia
** ===================================================================
*/
int get_ganancia(const struct SensorAdaptador * self){
  return PRom[R_Gan+self->numSensor];
}

/*
** ===================================================================
**     Method      :  set_offset 
**    Description :   Setea el valor de la Ganancia
** ===================================================================
*/
byte set_ganancia(struct SensorAdaptador * self,int val){
 // return _MANEJADOR_MEMORIA_SET_WORD(pFlash,&(self->conf->iGanancia),val);
}

/*  Cero  Termopar  */
/*
** ===================================================================
**     Method      :  get_CeroTermopar 
**    Description :   Obtiene el valor del ajuste de cero para termopar 
** ===================================================================
*/
int get_CeroTermopar(const struct SensorAdaptador * self){
  return  PRom[R_ACT+self->numSensor];
}

/*
** ===================================================================
**     Method      :  set_CeroTermopar 
**    Description :   Setea el valor del ajuste de cero para termopar 
** ===================================================================
*/
byte set_CeroTermopar(struct SensorAdaptador * self,int val){
//  return _MANEJADOR_MEMORIA_SET_WORD(pFlash,&(self->conf->iCeroTermopar),val);
}

/*  Ganancia  Termopar  */
/*
** ========================================================================
**     Method      :  get_GananciaTermopar 
**    Description :   Obtiene el valor del ajuste de ganancia para termopar 
** ========================================================================
*/
int get_GananciaTermopar(const struct SensorAdaptador * self){
  return  PRom[R_AGT+self->numSensor];
}

/*
** ======================================================================
**     Method      :  set_GananciaTermopar 
**    Description :   Setea el valor del ajuste de ganancia para termopar 
** ======================================================================
*/
byte set_GananciaTermopar(struct SensorAdaptador * self,int val){
 // return _MANEJADOR_MEMORIA_SET_WORD(pFlash,&(self->conf->iGananciaTermopar),val);
}

/*  Cero  PT100  */
/*
** =====================================================================
**     Method      :  get_CeroPT100 
**    Description :   Obtiene el valor del ajuste de cero para PT100 
** =====================================================================
*/
int get_CeroPT100(const struct SensorAdaptador * self){
  return  PRom[R_ACP+self->numSensor];
}

/*
** ===================================================================
**     Method      :  set_CeroPT100 
**    Description :   Setea el valor del ajuste de cero para PT100 
** ===================================================================
*/
byte set_CeroPT100(struct SensorAdaptador * self,int val){
 // return _MANEJADOR_MEMORIA_SET_WORD(pFlash,&(self->conf->iCeroPT100),val);
}

/*  Ganancia  PT100  */
/*
** =====================================================================
**     Method      :  get_GananciaPT100 
**    Description :   Obtiene el valor del ajuste de ganancia para PT100 
** =====================================================================
*/
int get_GananciaPT100(const struct SensorAdaptador * self){
  return  PRom[R_AGP+self->numSensor];
}

/*
** =====================================================================
**     Method      :  set_GananciaPT100 
**    Description :   Setea el valor del ajuste de ganancia para PT100 
** =====================================================================
*/
byte set_GananciaPT100(struct SensorAdaptador * self,int val){
//  return _MANEJADOR_MEMORIA_SET_WORD(pFlash,&(self->conf->iGananciaPT100),val);
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
  return  PRom[R_ATA];
}

/*
** =====================================================================
**     Method      :  set_AjTempAmb 
**    Description :   Setea el valor del Ajuste para compensar 
**                    la temperatura ambiente 
** =====================================================================
*/
byte set_AjTempAmb(void*a,int val){
 // return _MANEJADOR_MEMORIA_SET_WORD(pFlash,&iCompensacionTempAmb,val);
}

/*
** =====================================================================
**     Method      :  SensorAdaptador_getTiempoMuestreo 
**    Description :   Devuelve el tiempo de mustreo en milisegundos
** =====================================================================
*/
uint SensorAdaptador_getTiempoMuestreo(struct SensorAdaptador * self){
  return AD_TIME_EN_MILI_SEGUNDOS;
}

/* END Sensores. */
