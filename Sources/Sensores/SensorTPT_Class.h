#ifndef _SENSOR_TPT_CLASS_H
#define _SENSOR_TPT_CLASS_H

#include "stddef.h"	
#include "Mydefines.h"
#include "Errores.h"
#include "ClassADC.h"
#include "ClassSensor.h" // Para herencia de clase pura
#include "sensores.h"
#include "MethodContainer.h"
#include "FshTypeSizes.h"

#pragma CONST_SEG DEFAULT

extern const void * const TSensor_TermoPT;


/********Objeto*********************/
typedef struct {
  fbyte eSensor;	// (t_sensor)
  fbyte iDecimales;
  fbyte iFiltro;
  int iOffset;
  int iGanancia;
  int iCeroTermopar;
  int iGananciaTermopar;
  int iCeroPT100;
  int iGananciaPT100;
}SensorConf;

//extern const SensorConf DefaultSensorConf; //Configuración por defecto
#define STPT_DEF_CONF { \
  SENSOR_MV,						\
  2,										\
  5,										\
  0,										\
  1000,									\
  0,										\
  0,										\
  0,										\
  0  									  \
}

struct TSensor_TermoPT{
  const struct SensorVisual sensor;			// vtable
  volatile const SensorConf * conf;		// Variables de configuración en Rom
  struct TAdc * Adc;  											  // Pin del ADC al que pertenece este sensor
  int ProcVal;  											// Valor Leido y procesado del sensor
  bool newVal;                        //indica si el valor es nuevo
  long buffer_fil;                     // Privado: acumulacion del filtro  
  TSensorState state;									// Estado del sensor
};
/******************************************/


/*
** ===================================================================
**     Method      :  Sensor_Construct 
**     Description :  Constructor del Sensor
** ===================================================================
*/

void  SenTPT_Construct(struct TSensor_TermoPT * self,
                      struct TAdc* adc,
                      const SensorConf * conf,
                      const char *desc);

/*
** ===================================================================
**     Method      :  SenTPT_getState 
**     Description :  Indica el estado del sensor(UF,OF,OK)
** ===================================================================
*/
TSensorState SenTPT_getState(const struct TSensor_TermoPT * self);
/*
** ===================================================================
**     Method      :  SenTPT_getValue 
**     Description :  Devuelve el último valor procesado del AD
** ===================================================================
*/
int SenTPT_getValue(struct TSensor_TermoPT * self);
/*
** ===================================================================
**     Method      :  SenTPT_getDesc 
**     Returns		 :  Obtiene el nombre del sensor
** ===================================================================
*/
char *  SenTPT_getDesc(void * _self);
/*
** ===================================================================
**     Method      :  SensorHandler 
**     Description :  Checkea si hay un nuevo valor del AD, 
**                    y si lo hay lo procesa.
** ===================================================================
*/
void SenTPT_Handler(struct TSensor_TermoPT * self);

/// FUNCIONES DE SETEO Y OBTENCION DE VARIABLES 
/*
** ===================================================================
**     Method      :  get_LimInf_Sensor 
**     Description :  Limite inferior de temperatura del sensor
** ===================================================================
*/

int get_LimInf_Sensor(struct TSensor_TermoPT * self);

/*
** ===================================================================
**     Method      :  get_LimSup_Sensor 
**     Description :  Limite superior de temperatura del sensor
** ===================================================================
*/

int get_LimSup_Sensor(struct TSensor_TermoPT * self);


/*  Sensor  */
/*
** ===================================================================
**     Method      :  get_Sensor 
**     Description :  Obtiene el sensor actual(J,K,etc)
** ===================================================================
*/
int get_Sensor(const struct TSensor_TermoPT * self);

/*
** ===================================================================
**     Method      :  set_Sensor 
**     Description :  Setea el sensor (J,K,etc)
** ===================================================================
*/
TError set_Sensor(struct TSensor_TermoPT * self,int sensor);
/*
** ===================================================================
**     Function      :  get_LimInf_Sensores 
**     Type : Static 
**     Description :  Obtiene el minimo valor correspondiente a un int
**                    en el que se corresponde a un sensor. 
** ===================================================================
*/																				 
int get_LimInf_Sensores(void);
/*
** ===================================================================
**     Function      :  get_LimInf_Sensores
**     Type : Static  
**     Description :  Obtiene el minimo valor en integer
**                    en el que se corresponde a un sensor.
** ===================================================================
*/	
int get_LimSup_Sensores(void);


/*
** ===================================================================
**     Method      :  Sensor_ChangeTipo 
**    Description : Cambia el tipo de sensor (K,J,etc.)
** ===================================================================
*/
TError Sensor_ChangeTipo(struct TSensor_TermoPT * self,int val);
/*  Decimales */
/*
** ===================================================================
**     Method      :  SENSOR_setDecimales 
**    Description : Setea la cantidad de decimales
** ===================================================================
*/
byte SenTPT_setDecimales(struct TSensor_TermoPT * self, int val);
/*
** ===================================================================
**     Method      :  SENSOR_getDecimales 
**    Description :   Obtiene la cantidad de decimales
** ===================================================================
*/
int SenTPT_getDecimalesMostrados(const struct TSensor_TermoPT * self);
/*
** ==========================================================================
**     Method      :  get_LimInf_Decimales 
**    Type:   Static
**    Description :   Obtiene el minimo valor de decimales que puede setearse
** ==========================================================================
*/
int get_LimInf_Decimales(void);
/*
** ==========================================================================
**     Method      :  get_LimSup_Decimales 
**    Description :   Obtiene el maximo valor de decimales que puede setearse
** ==========================================================================
*/
int get_LimSup_Decimales(const struct TSensor_TermoPT * self);

/*  Filtro  */
/*
** ===================================================================
**     Method      :  get_filtro 
**    Description :   Obtiene el valor del filtro
** ===================================================================
*/
int get_filtro(const struct TSensor_TermoPT * self);

/*
** ===================================================================
**     Method      :  set_filtro 
**    Description :   Setea el valor del filtro
** ===================================================================
*/
byte set_filtro(struct TSensor_TermoPT * self,int val);
/*
** ===================================================================
**    Method      :   get_LimInf_filtro 
**    Type        :   Static
**    Description :   Lim inf del valor del filtro
** ===================================================================
*/
int get_LimInf_filtro(void);

/*
** ===================================================================
**    Method      :   get_LimSup_filtro 
**    Type        :   Static
**    Description :   Lim sup del valor del filtro
** ===================================================================
*/
int get_LimSup_filtro(void);

/*  Offset  */
/*
** ===================================================================
**     Method      :  get_offset 
**    Description :   Obtiene el valor del Offset
** ===================================================================
*/
int get_offset(const struct TSensor_TermoPT * self);

/*
** ===================================================================
**     Method      :  set_offset 
**    Description :   Setea el valor del Offset
** ===================================================================
*/
byte set_offset(struct TSensor_TermoPT * self,int val);
/*  Ganancia  */
/*
** ===================================================================
**     Method      :  get_offset 
**    Description :   Obtiene el valor de la Ganancia
** ===================================================================
*/
int get_ganancia(const struct TSensor_TermoPT * self);
/*
** ===================================================================
**     Method      :  set_offset 
**    Description :   Setea el valor de la Ganancia
** ===================================================================
*/
byte set_ganancia(struct TSensor_TermoPT * self,int val);

/*  Cero  Termopar  */
/*
** ===================================================================
**     Method      :  get_CeroTermopar 
**    Description :   Obtiene el valor del ajuste de cero para termopar 
** ===================================================================
*/
int get_CeroTermopar(const struct TSensor_TermoPT * self);
/*
** ===================================================================
**     Method      :  set_CeroTermopar 
**    Description :   Setea el valor del ajuste de cero para termopar 
** ===================================================================
*/
byte set_CeroTermopar(struct TSensor_TermoPT * self,int val);

/*  Ganancia  Termopar  */
/*
** ========================================================================
**     Method      :  get_GananciaTermopar 
**    Description :   Obtiene el valor del ajuste de ganancia para termopar 
** ========================================================================
*/
int get_GananciaTermopar(const struct TSensor_TermoPT * self);

/*
** ======================================================================
**     Method      :  set_GananciaTermopar 
**    Description :   Setea el valor del ajuste de ganancia para termopar 
** ======================================================================
*/
byte set_GananciaTermopar(struct TSensor_TermoPT * self,int val);

/*  Cero  PT100  */
/*
** =====================================================================
**     Method      :  get_CeroPT100 
**    Description :   Obtiene el valor del ajuste de cero para PT100 
** =====================================================================
*/
int get_CeroPT100(const struct TSensor_TermoPT * self);

/*
** ===================================================================
**     Method      :  set_CeroPT100 
**    Description :   Setea el valor del ajuste de cero para PT100 
** ===================================================================
*/
byte set_CeroPT100(struct TSensor_TermoPT * self,int val);
/*  Ganancia  PT100  */
/*
** =====================================================================
**     Method      :  get_GananciaPT100 
**    Description :   Obtiene el valor del ajuste de ganancia para PT100 
** =====================================================================
*/
int get_GananciaPT100(const struct TSensor_TermoPT * self);

/*
** =====================================================================
**     Method      :  set_GananciaPT100 
**    Description :   Setea el valor del ajuste de ganancia para PT100 
** =====================================================================
*/
byte set_GananciaPT100(struct TSensor_TermoPT * self,int val);
/*  */
/*
** =====================================================================
**     Method      :  get_AjTempAmb 
**    Description :   Obtiene el valor del Ajuste para compensar 
**                    la temperatura ambiente 
** =====================================================================
*/
int get_AjTempAmb(void);

/*
** =====================================================================
**     Method      :  set_AjTempAmb 
**    Description :   Setea el valor del Ajuste para compensar 
**                    la temperatura ambiente 
** =====================================================================
*/
byte set_AjTempAmb(void* null,int val);

/*
** ===================================================================
**     Method      :  SenTPT_Print 
**     Description :  Imprime el valor del sensor
** ===================================================================
*/
void SenTPT_Print(struct TSensor_TermoPT * self,uchar num_display);
void SenTPT_printVal(struct TSensor_TermoPT * self,struct OutputStream* os);



#endif




