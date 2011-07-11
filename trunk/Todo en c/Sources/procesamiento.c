/*IMPLEMENTATION MODULE procesamiento; */
#include "Mydefines.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "procesamiento.h"

#define PID 2
#define ONOFF 0
#define REF 1
int salidacontrol (int sp, int vx, int res)//, int deriv, int integr, int ab, int his, int potmax, int potmin, int mod_cont, int mod_sal, byte salida)

{
//        int duty;
        int contval;
                        
        contval = vx+res+sp;							          //el control es el valor mas el reset
/*        if(mod_cont=PID)													//en PID agrego int y der
          contval= contval+deriv+integr;
          
				if(contval>=(sp + ab/2))
				  duty = 0;
				else if(contval<=(sp - ab/2))  
					duty = 10000;
				else if(mod_cont != ONOFF){
				  duty = 10000 * (sp - contval + ab/2) / ab;
				  if(mod_sal = REF)
				    duty = 10000 - duty;
				  if(duty>=potmax)
				    duty=potmax;
				  else if(duty<=potmin)
				    duty= potmin;
				}
				 PWM_SetRatio16(duty,salida);
	*/			
        return;

}
