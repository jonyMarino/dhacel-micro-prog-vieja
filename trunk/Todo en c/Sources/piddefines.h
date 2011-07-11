#include "ADC.h"
#include "Mydefines.h"
#include "Integrador.h"
#include "Parametros.h"
#include "Medicion.h"

//valores de las variables simbolicas setpoint1

#ifdef jony_18_07
#define DELTAT  AD_TIME_EN_MILI_SEGUNDOS      //milisegundos
#else
#define DELTAT 1 
#endif

//valores de las variables simbolicas alarma1
#define  t_sp_alarma(n)	 getValParametro(R_T_AL+n)
#define  t_sal_alarma(n) getValParametro(R_Talar1+n)

#ifdef INTEGRADOR                                            
 #define  vx(n)        valorAmostrar[n]
 #define  vxl(n)       valorAmostrar[n]
#else
 #define  vx(n) 			 getValCalculado(n)
 #define  vxl(n)       getValCalculado(n)
#endif

#define  valcont(n) 	 ValCont[n]
#define  dutyold 	 		 duty_alar_ch[NAL]
#define  spcont(n)	   getSetPoint(n)

#define  spalar(n)		 getValParametro(R_AL+n)
#define  res(n) 			 getValParametro(R_Reset+n)
#define  hisalar(n)		 getValParametro(R_HA1+n)
#define  abl(n)        getValParametro(R_H1+n)
#define  kint(n)       getValParametro(R_IN1+n)
#define  mdc(n)        getValParametro(R_Tcontrol1+n) 
#define  kdl(n)        getValParametro(R_DR1+n) 

#define  dutyalar			 duty_alar_ch[NAL]

#ifdef RETsP
#define  VAL_RET(n)       getSetPoint(n)
#elif defined RUVAC
#define  VAL_RET(n)       valLinealizadoMV[n]
#else
#define  VAL_RET(n)       getValCalculado(n)
#endif