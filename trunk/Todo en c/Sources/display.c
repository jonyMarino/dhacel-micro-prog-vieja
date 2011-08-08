/********************************************************************
*
* Rutinas de visualizacion en display de 7 segmentos. Llama a la 
* funcion de escaneo del teclado ya que comparte pines.
*
********************************************************************/

/* archivos include */
#include "bits2ULN.h"
#include "bits5ULN.h"
#include "Display1.h"
#include "IO_Map.h" 
#include "Mydefines.h"
#include "display.h"
#include "TimerOld.h"
#include "TimeOut.h"
#include "StaticTimer.h"


/* variables de este modulo */

const byte  DigInfOn[4]={0x10,0x20,0x40,0x80};		// señales de control del display
const byte  DigSupOn[4]={0x01,0x02,0x04,0x08};		// señales de control del display

struct Display displays[CANTIDAD_DISPLAYS]={ 
  {0,4,{0},{0},0,0,0,NULL,NULL,NULL},
  {1,5,{0},{0},0,0,0,NULL,NULL,NULL} //posee un digito mas para barrer los leds
};

bool led[NUM_SALIDAS];
bool Scroll = FALSE;



void ResetScroll(){
  byte i;
  
  //resetear variables de SCROLL
  for(i=0;i<CANTIDAD_DISPLAYS;i++)
    displays[i].posicionScroll=0;
  								
	Scroll=FALSE;
	Timer_Run(TIME_SCROLL*2,&Scroll,UNICO_SET);	 //resetear variables de SCROLL
}


void SetLeds(){
  byte leds; 
  
  Display1_PutVal(0);	 
  
  if (led[outc1L]==TRUE)
    leds=1;
  else
    leds=0;
  
  #if NUM_SALIDAS > 1
  if (led[outa1L]==TRUE)
    leds|=2;
  #if NUM_SALIDAS > 2
  if (led[outa2L]==TRUE)
    leds|=4;
  #if NUM_SALIDAS > 3
  if (led[outa3L]==TRUE)
    leds|=8;
  #endif
  #endif
  #endif
  
	bits5ULN_PutVal(leds); 	

}


void SelecDigit(struct Display* dys,byte digit){
  
  Display1_PutVal(0); //apago todos los segmentos	
  
  switch(dys->nroDisplay){
    case 0: 	bits2ULN_PutVal(0);
	            bits5ULN_PutVal(DigSupOn[digit]);   
	            break;
    case 1:   if(digit<2){  
                bits2ULN_PutVal(0);		
                bits5ULN_PutVal(DigInfOn[digit]); 
              }else {
                bits5ULN_PutVal(0);
                bits2ULN_PutVal(DigInfOn[digit]);	
                }
              break;
  }
}


void SetValDigit(struct Display* dys,byte nroCaracter){
     Display1_PutVal(dys->deco7seg[nroCaracter]);
}


byte handlerScroll(struct Display* dys,byte digit){
   byte pos; 

   if (dys->longMsj>dys->nroDigitos){
      
      if (Scroll) {
    	    
    	    Scroll=FALSE;
          dys->posicionScroll++;
          if (dys->posicionScroll==dys->longMsj)
            dys->posicionScroll=0;  
          
          Timer_Run(TIME_SCROLL,&Scroll,UNICO_SET);    
      }
     
      pos=dys->posicionScroll+digit;
      
      if (pos>=dys->longMsj)
        pos=pos-dys->longMsj;
      
   }else
      pos=digit;
    
   return pos; 
  
}


