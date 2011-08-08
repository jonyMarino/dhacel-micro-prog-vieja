#include "Mydefines.h"
#include "str_lib.h"
#include "System.h"
#include "hidef.h"
#include "boxes.h"
#include "Wdog1.h"
#include "Sensores.h"
#include "adq.h"
#include "Parametros.h"
#include "RlxMTimer.h"
#include "Method.h"
#include "MethodContainer.h"

#ifdef ADQ

#define POSICIONenFLASH(i)   (FLASH_ADQUISIDOR_START+i)
#define DATOenFLASH(i)       (*(int*)(FLASH_ADQUISIDOR_START+(2*i)))
#define FECHAenFLASH(i)      (*(word*)(FLASH_ADQUISIDOR_START+(2*i)))

#define ESTADO_ILDE     0
#define ESTADO_SI       1


int stateAdq=ERR_OK;
char flagFechaHora=ESTADO_SI;
long indexGlobal;
struct RlxMTimer * _Rlxtimer;
struct Method* _Method;

#if CANT_VALORES == 2  && CANTIDAD_CANALES == 2
datoAdq valores[MAX_CANTIDAD_VALORES]={ 
    {(int*)&ValoresCalculados[0],0},
    {(int*)&ValoresCalculados[1],1},
  };
#elif CANT_VALORES == 2
datoAdq valores[MAX_CANTIDAD_VALORES]={ 
    {(int*)&ValoresCalculados[0],0},
    {(int*)&SetPoint[0],0},
  };
#else
datoAdq valores[MAX_CANTIDAD_VALORES]={ 
    {(int*)&ValoresCalculados[0],0},
};
#endif  


/*Esta funcion permite un valor y elejir la posicion-
 actualmente no se utiliza
*/

/*byte adquirirValor(word index,int val){
  
  if(PRom[R_AdqHabilitado]==ADQ_SI) 
   return WriteWord(FLASH_ADQUISIDOR_START+index,val);
 
  return ADQ_NO;
} */

/*******************************************************************************************/
/*Esta funcion guarda la fecha y la hora en la flash dejando un espacio en blanco
*/
/*int guardarFechaYhora(word *anio,word *mes,word *dia,word *hora,word *min,word *seg){
  
 DATEREC dateAdq;
 TIMEREC time;    
 int error;
  
 if((indexGlobal=buscarPosicionVacia())==ADQ_FULL)
    return ADQ_FULL;
 
  //tomo la fecha y la hora dle reloj de tiempo real
  getFecha(&dateAdq);
  getTiempo(&time);
  
  *anio=(word)dateAdq.Year;
  *mes=(word)dateAdq.Month;
  *dia=(word)dateAdq.Day;
  *hora=(word)time.Hour;
  *min=(word)time.Min;
  *seg=(word)time.Sec;
  
  indexGlobal+=2;   // dejo un word en blanco para utilizarlo como señalizacion
    //guardo fecha y hora
  error=(int)WriteWord(POSICIONenFLASH(indexGlobal),(word)dateAdq.Day); 
  indexGlobal+=2;
  error=(int)WriteWord(POSICIONenFLASH(indexGlobal),(word)dateAdq.Month);
  indexGlobal+=2;
  error=(int)WriteWord(POSICIONenFLASH(indexGlobal),(word)dateAdq.Year); 
  indexGlobal+=2;
  error=(int)WriteWord(POSICIONenFLASH(indexGlobal),(word)time.Hour);
  indexGlobal+=2;
  error=(int)WriteWord(POSICIONenFLASH(indexGlobal),(word)time.Min);
  indexGlobal+=2;
  error=(int)WriteWord(POSICIONenFLASH(indexGlobal),(word)time.Sec);
    
  
  return error;
}    */


void intAdqFlash(){
  
  _Rlxtimer=RlxMTimer_Init(PRom[R_AdqPeriodo],ADDAdqFlash,NULL);
  _Method=initMethod(TranferToPC,NULL);
  MethodContainer_add(_Method,ListaPrincipal);

}


void ADDAdqFlash(){
  if(PRom[R_AdqHabilitado] && stateAdq!=ADQ_FULL)     
    stateAdq=adquirirValorAut(valores,CANT_VALORES);
       
}

void TranferToPC(){
  if(datoSerie == 't' || PRom[R_AdqTranfer]==1){
    if(PRom[R_AdqTranfer]==1)
      EscribirWord((word)&PRom[R_AdqTranfer],0);  //pongo en no
    datoSerie=0;
    bajarTodosDatosAdq(valores,CANT_VALORES);
  }
}


void SetTimeAdq(unsigned int time){
  RlxTimer_setTime(_Rlxtimer, time);
}

int guardarFechaYhora(){
  
 DATEREC dateAdq;
 TIMEREC time;    
 int error;
  
 if((indexGlobal=buscarPosicionVacia())==ADQ_FULL)
    return ADQ_FULL;
 
  //tomo la fecha y la hora dle reloj de tiempo real
  getFecha(&dateAdq);
  getTiempo(&time);
  
  indexGlobal+=2;   // dejo un word en blanco para utilizarlo como señalizacion
    //guardo fecha y hora
  error=(int)WriteWord(POSICIONenFLASH(indexGlobal),(word)dateAdq.Day); 
  indexGlobal+=2;
  error=(int)WriteWord(POSICIONenFLASH(indexGlobal),(word)dateAdq.Month);
  indexGlobal+=2;
  error=(int)WriteWord(POSICIONenFLASH(indexGlobal),(word)dateAdq.Year); 
  indexGlobal+=2;
  error=(int)WriteWord(POSICIONenFLASH(indexGlobal),(word)time.Hour);
  indexGlobal+=2;
  error=(int)WriteWord(POSICIONenFLASH(indexGlobal),(word)time.Min);
  indexGlobal+=2;
  error=(int)WriteWord(POSICIONenFLASH(indexGlobal),(word)time.Sec);
    
  
  return error;
}


 /*******************************************************************************************************/ 
/*Guarda los valores en la flash*/
  
int adquirirValorAut(datoAdq dat[],byte cantidad){
  
  int error;
  //static word anio=0,mes=0,dia=0,hora=0,min=0,seg=0;
  byte i;
  DATEREC dateAdq;
  static word dia=0;
  
    //calculoInternoFechaYHora(&anio,&mes,&dia,&hora,&min,&seg);
    Sys_getDate(&dateAdq);
    if(dia!=(word)dateAdq.Day){
      dia=(word)dateAdq.Day;
      flagFechaHora=ESTADO_SI; 
    }
    if(flagFechaHora==ESTADO_SI ){
      flagFechaHora=ESTADO_ILDE; 
      error=guardarFechaYhora();
    }
    
    for(i=0;i<cantidad;i++){
     
      if(flagFechaHora!=ESTADO_SI)
        indexGlobal+=2;
      
      if(indexGlobal>=(CANT_PAG_ADQ*MEM_PAGINA)/2)
          return ADQ_FULL;
      
      error=(int)WriteWord(POSICIONenFLASH(indexGlobal),*(dat[i].valAdq));     
    }
  
   return error;
  }
 
/*******************************************************************************/
/*Esta funcion es llamada por el box del adquisidor*/
byte setflagFechaHora(int a,byte chan){
   if(a){ 
    flagFechaHora=ESTADO_SI;
   }
   return 0;
} 

/********************************************************************************/

/*borra el numero de pag deseado
actualmente no se utiliza*/
/*
byte BorrarPagAdq(char nroPagABorrar){
   if(nroPagABorrar<CANT_PAG_ADQ)
    return EraseSectorInternal(FLASH_ADQUISIDOR_START+nroPagABorrar*MEM_PAGINA);  
  
  return ERR_RANGE; 
}*/ 


/***********************************************************************************/
/*borra toda la memoria, es llamado desde el box*/
byte BorrarTodoAdq(int a,byte chan){

  char i;
  byte error;
  
  if(a){
    for(i=0;i<CANT_PAG_ADQ;i++)
      error=EraseSectorInternal(FLASH_ADQUISIDOR_START+i*MEM_PAGINA);
      
    stateAdq = ERR_OK;   
    return error;
  }
  return ERR_OK;
}

/************************************************************************************/
/*tranfiere todods los datos a la PC*/
byte bajarTodosDatosAdq(datoAdq dat[],byte cantidad){
  unsigned int i;
  byte j;
  char val[7];
  static word anio=0,mes=0,dia=0,hora=0,min=0,seg=0;
   
   for(i=0;i<((CANT_PAG_ADQ*MEM_PAGINA));i+=cantidad){
      
     WDog1_Clear(); 
    
    /*modifico los valores de la fecha y hora segun los valores guardados inicialmente y el periodo de adquisicion
       */
      
   //calculoInternoFechaYHora(&anio,&mes,&dia,&hora,&min,&seg);
     
    if((seg+PRom[R_AdqPeriodo])>59){
        seg=(seg+PRom[R_AdqPeriodo])-60;
        if((min+1)>59){
          min=0;
          if((hora+1)>23){
            hora=0;
          }else hora++;     
        }else min++;
      }else seg+=PRom[R_AdqPeriodo];
        
      
   if(DATOenFLASH(i)==BLANCO && DATOenFLASH(i+1)==BLANCO)
      break; // encontre el final
   
   if(DATOenFLASH(i)==BLANCO && DATOenFLASH(i+1)!=BLANCO){
      
      dia=FECHAenFLASH((i+1));
      mes=FECHAenFLASH((i+2));
      anio=FECHAenFLASH((i+3));
      hora=FECHAenFLASH((i+4));
      min=FECHAenFLASH((i+5));
      seg=FECHAenFLASH((i+6));
      i+=7;
      printf("%02u/%02u/%u\t",dia,mes,anio);                                    //dia/mes/año
      printf("%02u:%02u:%02u\t",hora,min,seg);                                  //hora:min:seg
     
    }else{
      
      printf("%02u/%02u/%u\t",dia,mes,anio);                                   //dia/mes/año
      printf("%02u:%02u:%02u\t",hora,min,seg);                                 //hora:min:seg
    }
     
     for(j=0;j<cantidad;j++){ 
        convertirString(val ,DATOenFLASH(i+j),dat[j].chan);
        printf("%s\t",val);     //valor
      }
      
      printf("\n");
      
    
    
   }
   
   printf("t"); //envio 't' indicando el final de los datos 
   return ERR_OK;
   
}

/*******************************************************************************************************/
/*void calculoInternoFechaYHora(word *anio,word *mes,word *dia,word *hora,word *min,word *seg){
  
  if(((*seg)+PRom[R_AdqPeriodo])>59){                                          //si mas de 59 s, modifico minuto
        (*seg)=((*seg)+PRom[R_AdqPeriodo])-60;
        if(((*min)+1)>59){                                                     //si mas de 59 min, modifico hora
          (*min)=0;
          if(((*hora)+1)>23){
            (*hora)=0;
            //flagFechaHora=ESTADO_SI;
          }else (*hora)++;     
        }else (*min)++;
      }else (*seg)+=PRom[R_AdqPeriodo];
      
} */



/******************************************************************************************************/
/*convierte a string el valor, verifica, antes, los limites*/

void convertirString (char str[], int val, char chan){
        int max = (Sensores[PRom[R_Sensor+chan]].ydat[Sensores[PRom[R_Sensor+chan]].max_array]/10)-1;
        int min = (Sensores[PRom[R_Sensor+chan]].ydat[0]/10)+1;

        if(val>=9999 || val>=max){
          str[0] = 'O';
          str[1] = 'F';
          str[2] = '\0';
        }else if(val<=-1999 || val<=min){
          str[0] = 'U';
          str[1] = 'F';
          str[2] = '\0';
        }else
          FloatToStr(val,str,6,PRom[R_Decimales+chan]);
          
          
}


/*******************************************************************************************************/
/*void bajarDatoAdq(word index){
   if(index<(CANT_PAG_ADQ*MEM_PAGINA)/2)
    printf("%u ",*(int*)(FLASH_ADQUISIDOR_START+2*index));
} */

/********************************************************************************************************/
long buscarPosicionVacia(void){
  word index;
    
  for(index=0;index<(CANT_PAG_ADQ*MEM_PAGINA)/2 && (DATOenFLASH(index)!=BLANCO  || ((DATOenFLASH(index))==BLANCO && DATOenFLASH(index+1)!=BLANCO)); index++ ){}
  
  if(index>=(CANT_PAG_ADQ*MEM_PAGINA)/2)
    return ADQ_FULL;
  else
    return (long)2*(index);
}

#endif