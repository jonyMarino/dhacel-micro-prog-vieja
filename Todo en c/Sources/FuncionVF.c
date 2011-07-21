#include "Mydefines.h"
#include "boxes.h" 
#include "boxesset.h"
//#include "Timer.h" 
#include "FuncionVF.h" 
#include "bkr.h"  
#include "O2.h"
//#include "presentaValor.hpp"
//#include "EPM203Manejador.hpp"
#include "vfboxes.h"
#include "Parametros.h"
#include "RlxMTimer.h"
#include "Method.h"
#include "MethodContainer.h"

#define ENDVF 0
#define RUNVF  1
#define RMPVF 0
#define MSTVF 1
  
extern word minutos;
extern long rampa_mestaTime;

#ifdef _PRINTER
//extern ManejadorImpresionPersistente mi;
//extern EPM203Manejador os;
#endif
long tempActVF;
char puntoDecimalVF;
char contProtecRuido;   
        
char VFmod;
char VFstatus; 
                      
char EtapaCorr=0;
char CantEtapas=0;
char Kdec= 0;

bool fistTime = TRUE;
bool fistTimeInicio = TRUE;
bool fistTimeFin = TRUE;
bool spIsMayorMeseta = FALSE;

dword tmp;

struct RlxMTimer * _rlxtimer;
struct Method* _method;


//PresentaValor cartelInicio(NULL,"Comienzo de Ciclo");
//PresentaValor cartelFin(NULL,"Fin de Ciclo");

#ifdef VF  

#ifdef GOROSITO
#define Unidad     60
#else
#define Unidad     1
#endif 

#define Te_MES_ANT  DatosVF[nroProgEnAccion-1][EtapaCorr-2].tem                              //temp meseta etapa anterior  
#define Te_MES     DatosVF[nroProgEnAccion-1][EtapaCorr-1].tem                               //temp meseta etapa actual
#define Ve_RMP     DatosVF[nroProgEnAccion-1][EtapaCorr-1].vel     
#define Ti_MES     DatosVF[nroProgEnAccion-1][EtapaCorr-1].tiem   
#define MaxMin     10                                                     //fija los limites de sobrepaso de la temperatura de meseta
#define PROTEC     2                                                      //cantidad de veces que compruebo la condicion de fin de rampa


void intVF(){
  _rlxtimer=RlxMTimer_Init(1000,calculoTempInstYtiempoMesetaVF,NULL);
  _method=initMethod(ProcesoTeclasVF,NULL);
  MethodContainer_add(_Method,ListaPrincipal);

}



void calculoTempInstYtiempoMesetaVF (void){     
   static int correc=0;
   static int tempMestAux=0;
   char EtapaCorrAux=0;
 
   if(EtapaCorr==0)
    VFstatus = ENDVF;

 /* calculo de la temperatura de la rampa cada 1 segundos y             */
/* calculo del tiempo transcurrido una ves llegado a la temp de meseta */  
 puntoDecimalVF = PRom[R_Decimales];

  if(VFstatus == RUNVF){                                                  //estoy corriendo?
     if(fistTimeInicio){
      #ifdef _PRINTER
       //if(mi.getHabilitado()!=1)   
         // mi.setHabilitado(1);
      // cartelInicio.imprimirValor(os);
       #endif
       fistTimeInicio=FALSE;
       fistTimeFin=TRUE;
       
     }
 /*/////////////////calculo del setpoint(tmpAct)/////////////// */
  
   if(VFmod==RMPVF) {                                                     //si, es rampa?
    
      if(puntoDecimalVF == 0)                                             //si, pongo decimales
        Kdec = 10;
      else
        Kdec = 1;
      
      
/*/////////////////////comprobacion de inicio del ciclo////////////////////*/

      if(fistTime){
        fistTime=FALSE;
        /*busco en que rampa debo arrancar segun el valor medido*/
        for(EtapaCorr=1;EtapaCorr<=CantEtapas;EtapaCorr++){
          if(getValCalculado(0)<Te_MES){
            //calculo el tiempo que tendria que haber transcurido
            if(EtapaCorr==1){
              correc = getValCalculado(0);
              /*tmp=((dword)ValFinal[0]*Kdec*Unidad*60);
              rampa_mestaTime=tmp/(unsigned int)Ve_RMP;*/
            }else{
              correc = getValCalculado(0)-Te_MES_ANT;
              /*tmp=((dword)(ValFinal[0]-Te_MES_ANT)*Kdec*Unidad*60);
              rampa_mestaTime=tmp/(unsigned int)Ve_RMP;*/
            }
          
            break;
          }
        }
        /*la temp medida es mayor a todas las mesetas,
        */
        if(EtapaCorr>CantEtapas){ 
         //busco la meseta mas alta para arrancar
         EtapaCorr=1; 
         tempMestAux=Te_MES;
         for(EtapaCorr=1;EtapaCorr<=CantEtapas;EtapaCorr++){
            if(tempMestAux<Te_MES){
              EtapaCorrAux=EtapaCorr;
              tempMestAux=Te_MES; 
            }
         }
          #ifdef VF_BKR
          EtapaCorr =1;
          #else
          EtapaCorr = EtapaCorrAux;
          #endif
          tempActVF = tempMestAux;
          
          rampa_mestaTime=0;
          correc=0;
          spIsMayorMeseta = FALSE;
          VFmod=MSTVF; 
          return; 
        }
      }
       
      
       
      if(EtapaCorr==1){    //estoy en el inicio del ciclo(primera etapa), la temp de la meseta anteriro es cero!
        
         tempActVF = (long)(correc + (long)(Ve_RMP*rampa_mestaTime)/(unsigned int)(60 * Kdec * Unidad));    //si, calculo sp
          
 /*///////////////control de fin de rampa//////////////////////// */
        if((long)getValCalculado(0)>=(Te_MES)){                                           //verifico la condicion contProtecRuido veces
           contProtecRuido++;
           
           if(contProtecRuido==PROTEC)
              contProtecRuido=0;
            else
              return;
            
            tempActVF = Te_MES; 
            rampa_mestaTime=0;
            correc=0;
            spIsMayorMeseta = FALSE;
            VFmod=MSTVF;                                                     //comienzo de meseta
            //ResetScroll();
            return;
        }
          
        //verifico la condicion para el limite max
        if(tempActVF>=(Te_MES+MaxMin)){     //me pase?
          tempActVF=Te_MES+MaxMin;                                   //limito
           spIsMayorMeseta=TRUE;
        }else
          return;         //retorno por que no llegue al limite max
        
       }else{
        
        //nro de etapa distinta a la inicial(1)
       
         if(Te_MES > Te_MES_ANT){                                           //pendiente positiva? 
         
          tempActVF = (long)(correc + Te_MES_ANT + (long)(Ve_RMP*rampa_mestaTime)/(unsigned int)(60 * Kdec * Unidad));    //si, calculo sp
          
 /*///////////////control de fin de rampa//////////////////////// */
          if((long)getValCalculado(0)>=(Te_MES)){                                           //verifico la condicion contProtecRuido veces
           contProtecRuido++;
           
           if(contProtecRuido==PROTEC)
              contProtecRuido=0;
            else
              return;
            
            tempActVF = Te_MES; 
            rampa_mestaTime=0;
            correc=0;
            spIsMayorMeseta = FALSE;
            VFmod=MSTVF;                                                     //comienzo de meseta
            //ResetScroll();
            return;
        }
          
        //verifico la condicion para el limite max
        if(tempActVF>=(Te_MES+MaxMin)){                             //me pase?
          tempActVF=Te_MES+MaxMin;                                           //limito
          spIsMayorMeseta = TRUE;
        }
        else
          return;         //retorno por que no llegue al limite max
        
       }
       
       if(Te_MES < Te_MES_ANT){                                               //pendiente negativa?
        tempActVF = (long)(Te_MES_ANT - (long)(Ve_RMP*rampa_mestaTime)/(unsigned int)(60 * Kdec * Unidad));
        /*/////////control de fin de rampa/////////////////// */
        if((long)getValCalculado(0)<=(Te_MES)){
           contProtecRuido++;
           
           if(contProtecRuido==PROTEC)
              contProtecRuido=0;
            else
              return;
            
            tempActVF = Te_MES; 
            rampa_mestaTime=0;
            spIsMayorMeseta = FALSE;
            VFmod=MSTVF;                                                      //comienzo de meseta
            //ResetScroll();
            return;
        }
          
        //verifico la condicion para el limite min
        if(tempActVF<=(Te_MES-MaxMin)){ 
          tempActVF=Te_MES-MaxMin;
          spIsMayorMeseta = TRUE;
        }
        else
          return;                                                             //retorno por que no llegue al limite max
        
       }
       }
       
       if(Te_MES == Te_MES_ANT){                                               //iguales?
          tempActVF =Te_MES;
          rampa_mestaTime=0;
          contProtecRuido=0;
          VFmod=MSTVF;                                                         //comienzo de meseta
          //ResetScroll();
          return;     
       }
      
  }
   
/*/////////////control de fin de meseta//////////////////////////*/  
  
  else{   // estoy en la meseta , cargo el valor de la temp de meseta por si se modifica
    
   tempActVF = Te_MES;
  
  if((Ti_MES*60) <= rampa_mestaTime){                                    //termino la meseta? 
    rampa_mestaTime = 0;
    minutos=1;
    EtapaCorr++;
    if((EtapaCorr-1)<CantEtapas){                                              //supere la cantidad de etapas?
      VFmod=RMPVF;      //INICIA RAMPA
      //ResetScroll();
      return;
    }else {                                                                     // si supere -> fin
      tempActVF =0; 
      fistTime = TRUE;
      VFstatus = ENDVF;
      #ifdef VF_PROG
      nroProgEnAccion=0;
      #endif
      //ResetScroll(); 
    }
  }

  }
}else{                                                                           //si no era run era stop  
  rampa_mestaTime = 0;                                                           // reseteo todo
  contProtecRuido=0;
  fistTime = TRUE;
  VFmod=RMPVF;
  tempActVF=0;
  if(fistTimeFin){
    #ifdef _PRINTER
    //if(mi.getHabilitado()!=0)
     //mi.setHabilitado(0); 
    //cartelFin.imprimirValor(os);
    #endif
    fistTimeInicio=TRUE;
    fistTimeFin=FALSE;
    
   }
 // ResetScroll();      
}
}

#endif

   
