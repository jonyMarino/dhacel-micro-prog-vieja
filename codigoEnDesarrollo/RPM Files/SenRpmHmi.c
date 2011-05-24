/*  MODULE: SnsHmi.c (Sensores Human Machine interface)*/
#include "PropVisual.h"
#include "PropNumerica.h"
#include "PropNumPV.h"
#include "PropTextual.h"

#include "DiagramaNavegacion.h"
#include "ClassBoxLineal.h"
#include "BoxProp.h"
#include "BoxCondicional.h"
#include "DataBox.h"

#include "com_events.h"

#include "SensorRPM.h"

#include "SenRpmHmi.h"

#pragma MESSAGE DISABLE C1825          /* Disable warning C5703 "Parameter is not referenced" */



/*****************************/
/********PROPIEDADES**********/
/*****************************/

  /*Valor del Sensor*/
  const struct GetterGenerico GetterRpmValSen={
    &GetterGenerico,SenRpm_getValue
  };
  
  /*Decimales*/
  const struct ConstructorPropWInc ParRpmDecimales= {
    &PropiedadGenerica,
    SenRpm_getDecimales,
    SenRpm_setDecimales,
    get_0,
    SenRpm_getLimSupDecimales,
    &PropWInc,
    "dec "
  };

  /*Filtro*/
  const struct ConstPropNum ParRpmFiltro={
    &PropiedadGenerica,SenRpm_getFiltro,SenRpm_setFiltro,SenRpm_getLimInfFiltro,SenRpm_getLimSupFiltro,&PropNum,"FiL",1
  };

  /*Pulsos por vuelta*/
  const struct ConstructorPropWInc ParRpmPulsosPorRevolucion={
    &PropiedadGenerica,SenRpm_getPulsosPorVuelta,SenRpm_setPulsosPorVuelta,SenRpm_getLimInfPulsosPorVuelta,NULL,&PropWInc,"P.P.V. "
  };

  
  const void *const SenRpmProps[]={
    &ParRpmDecimales,
    &ParRpmFiltro,
    &ParRpmPulsosPorRevolucion,
    &GetterRpmValSen
  };
  
   const NEW_ARRAY(arraySenGetters,SenRpmProps);
/*
** ===================================================================
**     Function      :  SnsHmi_ComuAdd 
**     Description :  Agrega el sensor a la comunicacion
** ===================================================================
*/

word SenRpmHmi_ComuAdd(const struct SensorRpm * sensor,word dir_ini){
  word cant = sizeof(SenRpmProps)/sizeof(struct GetterGenerico *)-1;
  word dir_end1=dir_ini+cant;
  com_Add(SenRpmProps,sensor,dir_ini,dir_end1);
  return cant;
}
 /***********************/
 /****** BOXES  *********/
 /***********************/
const struct BlockCnstrBoxLin CBox_SenRpm;
 
            
/*
** ===================================================================
**     Function      :  SnsHmi_Add 
**     Description :  Agrega a los boxes el sensor
** ===================================================================
*/

 void SenRpmHmi_Add(const struct SensorRpm * sensor,uchar num_obj){
  DN_AddBox(0,"rPM ",&CBox_SenRpm,sensor,num_obj);
 }
 

/*Boxes*/
const struct ConstructorPropWInc*const Props_SenRpm[]=	{
        &ParRpmPulsosPorRevolucion,
        &ParRpmDecimales,	      
        &ParRpmFiltro,
			  NULL
};
const struct BlockCnstrBoxLin CBox_SenRpm={
      &BoxLineal,									/* funcion que procesa al box*/     
			Props_SenRpm,
			NULL
};			
