#include "valorControl.h"
#include "Medicion.h"
#include "Parametros.h"
#include "cnfbox.h"
#include "In1.h"
#include "FuncionVF.h" 

int SetPoint[CANTIDAD_SAL_CONTROL];                        // Set Point en Ram que se utiliza para el control y los calculos



void setSetPoint(){
   byte i;
   
  for (i=0;i<CANTIDAD_SAL_CONTROL;i++){
       #ifdef programador
         if(getValParametro(R_Programa+i)!=NO_PROGRAMA) 
            SetPoint[i] = get_SP_Programa(i);
         else SetPoint[i] = getValParametro(R_SetPoint+i); 
       #else
         SetPoint[i] = getValParametro(R_SetPoint+i);
       #endif
       
     }
     
     
     #ifdef jony_13_07
     #ifdef SP_EXTERNO
     SetPoint[0]+=getValCalculado(1);
     #endif
     #endif

     #ifndef SP_EXTERNO
		 #ifndef programador
		   SetPoint[0] = getValParametro(R_SetPoint+0);  
		 #endif
		 #endif
     
     #ifdef CCAL

			if(flagsst == _STRT)																		                                          	// primero veo inicio y pongo sp
        SetPoint[0] =Lim_Inf_ch[0];														                                          	// bien abajo para que no prenda nunca
			else if(flagsst == _SST)																	                                          // primero veo sst y pongo sp
        SetPoint[0] = getValParametro(R_SPlim);
      else if(PRom[R_Stn]==Stc)																                                            // segundo veo autosint
        SetPoint[0] = (int)((long)getValParametro(R_SetPoint)*getValParametro(R_SPlimst)/1000);
      else if(!In1_GetVal())																	                                           	// tercero veo sp externo
        SetPoint[0] = getValParametro(R_SPsb); 
      else		  																							                                          	// y si nada, el sp normal
				SetPoint[0] = getValParametro(R_SetPoint);

		 #endif
		 
		 
		 
		 #ifdef VF
		    SetPoint[0] = tempActVF;
		 #endif
		 
		 #ifdef DOBLE_SP
		  if(In1_GetVal()) 
        SetPoint[0] = getValParametro(R_SetPoint+0);
		  else
		    SetPoint[0] = getValParametro(R_SetPoint+1);
		 #endif
		 		

}

int getSetPoint(char nroCanal){
  return SetPoint[nroCanal];
}