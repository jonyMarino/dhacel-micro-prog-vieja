#ifndef _SENSOR_ADAPTADOR_H
#define _SENSOR_ADAPTADOR_H

#include "stddef.h"	
#include "Mydefines.h"
//#include "Sensor.h" 
#include "sensores.h"
//#include "MethodContainer.hpp"

#pragma CONST_SEG DEFAULT

extern const struct SensorDecLimClass SensorAdaptador;

struct SensorAdaptador{
  const struct SensorVisual sensor;			// vtable
  byte numSensor;
};
/******************************************/


/*
** ===================================================================
**     Method      :  Sensor_Construct 
**     Description :  Constructor del Sensor
** ===================================================================
*/

void  SensorAdaptador_Construct(struct SensorAdaptador * self,
                      byte numSensor,
                      const char *desc);

/*
** ===================================================================
**     Method      :  SensorAdaptador_getState 
**     Description :  Indica el estado del sensor(UF,OF,OK)
** ===================================================================
*/
TSensorState SensorAdaptador_getState(const struct SensorAdaptador * self);
/*
** ===================================================================
**     Method      :  SensorAdaptador_getValue 
**     Description :  Devuelve el último valor procesado del AD
** ===================================================================
*/
int SensorAdaptador_getValue(struct SensorAdaptador * self);
/*
** ===================================================================
**     Method      :  SensorAdaptador_getDesc 
**     Returns		 :  Obtiene el nombre del sensor
** ===================================================================
*/
char *  SensorAdaptador_getDesc(void * _self);
/*
** ===================================================================
**     Method      :  SensorHandler 
**     Description :  Checkea si hay un nuevo valor del AD, 
**                    y si lo hay lo procesa.
** ===================================================================
*/
void SensorAdaptador_Handler(struct SensorAdaptador * self);

/// FUNCIONES DE SETEO Y OBTENCION DE VARIABLES 
/*
** ===================================================================
**     Method      :  get_LimInf_Sensor 
**     Description :  Limite inferior de temperatura del sensor
** ===================================================================
*/

int get_LimInf_Sensor(struct SensorAdaptador * self);

/*
** ===================================================================
**     Method      :  get_LimSup_Sensor 
**     Description :  Limite superior de temperatura del sensor
** ===================================================================
*/

int get_LimSup_Sensor(struct SensorAdaptador * self);


/*  Sensor  */
/*
** ===================================================================
**     Method      :  get_Sensor 
**     Description :  Obtiene el sensor actual(J,K,etc)
** ===================================================================
*/
int get_Sensor(const struct SensorAdaptador * self);

/*
** ===================================================================
**     Method      :  set_Sensor 
**     Description :  Setea el sensor (J,K,etc)
** ===================================================================
*/
uchar set_Sensor(struct SensorAdaptador * self,int sensor);
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
uchar Sensor_ChangeTipo(struct SensorAdaptador * self,int val);
/*  Decimales */
/*
** ===================================================================
**     Method      :  SENSOR_setDecimales 
**    Description : Setea la cantidad de decimales
** ===================================================================
*/
byte SensorAdaptador_setDecimales(struct SensorAdaptador * self, int val);
/*
** ===================================================================
**     Method      :  SENSOR_getDecimales 
**    Description :   Obtiene la cantidad de decimales
** ===================================================================
*/
int SensorAdaptador_getDecimalesMostrados(const struct SensorAdaptador * self);
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
int get_LimSup_Decimales(const struct SensorAdaptador * self);

/*  Filtro  */
/*
** ===================================================================
**     Method      :  get_filtro 
**    Description :   Obtiene el valor del filtro
** ===================================================================
*/
int get_filtro(const struct SensorAdaptador * self);

/*
** ===================================================================
**     Method      :  set_filtro 
**    Description :   Setea el valor del filtro
** ===================================================================
*/
byte set_filtro(struct SensorAdaptador * self,int val);
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
int get_offset(const struct SensorAdaptador * self);

/*
** ===================================================================
**     Method      :  set_offset 
**    Description :   Setea el valor del Offset
** ===================================================================
*/
byte set_offset(struct SensorAdaptador * self,int val);
/*  Ganancia  */
/*
** ===================================================================
**     Method      :  get_offset 
**    Description :   Obtiene el valor de la Ganancia
** ===================================================================
*/
int get_ganancia(const struct SensorAdaptador * self);
/*
** ===================================================================
**     Method      :  set_offset 
**    Description :   Setea el valor de la Ganancia
** ===================================================================
*/
byte set_ganancia(struct SensorAdaptador * self,int val);

/*  Cero  Termopar  */
/*
** ===================================================================
**     Method      :  get_CeroTermopar 
**    Description :   Obtiene el valor del ajuste de cero para termopar 
** ===================================================================
*/
int get_CeroTermopar(const struct SensorAdaptador * self);
/*
** ===================================================================
**     Method      :  set_CeroTermopar 
**    Description :   Setea el valor del ajuste de cero para termopar 
** ===================================================================
*/
byte set_CeroTermopar(struct SensorAdaptador * self,int val);

/*  Ganancia  Termopar  */
/*
** ========================================================================
**     Method      :  get_GananciaTermopar 
**    Description :   Obtiene el valor del ajuste de ganancia para termopar 
** ========================================================================
*/
int get_GananciaTermopar(const struct SensorAdaptador * self);

/*
** ======================================================================
**     Method      :  set_GananciaTermopar 
**    Description :   Setea el valor del ajuste de ganancia para termopar 
** ======================================================================
*/
byte set_GananciaTermopar(struct SensorAdaptador * self,int val);

/*  Cero  PT100  */
/*
** =====================================================================
**     Method      :  get_CeroPT100 
**    Description :   Obtiene el valor del ajuste de cero para PT100 
** =====================================================================
*/
int get_CeroPT100(const struct SensorAdaptador * self);

/*
** ===================================================================
**     Method      :  set_CeroPT100 
**    Description :   Setea el valor del ajuste de cero para PT100 
** ===================================================================
*/
byte set_CeroPT100(struct SensorAdaptador * self,int val);
/*  Ganancia  PT100  */
/*
** =====================================================================
**     Method      :  get_GananciaPT100 
**    Description :   Obtiene el valor del ajuste de ganancia para PT100 
** =====================================================================
*/
int get_GananciaPT100(const struct SensorAdaptador * self);

/*
** =====================================================================
**     Method      :  set_GananciaPT100 
**    Description :   Setea el valor del ajuste de ganancia para PT100 
** =====================================================================
*/
byte set_GananciaPT100(struct SensorAdaptador * self,int val);
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
**     Method      :  SensorAdaptador_Print 
**     Description :  Imprime el valor del sensor
** ===================================================================
*/
void SensorAdaptador_Print(struct SensorAdaptador * self,uchar num_display);
void SensorAdaptador_printVal(struct SensorAdaptador * self,struct OutputStream* os);



#endif




