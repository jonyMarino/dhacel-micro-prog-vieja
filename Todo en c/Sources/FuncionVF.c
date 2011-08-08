#include "Mydefines.h"
#include "boxes.h" 
#include "boxesset.h" 
#include "FuncionVF.h" 
#include "O2.h"
#include "presentarValor.h"
#include "vfboxes.h"
#include "Parametros.h"
#include "RlxMTimer.h"
#include "Method.h"
#include "MethodContainer.h"

 
struct RlxMTimer * _rlxtimer;

dataVF datosVF;

struct ArgPresentaValor cartelInicio={
 NULL,
 "Comienzo de Ciclo" 
};

struct ArgPresentaValor cartelFin={
 NULL,
 "Fin de Ciclo" 
};


#ifdef VF  


void intVF(){
  datosVF.fistTime=TRUE;
  getCantidadEtapasVF=1;
  
  #ifdef VF_PROG
  getNProgVF=0;
  #else
  getNProgVF=1;
  #endif
  
  _rlxtimer=RlxMTimer_Init(1000,calculoTempInstYtiempoMesetaVF,NULL);
 
}



void calculoTempInstYtiempoMesetaVF (void){     
   static int correc=0;
   static char contProtecRuido=0;
   static bool fistTimeInicio = TRUE;
   static bool fistTimeFin = TRUE;
   static bool spIsMayorMeseta = FALSE;  
   static int tempMestAux=0;
   char EtapaCorrAux=0;
   char Kdec;
 
   if(getNEtapaActualVF==0)
    getModeVF = ENDVF;

 /* calculo de la temperatura de la rampa cada 1 segundos y             */
/* calculo del tiempo transcurrido una ves llegado a la temp de meseta */  

  if(getModeVF == RUNVF){                                                  //estoy corriendo?
     getTimeVF++;
     if(fistTimeInicio){
      #ifdef _PRINTER
       if(getHabilitado()!=1)   
          setHabilitado(1);
       imprimirValor(&cartelInicio);
       #endif
       fistTimeInicio=FALSE;
       fistTimeFin=TRUE;
       
     }
 /*/////////////////calculo del setpoint(tmpAct)/////////////// */
  
   if(getStatusVF==RMPVF) {                                                     //si, es rampa?
    
      if(PRom[R_Decimales] == 0)                                             //si, pongo decimales
        Kdec = 10;
      else
        Kdec = 1;
      
      
/*/////////////////////comprobacion de inicio del ciclo////////////////////*/

      if(datosVF.fistTime){
        datosVF.fistTime=FALSE;
        /*busco en que rampa debo arrancar segun el valor medido*/
        for(getNEtapaActualVF=1;getNEtapaActualVF<=getCantidadEtapasVF;getNEtapaActualVF++){
         
          if(getValCalculado(0)<Te_MES){
            //calculo el tiempo que tendria que haber transcurido
            if(getNEtapaActualVF==1){
              correc = getValCalculado(0);
            }else{
              correc = getValCalculado(0)-Te_MES_ANT;
            }
          
            break;
          }
        }
        /*la temp medida es mayor a todas las mesetas,
        */
        if(getNEtapaActualVF>getCantidadEtapasVF){ 
         //busco la meseta mas alta para arrancar
         getNEtapaActualVF=1; 
         tempMestAux=Te_MES;
         for(getNEtapaActualVF=1;getNEtapaActualVF<=getCantidadEtapasVF;getNEtapaActualVF++){
            if(tempMestAux<Te_MES){
              EtapaCorrAux=getNEtapaActualVF;
              tempMestAux=Te_MES; 
            }
         }
          #ifdef VF_BKR
          getNEtapaActualVF =1;
          #else
          getNEtapaActualVF = EtapaCorrAux;
          #endif
          getSpVF = tempMestAux;
          
          getTimeVF=0;
          correc=0;
          spIsMayorMeseta = FALSE;
          getStatusVF=MSTVF; 
          return; 
        }
      }
       
      
       
      if(getNEtapaActualVF==1){    //estoy en el inicio del ciclo(primera etapa), la temp de la meseta anteriro es cero!
        
         getSpVF = (long)(correc + (long)(Ve_RMP*getTimeVF)/(unsigned int)(60 * Kdec * Unidad));    //si, calculo sp
          
 /*///////////////control de fin de rampa//////////////////////// */
        if((long)getValCalculado(0)>=(Te_MES)){                                           //verifico la condicion contProtecRuido veces
           contProtecRuido++;
           
           if(contProtecRuido==PROTEC)
              contProtecRuido=0;
            else
              return;
            
            getSpVF = Te_MES; 
            getTimeVF=0;
            correc=0;
            spIsMayorMeseta = FALSE;
            getStatusVF=MSTVF;                                                     //comienzo de meseta
           
            return;
        }
          
        //verifico la condicion para el limite max
        if(getSpVF>=(Te_MES+MaxMin)){     //me pase?
          getSpVF=Te_MES+MaxMin;                                   //limito
           spIsMayorMeseta=TRUE;
        }else
          return;         //retorno por que no llegue al limite max
        
       }else{
        
        //nro de etapa distinta a la inicial(1)
       
         if(Te_MES > Te_MES_ANT){                                           //pendiente positiva? 
         
          getSpVF = (long)(correc + Te_MES_ANT + (long)(Ve_RMP*getTimeVF)/(unsigned int)(60 * Kdec * Unidad));    //si, calculo sp
          
 /*///////////////control de fin de rampa//////////////////////// */
          if((long)getValCalculado(0)>=(Te_MES)){                                           //verifico la condicion contProtecRuido veces
           contProtecRuido++;
           
           if(contProtecRuido==PROTEC)
              contProtecRuido=0;
            else
              return;
            
            getSpVF = Te_MES; 
            getTimeVF=0;
            correc=0;
            spIsMayorMeseta = FALSE;
            getStatusVF=MSTVF;                                                     //comienzo de meseta
            
            return;
        }
          
        //verifico la condicion para el limite max
        if(getSpVF>=(Te_MES+MaxMin)){                             //me pase?
          getSpVF=Te_MES+MaxMin;                                           //limito
          spIsMayorMeseta = TRUE;
        }
        else
          return;         //retorno por que no llegue al limite max
        
       }
       
       if(Te_MES < Te_MES_ANT){                                               //pendiente negativa?
        getSpVF = (long)(Te_MES_ANT - (long)(Ve_RMP*getTimeVF)/(unsigned int)(60 * Kdec * Unidad));
        /*/////////control de fin de rampa/////////////////// */
        if((long)getValCalculado(0)<=(Te_MES)){
           contProtecRuido++;
           
           if(contProtecRuido==PROTEC)
              contProtecRuido=0;
            else
              return;
            
            getSpVF = Te_MES; 
            getTimeVF=0;
            spIsMayorMeseta = FALSE;
            getStatusVF=MSTVF;                                                      //comienzo de meseta
            
            return;
        }
          
        //verifico la condicion para el limite min
        if(getSpVF<=(Te_MES-MaxMin)){ 
          getSpVF=Te_MES-MaxMin;
          spIsMayorMeseta = TRUE;
        }
        else
          return;                                                             //retorno por que no llegue al limite max
        
       }
       }
       
       if(Te_MES == Te_MES_ANT){                                               //iguales?
          getSpVF =Te_MES;
          getTimeVF=0;
          contProtecRuido=0;
          getStatusVF=MSTVF;                                                         //comienzo de meseta
          
          return;     
       }
      
  }
   
/*/////////////control de fin de meseta//////////////////////////*/  
  
  else{   // estoy en la meseta , cargo el valor de la temp de meseta por si se modifica
    
   getSpVF = Te_MES;
  
  if((Ti_MES*60) <= getTimeVF){                                    //termino la meseta? 
    getTimeVF = 0;
    minutos=1;
    getNEtapaActualVF++;
    if((getNEtapaActualVF-1)<getCantidadEtapasVF){                                              //supere la cantidad de etapas?
      getStatusVF=RMPVF;      //INICIA RAMPA
      
      return;
    }else {                                                                     // si supere -> fin
      getSpVF =0; 
      datosVF.fistTime = TRUE;
      getModeVF = ENDVF;
      
      #ifdef VF_PROG
      getNProgVF=0;
      #endif
      
    }
  }

  }
}else{                                                                           //si no era run era stop  
  getTimeVF = 0;                                                           // reseteo todo
  contProtecRuido=0;
  datosVF.fistTime = TRUE;
  getStatusVF=RMPVF;
  getSpVF=0;
  if(fistTimeFin){
    #ifdef _PRINTER
    if(mi.getHabilitado()!=0)
     setHabilitado(0); 
    imprimirValor(&cartelFin);;
    #endif
    fistTimeInicio=TRUE;
    fistTimeFin=FALSE;
    
   }
 
}
}

#endif

   
