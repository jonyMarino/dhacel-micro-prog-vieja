#include "TeclasYDisplay.h"


bool flagHD90=TRUE;


void initDisplay(struct Display *_displays){
  byte i;
  for(i=0;i<CANTIDAD_DISPLAYS;i++){ 
    _displays[i].presentarTexto=Pasar_ASCII;
    _displays[i].presentarValor=PasarNumero;
    _displays[i].presentarEspcific=PasarNumeroExpo;
  }
}


void handlerTeclasYDisplay(struct Display *_displays){
  static byte digito=0;	
  static byte Ndys=0;
  
  switch(Ndys){
    case 0:
            if(digito>0)
	            Switches(DigSupOn[digito]);
            
            #ifdef HD90
        		if(!flagHD90 && (KeyEdge=='u' || KeyEdge=='d'))
        		  KeyEdge=0;
            if (KeyEdge=='r' || KeyEdge=='f'){
              flagHD90 = 0;
              Timer_Run(CHANGE_HD90_TEXT,&flagHD90,UNICO_SET);
              
            }
            #endif
            
            SelecDigit(&_displays[0],digito);
            
            #ifdef HD90
            SetValDigit(&_displays[flagHD90],handlerScroll(&_displays[0],digito))
            #else
            SetValDigit(&_displays[0],handlerScroll(&_displays[0],digito));
            #endif
           
            if(++digito==_displays[0].nroDigitos){
              digito=0;
              #if CANTIDAD_DISPLAYS >1
              Ndys=1;
              #endif
              }
            
            break;
    case 1: 
            if (digito<(_displays[1].nroDigitos-1)){
              if(digito==0)
                Switches(DigInfOn[digito]);
                
               SelecDigit(&_displays[1],digito);
               SetValDigit(&_displays[1],handlerScroll(&_displays[1],digito));
            
            }else 										//Muestro los leds (los puertos que seleccionan el digito ya se encuentran en el DL2)
              SetLeds();
            
            if(++digito==_displays[1].nroDigitos){
              digito=0;
              #if CANTIDAD_DISPLAYS >2
              Ndys=2;
              #else
              Ndys=0;
              #endif
            }
            
            break; 
  
  }
  
  
  
}



///////////////////////////////////////////////////////////////////////
// Rutina de decodificacion de caracteres ASCII a 7 segmentos
//
// IN:  Buffer conteniendo caracteres ASCII.
// OUT: Buffer decodificado para 7 segmentos.
///////////////////////////////////////////////////////////////////////

void AsciiToSevenSeg(void* dys)
{
  byte i, Temp8, DigTmp;
  char *ptrs;
  struct Display* _display=(struct Display*)dys;

  ptrs = _display->Apresentar;
 										 
  for(i=0; i<_display->longMsj; i++)	{
    Temp8=*ptrs;
    Temp8-='-'; 	
 	
 	  if((*ptrs)!=' '){
	      DigTmp=(byte)Car_Ini[Temp8];
 	   }else 	
 	      DigTmp=0x00;
	   /* enciendo punto decimal de acuerdo a Cdt en la pantalla principal */
	  	if(_display->decimales==3-i)
	    	DigTmp|=0x80;  
    	
    	if(_display->decimales==0 && i==3)
	    	DigTmp&=0x7F; 

	  _display->deco7seg[i]=DigTmp;
	 
	  ptrs++;    
  
  }
  
} 

///////////////////////////////////////////////////////////////////////
// Rutina de pasaje de los textos de los boxes al buffer de caracteres ASCII
//
// IN:  Texto.
// OUT: Buffer conteniendo caracteres ASCII y Cantidad de letras
///////////////////////////////////////////////////////////////////////
void Pasar_ASCII(char *PtrText,void * dys){
  byte i;
  struct Display* _display=(struct Display*)dys;
  
  for (i=0,_display->longMsj=0;*PtrText!='\0';i++,_display->longMsj++){
		_display->Apresentar[i]=*PtrText;
		PtrText++;
	}
  
  AsciiToSevenSeg(_display);
}

///////////////////////////////////////////////////////////////////////
// Rutina de codificacion de numeros a caracteres ASCII 
//
// IN:  Valor a mostrar, Display en el cual mostrar, Punto decimal 
//      del valor a mostrar.
// OUT: Numero en ASCII.
///////////////////////////////////////////////////////////////////////

void PasarNumero(int val,void* dys){
  int valortemp;
  char * PtrTexto;
  struct Display* _display=(struct Display*)dys;
  
  _display->longMsj=4;
  PtrTexto = _display->Apresentar;
		
	if (val<0){
		val=0-val;
		
		if (((val/1000)%10)<1)		// 0 
		  *PtrTexto='-';
		else if(((val/1000)%10)<2) // 1
		  *PtrTexto=':';		 // "-1"
		else  
		  *PtrTexto= (val/1000)%10+'0';
		  
	}else if (((val/1000)>0) || (_display->decimales>2))
		*PtrTexto=(val/1000)%10+'0';
  else 
    *PtrTexto = ' ';
	
	valortemp = val;
	val %= 1000;
	PtrTexto++;
	
	if (((valortemp/100)>0) || (_display->decimales>1)) 
	  *PtrTexto= val/100 +'0';
	else 
	  *PtrTexto = ' ';
	
	val = val % 100;
	PtrTexto++;
	
	if (((valortemp/10)>0) || (_display->decimales>0)) 
	  *PtrTexto= val/10 +'0';
	else 
	  *PtrTexto = ' ';
	
	val = val % 10;					 
	PtrTexto++;
	
	*PtrTexto= val+'0';

  AsciiToSevenSeg(_display);	  
}

///////////////////////////////////////////////////////////////////////
// Rutina de codificacion de numeros a notacion exponencial en ASCII 
//
// IN:  Valor a mostrar, Display en el cual mostrar, Punto decimal 
//      del valor a mostrar como si fuese no exponencial
// OUT: Numero en ASCII.
///////////////////////////////////////////////////////////////////////
void PasarNumeroExpo(int val,void * dys) {
  byte e;
  char expo;
  int div;
  struct Display* _display=(struct Display*)dys;
  
  _display->longMsj=4;

  if(val<0){
    _display->Apresentar[0]='E';
    _display->Apresentar[1]='r';
    _display->Apresentar[2]='r';
    _display->Apresentar[3]='D';
  }else{
    div=10;
    for(e=0;(val/div)>0;e++)
      div*=10;
    if(e==0){
      _display->Apresentar[0]=val+'0';
      _display->Apresentar[1]='0';
    }else{
      _display->Apresentar[0]=(int)val/(div/10)+'0';
      _display->Apresentar[1]=((int)val/(div/100))%10+'0';
    }
    
    expo= e-_display->decimales;
    
    if(expo<0){
      expo=-expo;
      _display->Apresentar[2]='-';
    }else
      _display->Apresentar[2]=' ';  
    
    _display->Apresentar[3]=expo+'0';  
  }

  _display->decimales=3;

  AsciiToSevenSeg(_display);	

}


