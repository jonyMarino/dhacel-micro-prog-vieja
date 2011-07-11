#include "Medicion.h"
#include "Parametros.h"
#include "cnfbox.h"
#include "ADC.h"
#include "Sensores.h"
#include "piddefines.h"
#include "boxescolcal.h"
#include "Humedad.h"
#include "Mmax.h"
#include "tara.h"
#include "hold.h"


#define valnorm(i) (((getValADC(i)+getValParametro(R_ACT+i))*(1000+getValParametro(R_AGT+i)))/1000)*MAXMV/MAXAD  

const int div_dec[]={1,10,100,1000,10000};// divisores del decimal

byte EstatusResult[CANTIDAD_CANALES]={ERR_OK};
int ValoresCalculados[CANTIDAD_CANALES]={0};
long buffer_fil[CANTIDAD_CANALES]={0};

#ifdef RUVAC
int valLinealizadoMV[CANTIDAD_CANALES];
#endif

#ifdef SENS_HUMEDAD
unsigned int Humedad =0;
#endif

/**/
void calculaValorFinal(){
  
  static bool fisrtTime=TRUE;
  byte i;
  int valorLinealizado;
  int vCompTamb;
  long tmpVal;
  
  
  if(fisrtTime==TRUE){                                  //es la primera vez?
    //descarto la primera medicion
    fisrtTime=FALSE;
    return;
  }
  
   
  for(i=0;i<CANTIDAD_CANALES;i++){                                    // recorrer todos los canales
   
    #ifdef RUVAC
    	Linealizar(valnorm(i),SENSOR_MV,&valLinealizadoMV[i]);                               //linealizo mv 
   		valLinealizadoMV[i]=(int)((long)(valLinealizadoMV[i]+getValParametro(R_Offset+i))*getValParametro(R_Gan+i)/1000);          //aplico offset y gan	                                            
    #endif 
   
    vCompTamb =	CompTempAmb((t_sensor)getValParametro((R_Sensor+i)));          
   
    if (SENSOR_Tipo(getValParametro(R_Sensor+i))==TermoCupla)
  /*como la termocupla me entrega los mv de la diferencia(jc-jf)para compensar debo !sumar los mv de jf*/
      EstatusResult[i]=Linealizar((long)(valnorm(i)+(long)vCompTamb),(t_sensor)getValParametro(R_Sensor+i),&valorLinealizado);
    else
      EstatusResult[i]=Linealizar(valnorm(i),(t_sensor)getValParametro(R_Sensor+i),&valorLinealizado);
      
     tmpVal=(long)valorLinealizado;
  
    
     if(SENSOR_Tipo(getValParametro(R_Sensor+i))==TermoResistencia)
        tmpVal = (long)((long)(valorLinealizado+PRom[R_ACP+i])*(1000+PRom[R_AGP+i])/1000);  
   
 
     if(SENSOR_Tipo(getValParametro(R_Sensor+i))!=Lineal)      
        tmpVal/= div_dec[SENSOR_Decimales(getValParametro(R_Sensor+i))-getValParametro(R_Decimales+i)];  
   
     if(tmpVal>9999) 
        EstatusResult[i]=ERR_OF;
	   else if (tmpVal<-1999) 
	      EstatusResult[i]=ERR_UF;
	 
     #ifdef TARA	 
	      puestaCero();
     #endif	 

	   tmpVal=(long)(tmpVal+getValParametro(R_Offset+i))*getValParametro(R_Gan+i)/1000;         //preciso para pasar a long


     ValoresCalculados[i] = filtro ((int)tmpVal,
                                     getValParametro(R_Filtro1+i),
                                     DELTAT/100,
                                     200,
                                     &buffer_fil[i]
                                    );
    }
 

    if (PRom[R_Ver]==VER_DIF)
        ValoresCalculados[0]=ValoresCalculados[0] - ValoresCalculados[1];
				
    #ifdef SENS_HUMEDAD
      if(getValParametro(R_Sensor+1)==SENSOR_HUM){
        Humedad = interpolacionDoble();
        if(Humedad ==ERR_OF)
          EstatusResult[1]=ERR_OF;
      }
    #endif


    #ifdef CCAL 
	    monit_colcal();
    #endif
  
    #ifdef HOLD
      if(getFlagHold())
        ValoresCalculados[0]=getBuffer();
    #endif 

    #ifdef MMAX
      calculoMmax();
      if(getFlagMmax())
        ValoresCalculados[0]=getBufferMmax();
    #endif
 

}


/**/
int getValCalculado(char nroCanal){
  return ValoresCalculados[nroCanal];
}

/**/
void setValCalculado(int val,char nroCanal){
   ValoresCalculados[nroCanal]=val;
}

/**/
byte getEstatusResult(char nroCanal){
  return EstatusResult[nroCanal];
}

/**/
void setEstatusResult(byte val,char nroCanal){
  EstatusResult[nroCanal]=val;
}