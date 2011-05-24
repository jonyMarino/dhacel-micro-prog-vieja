/*  MODULE str_lib.c  */
/*  Implementacion de funciones para strings no contenidas en stdlib.h*/

#include <stdlib.h>
#include <stdio.h>
#include "Math.h"
#include "stddef.h"
#include "str_lib.h"


char * IntToStr(int integer, char * str, int max_size){
   int i;
   int tmp_integer=integer;
   char * ret;
   
   for(i=0;i<max_size;i++){
    tmp_integer/=10;
    if(!tmp_integer)
      break;  
   }
   
   if(i>=max_size)
    return NULL;
   
   ret=str+i+1;
   *ret = '\0';
   
   for(;i>=0;i--){
    *(str+i)=integer%10+'0';
    integer/=10;
   }
   
   return ret;
}

char * FloatToStr(int integer, char * str, int max_size,int decimales){
   int i;
   int tmp_integer=integer;
   char * ret=str;
 /*  
   for(i=0;i<max_size;i++){
    tmp_integer/=10;
    if(!tmp_integer)
      break;  
   }
   ++i;//sumo uno para que ente el punto
    
   if(i>=max_size)
    return NULL;
   
   ret=str+i+1;
   *ret = '\0';
   
   for(;i>= decimales;i--){
    *(str+i)=integer%10+'0';
    integer/=10;
   }
   
   if(decimales){ 
    *(str+i) = '.';
    --i;
   }
   
   for(;i>=0;--i){
    *(str+i)=integer%10+'0';
    integer/=10;
   }
   */
 /* if (integer<0){
		integer=0-integer;
		
		if (((integer/1000)%10)<1)		// 0 
		  *str='-';
		else if(((integer/1000)%10)<2) // 1
		  *str=':';		 // "-1"
		else  
		  *str= (integer/1000)%10+'0';
		  
	}else if (((integer/1000)>0) || (decimales>2))
		*str=(integer/1000)%10+'0';
  else *str = ' ';
	tmp_integer = integer;
	integer %= 1000;
	str++;
	
	if (((tmp_integer/100)>0) || (decimales>1)) 
	*str= integer/100 +'0';
	else *str = ' ';
	integer = integer % 100;
	str++;
	
	if (((tmp_integer/10)>0) || (decimales>0)) 
	*str= integer/10 +'0';
	else *str = ' ';
	integer = integer % 10;					 
	str++;
	
	*str= integer+'0';
	
	str++;
	*str='\0';

   return ret;
   */
   
   if(decimales){
    int a;
    if(integer<0){
      *str='-';
      integer = -integer;
      ++str;
    }
    a=sprintf(str,"%d.",integer/ pow10(decimales));
    str+=a;
    sprintf(str,"%0*d",decimales,integer% pow10(decimales));
   }else
    sprintf(str,"%d",integer); 
  return str; 
}