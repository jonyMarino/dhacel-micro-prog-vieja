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
#include "matriz.h"
#include "display.h"
#include "TimerOld.h"
#include "TimeOut.h"
#include "StaticTimer.h"


/* definiciones */


#define _DPY_SUP			0							// dpy inferior
#define _DPY_INF			1							// dpy superior



/* variables de este modulo */
#pragma CONST_SEG DEFAULT
static const byte  DigInfOn[4]={0x10,0x20,0x40,0x80};		// señales de control del display
static const byte  DigSupOn[4]={0x01,0x02,0x04,0x08};		// señales de control del display

bool led[NUM_SALIDAS];
byte b = 0, d = 0;
byte c;
bool Scroll = FALSE;
byte DotNum[CANTIDAD_DISPLAYS];
byte Nletras[CANTIDAD_DISPLAYS];

/* funciones externas */
extern void Switches(byte);

char  DigDpy[CANTIDAD_DISPLAYS][MAX_DIGITOS];		// caracteres decodificados del buffer ascii
char  Digitos[CANTIDAD_DISPLAYS][MAX_DIGITOS];	// buffer de caracteres ascii a visualizar en 7 segmentos
extern byte KeyEdge;
bool HD90_flag=TRUE;






void ResetScroll(void){
  b=d=0;								//resetear variables de SCROLL
	Scroll=FALSE;
	Timer_Run(TIME_SCROLL*2,&Scroll,UNICO_SET);	 //resetear variables de SCROLL
}
/////////////////////////////////////////////////////////////////////
// Muestro texto en 7 segmentos y escaneo teclado
//
// IN:  nothing
// OUT: nothing
/////////////////////////////////////////////////////////////////////
#pragma INLINE
void DpyAndSwitch(void)
{
  static byte digito=0;				// digito a visualizar (0-3)
  static byte display=_DPY_INF;		// display a refrescar (sup/inf) 	
  byte leds; //conjunto de leds a mostrar
/* cada digito lo refresco cada 1 msg */
/* muestro display inferior */

  if(display==_DPY_INF)
  {
    
/* escaneo la ultima tecla */
  if (digito<4){
    
    if(digito==0)
      Switches(DigInfOn[digito]);
    
    #ifdef HD90
    	if(!HD90_flag && (KeyEdge=='u' || KeyEdge=='d'))
		  KeyEdge=0;
    #endif
    
    Display1_PutVal(0);		//PTA
    if(digito<2)					// PTM
    {  
      bits2ULN_PutVal(0);		//PTJ
      bits5ULN_PutVal(DigInfOn[digito]); //PTM
    }
    
    else 
    {
      bits5ULN_PutVal(0);
      bits2ULN_PutVal(DigInfOn[digito]);		// PTJ
      
    };
    if (Nletras[display]>4){
    if (Scroll) {
  	    Scroll=FALSE;
  	    b++;
        if (b==Nletras[display])
          b=0;  
        Timer_Run(TIME_SCROLL,&Scroll,ONETIME);
		    
    }
    c=b+digito;
    if (c>=Nletras[display]){
    c=c-Nletras[display];
    }
    }else c=digito;
    Display1_PutVal(DigDpy[display][c]);  //PTA
  } else {										//Muestro los leds (los puertos que seleccionan el digito ya se encuentran en el DL2)
  Display1_PutVal(0);	 //PTA
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
	   bits5ULN_PutVal(leds); 	 //PTM
    }
 
  if(++digito==5)
  {
    digito=0;


//dan
    display=_DPY_SUP;
//  if(HD90_flag == 1)
//		display = _DPY_SUP;
//  else display =  _DPY_INF;
//dan  

  }
  }  
  
/* muestro display superior */

  else
  {
  
/* el escaneo del teclado se hace luego del envio de la señal de ON
   del digito para que el pin PULL se encuentre estable */
    
	if(digito>0)
	  Switches(DigSupOn[digito]);
		#ifdef HD90
		if(!HD90_flag && (KeyEdge=='u' || KeyEdge=='d'))
		  KeyEdge=0;
    if (KeyEdge=='r' || KeyEdge=='f'){
      HD90_flag = 0;
      Timer_Run(CHANGE_HD90_TEXT,&HD90_flag,UNICO_SET);
      
    }
    #endif
	Display1_PutVal(0);		 //PTA
	bits2ULN_PutVal(0);
	bits5ULN_PutVal(DigSupOn[digito]);  //PTM
	//Display1_PutVal(DigDpy[0][digito]);	  //PTA
  if (Nletras[display]>4){
    if (Scroll) {
  	    Scroll=FALSE;
  	    d++;
        if (d==Nletras[display])
          d=0;
        Timer_Run(TIME_SCROLL,&Scroll,UNICO_SET);    
    }
    c=d+digito;
    if (c>=Nletras[display]){
    c=c-Nletras[display];
    }
    }else c=digito;
    #ifdef HD90
    Display1_PutVal(DigDpy[!HD90_flag][c]);  //PTA
    #else
    Display1_PutVal(DigDpy[display][c]);  //PTA
    #endif
  if(++digito==4)
  {
    digito=0;

    display=_DPY_INF;


  }
  }  
		  
  

}

///////////////////////////////////////////////////////////////////////
// Rutina de decodificacion de caracteres ASCII a 7 segmentos
//
// IN:  Buffer conteniendo caracteres ASCII.
// OUT: Buffer decodificado para 7 segmentos.
///////////////////////////////////////////////////////////////////////

void AsciiTo7Seg(byte Num_display,byte Dot)
{
  byte i, Temp8, DigTmp;
  char *ptrs;
  

  ptrs = Digitos[Num_display];
 										 
  for(i=0; i<Nletras[Num_display]; i++)	// los N digitos
  {
    Temp8=*ptrs;
    Temp8-='-'; 	
 	
 	if((*ptrs)!=' ')
 	      {
	      DigTmp=(byte)Car_Ini[Temp8];
 	      } else 	DigTmp=0x00;
	      /* enciendo punto decimal de acuerdo a Cdt en la pantalla principal */
	  		if(Dot==3-i)
	    	DigTmp|=0x80;  
    		if(Dot==0 && i==3)
	    	DigTmp&=0x7F; 

	DigDpy[Num_display][i]=DigTmp;
	 
	ptrs++;    
  
  }
  
} 

///////////////////////////////////////////////////////////////////////
// Rutina de pasaje de los textos de los boxes al buffer de caracteres ASCII
//
// IN:  Texto.
// OUT: Buffer conteniendo caracteres ASCII y Cantidad de letras
///////////////////////////////////////////////////////////////////////
void PasarASCII(const char *PtrText,byte Num_display){
for (Nletras[Num_display]=0;*PtrText!='\0'; Nletras[Num_display]++)
		{
		Digitos[Num_display][Nletras[Num_display]]=*PtrText;
		PtrText++;
		}
AsciiTo7Seg(Num_display,0);
}

///////////////////////////////////////////////////////////////////////
// Rutina de codificacion de numeros a caracteres ASCII 
//
// IN:  Valor a mostrar, Display en el cual mostrar, Punto decimal 
//      del valor a mostrar.
// OUT: Numero en ASCII.
///////////////////////////////////////////////////////////////////////

void Pasar_Numero(int valor,byte Num_display,byte Dot){
  int valortemp;
  char * PtrTexto;
  
  Nletras[Num_display]=4;
  PtrTexto = Digitos[Num_display];
		
	if (valor<0){
		valor=0-valor;
		
		if (((valor/1000)%10)<1)		// 0 
		  *PtrTexto='-';
		else if(((valor/1000)%10)<2) // 1
		  *PtrTexto=':';		 // "-1"
		else  
		  *PtrTexto= (valor/1000)%10+'0';
		  
	}else if (((valor/1000)>0) || (Dot>2))
		*PtrTexto=(valor/1000)%10+'0';
  else *PtrTexto = ' ';
	valortemp = valor;
	valor %= 1000;
	PtrTexto++;
	
	if (((valortemp/100)>0) || (Dot>1)) 
	*PtrTexto= valor/100 +'0';
	else *PtrTexto = ' ';
	valor = valor % 100;
	PtrTexto++;
	
	if (((valortemp/10)>0) || (Dot>0)) 
	*PtrTexto= valor/10 +'0';
	else *PtrTexto = ' ';
	valor = valor % 10;					 
	PtrTexto++;
	
	*PtrTexto= valor+'0';

AsciiTo7Seg(Num_display,Dot);	  
}

///////////////////////////////////////////////////////////////////////
// Rutina de codificacion de numeros a notacion exponencial en ASCII 
//
// IN:  Valor a mostrar, Display en el cual mostrar, Punto decimal 
//      del valor a mostrar como si fuese no exponencial
// OUT: Numero en ASCII.
///////////////////////////////////////////////////////////////////////
void Pasar_Numero_Expo(int valor,byte Num_display, byte Dot) {
byte e;
char expo;
int div;
  
Nletras[Num_display]=4;

if(valor<0){
  Digitos[Num_display][0]='E';
  Digitos[Num_display][1]='r';
  Digitos[Num_display][2]='r';
  Digitos[Num_display][3]='D';
}else{
  div=10;
  for(e=0;(valor/div)>0;e++)
    div*=10;
  if(e==0){
    Digitos[Num_display][0]=valor+'0';
    Digitos[Num_display][1]='0';
  } else{
    Digitos[Num_display][0]=(int)valor/(div/10)+'0';
    Digitos[Num_display][1]=((int)valor/(div/100))%10+'0';
  }
  expo= e-Dot;
  if(expo<0){
    expo=-expo;
    Digitos[Num_display][2]='-';
  }else
    Digitos[Num_display][2]=' ';  
  Digitos[Num_display][3]=expo+'0';  
}


AsciiTo7Seg(Num_display,3);	
}


