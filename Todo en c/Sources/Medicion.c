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
void calculaValorFinal(byte NroChanel){
  
  static bool fisrtTime=TRUE;
 
  int valorLinealizado;
  int vCompTamb;
  long tmpVal;
  
  
  if(fisrtTime==TRUE){                                  //es la primera vez?
    //descarto la primera medicion
    fisrtTime=FALSE;
    return;
  }
  
  
    #ifdef RUVAC
    	Linealizar(valnorm(NroChanel),SENSOR_MV,&valLinealizadoMV[NroChanel]);                               //linealizo mv 
   		valLinealizadoMV[NroChanel]=(int)((long)(valLinealizadoMV[NroChanel]+getValParametro(R_Offset+i))*getValParametro(R_Gan+i)/1000);          //aplico offset y gan	                                            
    #endif 
   
    vCompTamb =	CompTempAmb((t_sensor)getValParametro((R_Sensor+NroChanel)));          
   
    if (SENSOR_Tipo(getValParametro(R_Sensor+NroChanel))==TermoCupla)
  /*como la termocupla me entrega los mv de la diferencia(jc-jf)para compensar debo !sumar los mv de jf*/
      EstatusResult[NroChanel]=Linealizar((long)(valnorm(NroChanel)+(long)vCompTamb),(t_sensor)getValParametro(R_Sensor+NroChanel),&valorLinealizado);
    else
      EstatusResult[NroChanel]=Linealizar(valnorm(NroChanel),(t_sensor)getValParametro(R_Sensor+NroChanel),&valorLinealizado);
      
     tmpVal=(long)valorLinealizado;
  
    
     if(SENSOR_Tipo(getValParametro(R_Sensor+NroChanel))==TermoResistencia)
        tmpVal = (long)((long)(valorLinealizado+PRom[R_ACP+NroChanel])*(1000+PRom[R_AGP+NroChanel])/1000);  
   
 
     if(SENSOR_Tipo(getValParametro(R_Sensor+NroChanel))!=Lineal)      
        tmpVal/= div_dec[SENSOR_Decimales(getValParametro(R_Sensor+NroChanel))-getValParametro(R_Decimales+NroChanel)];  
   
     if(tmpVal>9999) 
        EstatusResult[NroChanel]=ERR_OF;
	   else if (tmpVal<-1999) 
	      EstatusResult[NroChanel]=ERR_UF;
	 
     #ifdef TARA	 
	      puestaCero();
     #endif	 

	   tmpVal=(long)(tmpVal+getValParametro(R_Offset+NroChanel))*getValParametro(R_Gan+NroChanel)/1000;         //preciso para pasar a long


     ValoresCalculados[NroChanel] = filtro ((int)tmpVal,
                                     getValParametro(R_Filtro1+NroChanel),
                                     DELTAT/100,
                                     200,
                                     &buffer_fil[NroChanel]
                                    );

 

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