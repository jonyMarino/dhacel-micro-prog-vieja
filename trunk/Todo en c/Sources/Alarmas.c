#include "Alarmas.h"
#include "piddefines.h"
#include "paramdefines.h"
#include "Control.h"
#include "Parametros.h"
#include "FuncionVF.h"
#include "SelfTune.h"
#include "PWM.h"
#include "Parametros.h"
#include "Programador.h"
#include "PID.h"
#include "teclas.h"

int duty_alar_ch[CANTIDAD_CANALES]={0};
bool flagAlarmaConet = FALSE;
bool flagAlarmaEoC=FALSE;

void calculosDeAlarma(struct ArgAlarma* arg) {
 /*Este modulo calcula la señal de alarma*/
 
 long vxalar;
 byte flagalar[ALARMAS_CH1];

switch (t_sp_alarma((arg->NroAlarma))){				
    case _rel:														
		    vxalar = cprop(hisalar((arg->NroAlarma)), spcont((arg->NroControl))+spalar((arg->NroAlarma)), 0, vx((arg->NroControl)), 0, 0,res((arg->NroControl)));                  //relativa, los dos sp      
    break;
 
    case _abs:														
		    vxalar = cprop(hisalar((arg->NroAlarma)),spalar((arg->NroAlarma)),0,vx((arg->NroControl)),0,0,res((arg->NroControl)));      												//absoluta solo el sp de al
		break;

    case _refr:																																//refrig, los dos sp con In y Dr
		    vxalar = cprop(hisalar((arg->NroAlarma)),spcont((arg->NroControl))+spalar((arg->NroAlarma)),kint((arg->NroControl)),vx((arg->NroControl)),suminteg[((arg->NroControl))],(int)buffer_deriv[((arg->NroControl))],res((arg->NroControl)));      
    break;
          
    case _ban:														
        if(vx((arg->NroControl)) > spcont((arg->NroControl)))         
            vxalar =cprop(hisalar((arg->NroAlarma)),spcont((arg->NroControl))+spalar((arg->NroAlarma))/2,0,vx((arg->NroControl)),0,0,res((arg->NroControl)));									//banda, arriba como rel
        else
            vxalar =1000 - cprop(hisalar((arg->NroAlarma)),spcont((arg->NroControl))-spalar((arg->NroAlarma))/2,0,vx((arg->NroControl)),0,0,res((arg->NroControl)));						//abajo idem pero polarid inv
    break;



#ifdef programador
    case _seg:							                                                  //segmento							
        if(SegmentoActual[((arg->NroControl))]+OFFSET_SEG1==spalar((arg->NroAlarma)) && PRom[R_Programa]!=NO_PROGRAMA)
            vxalar = 30000;	                                                  //Siempre on
        else
            vxalar = -30000;                                                  //Siempre off
    break;
    
    case _eoc:
        if(isProgFinish(((arg->NroControl))))																								//fin de ciclo
            vxalar = 30000;	 
        else
            vxalar = -30000; 
    break;
#endif
    
#ifdef VF
    case _eoc:
        if(getStatusVF == ENDVF && tecla_d!=1){
            vxalar = 30000;
            flagAlarmaConet=TRUE;
        }else
            vxalar = -30000;     
     break;
#endif     

    default:
		    vxalar = vx((arg->NroControl))-spalar((arg->NroAlarma));
    break;
    };


/*Este modulo limita los valores a int*/

if (vxalar > 30000)
    vxalar = 30000;
if (vxalar < -30000)
    vxalar = -30000;



/*Este modulo calcula el valor del duty*/

 switch (t_sal_alarma((arg->NroAlarma))){				
    case _e:														
        if (hisalar((arg->NroAlarma)) > 0)													                            //exceso, al reves que la señal de cal
            duty_alar_ch[((arg->NroAlarma))] = dtprop   (1000 - vxalar,0,1000);
        else 
            duty_alar_ch[((arg->NroAlarma))] = dtonoff(1000 - vxalar,duty_alar_ch[((arg->NroAlarma))]);
    break;    

    case _d:														
        if (hisalar((arg->NroAlarma)) > 0)																											//defecto
            duty_alar_ch[((arg->NroAlarma))] = dtprop   (vxalar,0,1000);                                     
        else 
            duty_alar_ch[((arg->NroAlarma))] = dtonoff(vxalar,duty_alar_ch[((arg->NroAlarma))]);
    break;

    case _eret:																																//exc con ret
	      if (FlagCleaner==1)																										//si fue pedido limpio flag
	          flagalar[((arg->NroAlarma))] = 0;	 

       if(flagalar[((arg->NroAlarma))]!=1)																										//si no hay flag de ret, calculo
            duty_alar_ch[((arg->NroAlarma))] = dtonoff(1000 - vxalar,duty_alar_ch[((arg->NroAlarma))]);      

       if(duty_alar_ch[((arg->NroAlarma))]==1000)																						//y si llegue a 1000, pongo flag
		        flagalar[((arg->NroAlarma))] = 1;
    break;

    case _dret:																																//def con ret
	      if (FlagCleaner==1)
	          flagalar[((arg->NroAlarma))] = 0;	
	      
       if(flagalar[((arg->NroAlarma))]!=1)
            duty_alar_ch[((arg->NroAlarma))] = dtonoff(vxalar,duty_alar_ch[((arg->NroAlarma))]);
       
       if(duty_alar_ch[((arg->NroAlarma))]==1000)
		        flagalar[((arg->NroAlarma))] = 1;
    break;

    case _dbloq:																															//bloqueada
            duty_alar_ch[((arg->NroAlarma))] = dtonoff(vxalar,duty_alar_ch[((arg->NroAlarma))]);						//calculo

       if(duty_alar_ch[((arg->NroAlarma))]==0)
    		    flagalar[((arg->NroAlarma))] = 1;
       
       if(flagalar[((arg->NroAlarma))] == 0)
            duty_alar_ch[((arg->NroAlarma))]=0;
    break;

    case _RET:
      
       duty_alar_ch[((arg->NroAlarma))]=((long)(VAL_RET((arg->NroControl)) - getValParametro(R_AAnlLow+((arg->NroAlarma)))))*1000/(getValParametro(R_AAnlHi+((arg->NroAlarma)))-getValParametro(R_AAnlLow+((arg->NroAlarma)))); 
			 
			 if(duty_alar_ch[((arg->NroAlarma))]>1000)
			      duty_alar_ch[((arg->NroAlarma))]=1000;
			 else if(duty_alar_ch[((arg->NroAlarma))]<0)
			      duty_alar_ch[((arg->NroAlarma))]=0;
			 break;
			 
    default:
			 duty_alar_ch[((arg->NroAlarma))] = 0;    //Alarma en "NO"
       break; 
 };

#ifdef VF
  if(t_sp_alarma((arg->NroAlarma))==_eoc){
//Calculo valor de la salida de alarma
  PWM_SetRatio16(duty_alar_ch[((arg->NroAlarma))],(arg->salida));  

	if(PRom[R_Talar1+((arg->NroAlarma))]==_RET || hisalar((arg->NroAlarma))<=0)
     setPWM_period(PWM_Anl,(arg->salida));		
  else setPWM_period(PRom[R_Per+((arg->NroControl))],(arg->salida));
 
  }else if(getStatusVF == ENDVF)  //si estoy en "end" y no es EoC desconecto la alarma
    PWM_SetRatio16(0,(arg->salida));
  else{
   PWM_SetRatio16(duty_alar_ch[((arg->NroAlarma))],(arg->salida));  

	if(PRom[R_Talar1+((arg->NroAlarma))]==_RET || hisalar((arg->NroAlarma))<=0)
     setPWM_period(PWM_Anl,(arg->salida));		
  else setPWM_period(PRom[R_Per+((arg->NroControl))],(arg->salida)); 
  }

#else
   PWM_SetRatio16(duty_alar_ch[((arg->NroAlarma))],(arg->salida));  

	if(PRom[R_Talar1+((arg->NroAlarma))]==_RET || hisalar((arg->NroAlarma))<=0)
     setPWM_period(PWM_Anl,(arg->salida));		
  else setPWM_period(getValParametro(R_Per+((arg->NroControl))),(arg->salida));
#endif

}