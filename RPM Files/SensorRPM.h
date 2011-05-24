#ifndef _SENSOR_RPM_H
#define _SENSOR_RPM_H

#include "stddef.h"	
#include "Mydefines.h"
#include "Errores.h"
#include "Sensor.h" 
#include "sensores.h"
#include "MethodContainer.hpp"
#include "Capturador.h"
#include "MethodTimer.h"
#include "adjuntador_a_hilo.h"
//#include "ThreadAttachable.hpp"
#include "FshTypeSizes.h"
#include "RlxMTimer.h"

#pragma CONST_SEG DEFAULT


extern const struct SensorDecLimClass _SensorRpm;



typedef struct {
  fbyte iDecimales;
  fbyte iFiltro;
  int pulsosPorVuelta;
}SensorRpmConf;

#define DEFAULT_SENSOR_RPM_CONF {	 \
  0,															 \
  0,															 \
  1															 \
}																	 


/********Clase*********************/
struct SensorRpm{
  SensorRpm();
  const struct SensorVisual base;			// clase base
  volatile const SensorRpmConf * conf;		// Variables de configuración en Rom
  uint procVal;  											// Valor Leido y procesado del sensor
  long bufferFiltro;                     // Privado: acumulacion del filtro  
 // long buffer;
  TSensorState state;									// Estado del sensor
   MethodContainer * onNewVal;
  struct Capturador * capturador;
  //struct MethodTimer timerMuestreo;
  struct RlxMTimer timerMuestreo; 
  bool procesar;
  byte ContadorUf; //contador de errores para saber cuando poner UF
};
/******************************************/


/*
** ===================================================================
**     Method      :  SenTPT_getState 
**     Description :  Indica el estado del sensor(UF,OF,OK)
** ===================================================================
*/
TSensorState SenRpm_getState(const struct SensorRpm * self);
/*
** ===================================================================
**     Method      :  SenRPM_getDesc 
**     Returns		 :  Obtiene la descripcion del sensor
** ===================================================================
*/
char *  SenRpm_getDesc(void * _self);
/*
** ===================================================================
**     Method      :  SenRpm_getValue 
**     Description :  Devuelve el último valor procesado
** ===================================================================
*/
int SenRpm_getValue(struct SensorRpm * self);
/*
** ===================================================================
**     Method      :  SenRpm_getDesc 
**     Returns		 :  Obtiene el nombre del sensor
** ===================================================================
*/
char *  SenRpm_getDesc(void * _self);

/*
** ===================================================================
**     Method      :  SENSOR_setDecimales 
**    Description : Setea la cantidad de decimales
** ===================================================================
*/
byte SenRpm_setDecimales(struct SensorRpm * self, int val);

/*
** ===================================================================
**     Method      :  SENSOR_getDecimales 
**    Description :   Obtiene la cantidad de decimales
** ===================================================================
*/
int SenRpm_getDecimales(const struct SensorRpm * self);
/*
** ==========================================================================
**     Method      :  get_LimInf_Decimales 
**    Type:   Static
**    Description :   Obtiene el minimo valor de decimales que puede setearse
** ==========================================================================
*/
int SenRpm_getLimInfDecimales(void);

/*
** ==========================================================================
**     Method      :  get_LimSup_Decimales 
**    Description :   Obtiene el maximo valor de decimales que puede setearse
** ==========================================================================
*/
int SenRpm_getLimSupDecimales(const struct SensorRpm * self);



/*  Filtro  */
/*
** ===================================================================
**     Method      :  get_filtro 
**    Description :   Obtiene el valor del filtro
** ===================================================================
*/
int SenRpm_getFiltro(const struct SensorRpm * self);

/*
** ===================================================================
**     Method      :  set_filtro 
**    Description :   Setea el valor del filtro
** ===================================================================
*/
byte SenRpm_setFiltro(struct SensorRpm * self,int val);

/*
** ===================================================================
**    Method      :   get_LimInf_filtro 
**    Type        :   Static
**    Description :   Lim inf del valor del filtro
** ===================================================================
*/
int SenRpm_getLimInfFiltro(void);
/*
** ===================================================================
**    Method      :   get_LimSup_filtro 
**    Type        :   Static
**    Description :   Lim sup del valor del filtro
** ===================================================================
*/
int SenRpm_getLimSupFiltro(void);

/*  Pulsos por vuelta  */
/*
** ===================================================================
**     Method      :  SenRpm_getPulsosPorVuelta 
**    Description :   Obtiene el valor 
** ===================================================================
*/
int SenRpm_getPulsosPorVuelta(const struct SensorRpm * self);

/*
** ===================================================================
**     Method      :  SenRpm_setPulsosPorVuelta 
**    Description :   Setea el valor 
** ===================================================================
*/
byte SenRpm_setPulsosPorVuelta(struct SensorRpm * self,int val);

/*
** ===================================================================
**    Method      :   SenRpm_getLimInfPulsosPorVuelta 
**    Type        :   Static
**    Description :   Lim inf del valor
** ===================================================================
*/
int SenRpm_getLimInfPulsosPorVuelta(void);



#endif




