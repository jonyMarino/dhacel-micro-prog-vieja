#ifndef _CAPTURE_H
#define _CAPTURE_H

#include "PE_Types.h"

typedef struct {
  long microsegundos;		
  word pulsos;	 
}TCapture;

typedef enum{
  CAP_OK,
  CAP_UF
}TCapErr;

#define Capture_getMicroSegundos(pCapture) ((pCapture).microsegundos)

#define Capture_getPulsos(pCapture) ((pCapture).pulsos)

#endif