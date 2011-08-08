#include "PID.h"
#include "piddefines.h"
#include "paramdefines.h"
#include "Control.h"
#include "Parametros.h"
#include "FuncionVF.h"
#include "SelfTune.h"
#include "PWM.h"
#include "Parametros.h"

int ValCont [CANTIDAD_CANALES]={0};         //valor que controla el equipo con PID
long suminteg[CANTIDAD_CANALES]={0};
long buffer_deriv[CANTIDAD_CANALES]={0};
int deriv[CANTIDAD_CANALES]={0};
int duty_cont_ch[CANTIDAD_CANALES]={0};

int dutytmp; 

#ifdef VPROP
int duty_vprop;
long cnt_duty_vprop = 0;
#endif

void calculosDeControl(struct ArgControl* arg){
  int Dr;
  long auxiliar;
  
  
if (getValParametro(R_Stn+(arg->NroControl)) != Stc){                                            //primero veo stune

//calculo la integral

 integral(abl((arg->NroControl)),hisalar((arg->NroAlarma)),spcont((arg->NroControl)),spalar((arg->NroAlarma)),kint((arg->NroControl)),vxl((arg->NroControl)),&suminteg[(arg->NroControl)],1);

//la derivada

 Dr = derivada   (vxl((arg->NroControl)),kdl((arg->NroControl)),mdc((arg->NroControl)),abl((arg->NroControl)),&buffer_deriv[(arg->NroControl)],&deriv[(arg->NroControl)]);

//el valor final de control. Es long

 auxiliar =  cprop(abl((arg->NroControl)),spcont((arg->NroControl)),kint((arg->NroControl)),vx((arg->NroControl)),suminteg[(arg->NroControl)],Dr,res((arg->NroControl)));      

//ajusto al rango de long a int
				
 if (auxiliar > 30000)
    valcont((arg->NroControl)) = 30000;
 else if (auxiliar < -30000)
    valcont((arg->NroControl)) = -30000;
 else
    valcont((arg->NroControl)) = (int)auxiliar;
 
//Calculo del duty

 if (abl((arg->NroControl)) >0 )

 #ifdef CCAL
	 if(flagsst == _SST)
    duty_cont_ch[((arg->NroControl))] = dtprop(valcont((arg->NroControl)),getValParametro(R_Pot_Inf+((arg->NroControl))),getValParametro(R_Pss));
	 else
    duty_cont_ch[((arg->NroControl))] = dtprop(valcont((arg->NroControl)),getValParametro(R_Pot_Inf+((arg->NroControl))),getValParametro(R_Pot_Sup+((arg->NroControl))));
 #else
    duty_cont_ch[((arg->NroControl))] = dtprop(valcont((arg->NroControl)),getValParametro(R_Pot_Inf+((arg->NroControl))),getValParametro(R_Pot_Sup+((arg->NroControl))));
 #endif

 else 
    duty_cont_ch[((arg->NroControl))] = dtonoff(valcont((arg->NroControl)),duty_cont_ch[((arg->NroControl))]);
 
//Valor de la salida
#ifdef RPM
 if (getValParametro(R_Tsalcont+(arg->NroControl)) == _inv)
#else 
 if (getValParametro(R_Tsalcont+(arg->NroControl)) == _ref)
#endif 
    dutytmp = 1000-duty_cont_ch[(arg->NroControl)];
 else if (getValParametro(R_Tsalcont+(arg->NroControl)) == _man)
    dutytmp = getValParametro(R_Potman+(arg->NroControl));
 else dutytmp = duty_cont_ch[(arg->NroControl)];
      
 #ifdef VPROP
 
  duty_vprop = dutytmp;
 
 #else
 
 #ifdef VF
 if(getModeVF == ENDVF)
   PWM_SetRatio16(0,(arg->salida));       //si esta en "fin" desconecta la salida de control
 else
   PWM_SetRatio16(dutytmp,(arg->salida)); //de lo contrario funciona segun el sp
 #else
 PWM_SetRatio16(dutytmp,(arg->salida));
 #endif

  if(abl((arg->NroControl))<=0 && getValParametro(R_Tsalcont+((arg->NroControl)))!=_man)
    setPWM_period(PWM_Anl,(arg->salida));	
 else setPWM_period(getValParametro(R_Per+((arg->NroControl))),(arg->salida));
 
 #endif 
 
 }else SelfTuneControl((arg->NroControl),spcont((arg->NroControl)));
	

}