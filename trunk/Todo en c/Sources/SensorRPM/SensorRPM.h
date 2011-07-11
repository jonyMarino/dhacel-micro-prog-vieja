#ifndef _SENSOR_RPM_H
#define _SENSOR_RPM_H

#include "stddef.h"	
#include "Mydefines.h"
#include "Errores.h"
#include "sensores.h"
#include "MethodContainer.h"
#include "RlxMTimer.h"

#pragma CONST_SEG DEFAULT

typedef enum {
  RPM_OK,  //OK
  RPM_OF,  //Over Flow
  RPM_UF   //Under Flow
}TrPMState;


typedef struct {
  word iDecimales;
  word iFiltro;
  int pulsosPorVuelta;
  word AjusteGan;
}SensorRpmConf;


struct SensorRpm{
  volatile const SensorRpmConf * conf;		// Variables de configuración en Rom
  uint procVal;  											    // Valor Leido y procesado del sensor
  long bufferFiltro;                      // Privado: acumulacion del filtro  
  TrPMState state;									    // Estado del sensor
  struct Method * onNewVal;
  struct CapturadorPT2 * capturador;
  struct RlxMTimer * timerMuestreo; 
  bool procesar;
  byte ContadorUf;                          //contador de errores para saber cuando poner UF
};


/*
** ===================================================================
**     Method      :  SenTPT_getState 
**     Description :  Indica el estado del sensor(UF,OF,OK)
** ===================================================================
*/
TrPMState SenRpm_getState(const struct SensorRpm * self);

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

word SenRpm_getAjusteGanancia(const struct SensorRpm * self);

byte SenRpm_setAjusteGanancia(struct SensorRpm * self,int val);

#endif

