#ifndef _SENSORES_H
#define _SENSORES_H

#include "PE_Types.h"
#include "Mydefines.h"

typedef enum{
  TermoCupla,
  TermoResistencia,
  Lineal,
  Mv
}TipoSensor;

typedef struct{
  const int * xdat;
  const int * ydat;
  byte max_array;
  byte decimales_max;
  byte decimales_mostrar;
  TipoSensor tipoSensor;
}T_Sensor;

#ifdef pirani
//#define SEN_PIR 1
#define SEN_PIR 2

#else
#define SEN_PIR 0
#endif

#ifdef SENS_HUMEDAD
#define HUD 1
#else
#define HUD 0
#endif

#define NUM_SENSORES 15+SEN_PIR+HUD

extern const T_Sensor Sensores[NUM_SENSORES];

///////////Sensor//////////////////

typedef enum{
 SENSOR_JB,
 SENSOR_JA,
 SENSOR_N,   
 SENSOR_KA,
 SENSOR_KB,   
 SENSOR_S,   
 SENSOR_R,
 SENSOR_B,   
 SENSOR_T,   
 SENSOR_PT,  
 #ifdef pirani
 SENSOR_PIR, 
 SENSOR_PR1, 
 #endif
 SENSOR_LIN1, 
 SENSOR_LIN2,
 SENSOR_LIN3,
 SENSOR_LIN4,
 SENSOR_MV,
 #ifdef SENS_HUMEDAD
 SENSOR_HUM,
 #endif    
}t_sensor;


/* Devuelve la cantidad de decimales con que se utiliza la tabla del sensor*/
#define SENSOR_Decimales(sensor) Sensores[sensor].decimales_max		
/* Devuelve la cantidad de decimales con que se muestran las lecturas del sensor*/
#define SENSOR_Decimales_Mostrar(sensor) Sensores[sensor].decimales_mostrar		
/* Devulve el minimo valor de temperatura admisible(por el aparato) para Sensor*/
#define SENSOR_Minimo(sensor)  Sensores[sensor].ydat[0]			
/* Devulve el maximo valor de temperatura admisible(por el aparato) para Sensor*/
#define SENSOR_Maximo(sensor)  Sensores[sensor].ydat[Sensores[sensor].max_array]			
/* Devuelve el tipo de Sensor, util para saber como tratarlo */
#define SENSOR_Tipo(sensor)  Sensores[sensor].tipoSensor  

unsigned char Linealizar(long Vx, t_sensor sensor, int * Vy);
int CompTempAmb(t_sensor sensor);

int filtro (int val_x,
            int Tf,
            int DEL_T,
            int TfMAX,
            long *buffer_fil
            );  

#endif