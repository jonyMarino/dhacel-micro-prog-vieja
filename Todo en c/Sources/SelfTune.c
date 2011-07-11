#include "Control.h"
#include "Mydefines.h"
#include "cnfbox.h"
#ifdef jony_28_06
#include "boxes.h"
#endif
#include "IFsh10.h"
#include "PWM.h"
#include "SelfTune.h"
#include "Parametros.h"


////////////AUTO SINTONIA //////////////////////
dword St_Counter[CANTIDAD_SAL_CONTROL+CANTIDAD_SAL_ALARMA];
byte St_Step[CANTIDAD_SAL_CONTROL+CANTIDAD_SAL_ALARMA];
int prev_val,St_val_max,St_val_min,St_time_cnt;
int St_abrt_cnt[CANTIDAD_SAL_CONTROL];

extern char main_text[5];
extern bool show_main_text;

#ifdef VPROP
extern int duty_vprop;
#endif

#pragma DATA_SEG MYDATA
extern int duty_cont_ch[4];
extern int duty_alar_ch[4];
#pragma DATA_SEG DEFAULT

void SelfTuneControl(byte chan,int spaut){

#define His_stn 4

static int dutystn=0;
int Prop;  

// primero hago el control onoff  

        Prop = proporcional (getValCalculado(chan),								        //calculo señal de error
													 spaut
													 );
  
        dutystn =  duty_onoff(dutystn,                              //hago accion onoff
                              Prop, 
                              -His_stn 
                              );

      if(chan ==0)
        PWM_SetRatio16(dutystn,outc1);								              //pongo salida

        #ifdef VPROP
        duty_vprop = dutystn;																				//vprop es solo canal 1
        #endif


			if(chan ==1)
        PWM_SetRatio16(dutystn,outa3);								                //pongo salida

	      if (St_abrt_cnt[chan]<St_abrt_MAX)
          St_abrt_cnt[chan]++;
	      else 
	        St_Step[chan]=6;                              //veo si me pase y pongo error

   

 switch(St_Step[chan]){							          					             //estoy en el paso cero que es comienzo abajo. 


//En paso 0 no hago nada hasta que suba la temp por arriba del sp. Cuando sube pongo paso 1
   case 0: 
   			setPWM_period(PWM_Anl,0);														 //pongo pwm rapido
        if (getValCalculado(chan)>= (spaut+His_stn/2)){    //Si subo pongo paso 1
          St_Step[chan]++;																		 //
          set_MainText("St 1");                                //anuncio en display 
				  St_abrt_cnt[chan]=0;																 //paro el abort
        } else  set_MainText("St 0");
   break;																										 //y me voy

//En paso 1 solo espero pasar abajo
   case 1: if (getValCalculado(chan)<= (spaut-His_stn/2)){ //si bajo
        St_Step[chan]++;														         //pongo paso 2
        set_MainText("St 2");																 //anuncio en display
				St_abrt_cnt[chan]=0;																 //paro el abort
        prev_val = getValCalculado(chan);                           //e inicio detector de minima
        }
   break;

//En paso 2 detecto minimo, comienzo cuenta, y espero paso siguiente
   case 2: if (getValCalculado(chan) < prev_val){                   //es minimo?
        prev_val = getValCalculado(chan);    								       //si, nuevo minimo
//        St_Counter[chan]=0;																	 //reinicio cuenta
        }
        if (getValCalculado(chan)>= (spaut+His_stn/2)){    //pase el sp?
        St_Step[chan]++;                             				 //pongo paso 3
        set_MainText("St 3");																 //anuncio
				St_abrt_cnt[chan]=0;																 //paro el abort
        St_val_min = prev_val;                               //pero guardo minimo
        St_Counter[chan]=0;																	 //reinicio cuenta al final
        }
	 break;

//En paso 3 busco maximo
   case 3: if (getValCalculado(chan) > prev_val)									   //es maximo?
        prev_val = getValCalculado(chan);            							 //nuevo maximo
        if (getValCalculado(chan)<= (spaut-His_stn/2)){    //pase el sp?
        St_Step[chan]++;                             				 //pongo paso 4
        set_MainText("St 4");																 //anuncio
				St_abrt_cnt[chan]=0;																 //paro el abort
        St_val_max = prev_val;                               //pero guardo maximo
        }
   break;

//En paso 4 busco nuevo munimo
   case 4: if (getValCalculado(chan)<prev_val)										   //busco minimo
        prev_val = getValCalculado(chan);  
        if (getValCalculado(chan)>= (spaut+His_stn/2)){    //pase el sp?
        St_Step[chan]++;                             				 //pongo paso 5
        set_MainText("St 5");																 //anuncio
				St_abrt_cnt[chan]=0;																 //paro el abort
        }
	 break;

//En paso 5 tomo valores							 
        case 5: setPWM_period(PRom[R_Per+chan],0);
                EscribirWord((word)&PRom[R_H1+chan],(int)(St_val_max-St_val_min)*2);
	              EscribirWord((word)&PRom[R_IN1+chan],St_Counter[chan]/1000);
	              EscribirWord((word)&PRom[R_DR1+chan],(St_Counter[chan]/10000));
	              EscribirWord((word)&PRom[R_Stn+chan],St_OFF);
                set_MainText("");
	             break; 

	      case 6: set_MainText("St E");
							 break;


   		}
}


