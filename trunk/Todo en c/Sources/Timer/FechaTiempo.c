/* MODULE FechaTiempo. */

#include "PE_Error.h"
#include "stddef.h"
#include "FechaTiempo.h"
#include "BaseTiempoDS1307.h"

#pragma CODE_SEG FechaTiempo_CODE                     
#pragma CONST_SEG DEFAULT



/* Table of month length (in days) */
static const byte ULY[12] = {31,28,31,30,31,30,31,31,30,31,30,31}; /* Un-leap-year */
static const byte  LY[12] = {31,29,31,30,31,30,31,31,30,31,30,31}; /* Leap-year */



bool isTiempoValido(byte hora,byte min,byte segs){

  if ((min > 59) || (hora > 23) || (segs>59) ) /* Test correctnes of given time */
    return FALSE;                  /* If not correct then error */
  return TRUE;
}

/*
** ===================================================================
**     Method      :  setTime (bean TimeDate)
**
**     Description :
**         Sets a new actual time.
**     Parameters  :
**         self            - Object pointer
**         Hour            - Hours (0 - 23)
**         Min             - Minutes (0 - 59)
**         Sec             - Seconds (0 - 59)
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_RANGE - Parameter out of range
** ===================================================================
*/
byte setTiempo(void * self,byte Hour,byte Min,byte secs)
{

  if( isTiempoValido( Hour, Min, secs))
    BaseTiempoDS1307_setTiempoValidado(Hour,Min,secs);
  
  return ERR_OK;                       /* OK */

}



bool isFechaValida(word anio,byte mes,byte dia){
  word tY = 1998;                      /* anio counter, starting with 1998 */
  byte tM = 1;                         /* mes counter, starting with January */
  byte tD = 1;                         /* dia counter, starting with 1 */
  byte tW = 4;                         /* Sun - Sat counter, starting with Thu */
  const byte *ptr;                     /* Pointer to ULY/LY table */

  if ((anio < 1998) || (anio > 2099) || (mes > 12) || (mes == 0) || (dia > 31) || (dia == 0)) /* Test correctness of given parameters */
    return FALSE;                  /* If not correct then error */
  if (tY & 3)                          /* Is given anio un-leap-one? */
    ptr = ULY;                         /* Set pointer to un-leap-anio dia table */
  else                                 /* Is given anio leap-one? */
    ptr = LY;                          /* Set pointer to leap-anio dia table */
  ptr--;                               /* Decrement pointer */
  if (ptr[mes] < dia)                /* Does the obtained number of days exceed number of days in the appropriate mes & anio? */
    return FALSE;                  /* If yes (incorrect date inserted) then error */

  return TRUE;
}
/*
** ===================================================================
**     Method      :  setDate
**
**     Description :
**         Set a new actual date.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Year            - Years (16-bit unsigned integer)
**         Month           - Months (8-bit unsigned integer)
**         Day             - Days (8-bit unsigned integer)
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_RANGE - Parameter out of range
** ===================================================================
*/
byte setFecha(word Year,byte Month,byte Day)
{
  if(isFechaValida( Year, Month, Day))
    return  BaseTiempoDS1307_setFechaValidada(Year, Month, Day);
  return ERR_VALUE;
}


/*
** ===================================================================
**     Function      :  FechaTiempo_getMaximoDiaDelMes 
**
**     Description :
**         Retorna la maxima cantidad de dias para el mes de ese anio
**     Parameters  :
**         Year            - anio
**         Month           - Mes
**     Returns     :
**         maxima cantidad de dias ara el mes de ese anio
**         0							 - Error
** ===================================================================
*/

byte  FechaTiempo_getMaximoDiaDelMes(word Year,byte Month){
  const byte *ptr;                     /* Pointer to ULY/LY table */

  if (Month>12 || Month<0)
    return 0;
  if (Year & 3)                          /* Is given year un-leap-one? */
    ptr = ULY;                         /* Set pointer to un-leap-year day table */
  else                                 /* Is given year leap-one? */
    ptr = LY;                          /* Set pointer to leap-year day table */
 return ptr[Month-1];
}

  
void getFecha(DATEREC * date){
  BaseTiempoDS1307_getFecha(date);
}

void getTiempo(TIMEREC * time){
 BaseTiempoDS1307_getTiempo(time);
}                              
                    
/* END FechaTiempo. */

