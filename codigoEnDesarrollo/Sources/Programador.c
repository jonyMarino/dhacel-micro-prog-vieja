#include <string.h> 
#include "Programador.h"
#ifdef programador
#include "display.h"
#include "cnfbox.h"
#include "IFsh10.h"
#include "cnfbox.h"
#include "TimerOld.h"

#if End !=0
  #error se va a tener que inicializar los programas en nada[0]=End
#endif 

extern int ValFinal[]; //Variable Proceso

static bool pendiente_negativa; //Determina la pendiente para el tipo rampa velocidad
bool Prog_actualizar;

/////Guardar ante corte//////
struct TProg progToSave;

//#define ProgramaActual progToSave.programa_actual 				
//#define SegmentoActual progToSave.segmento_actual 
//#define SegundosSegmento progToSave.segundos_segmento 		

#ifdef jony_25_06
#define Prog_State progToSave.prog_state 
#endif

#define ProgramReturn progToSave.program_return 
//#define SetPointPrograma progToSave.set_point_programa 				
#define TemperaturaAnterior progToSave.temperatura_anterior 		
////////////////////////


int SegmentoMostrado[CANTIDAD_SAL_CONTROL];
int TiempoEnMinutos[CANTIDAD_SAL_CONTROL];

TSegmentos SegVal[CANTIDAD_SAL_CONTROL];					//Valores temporales para correr el segmento actual
/*const TProgramas  Programa[MAX_PROGRAMAS]={
0}; */
const TProgramas  Programa[MAX_PROGRAMAS] @(FLASH_PROGRAMADOR_START)={		 //Programas guardados en FLASH

no,  //condicion_emer
no,  //tipo_tolerancia
0,  //temperatura_inicial
0,  // nada
0,  //tolerancia

//Tipo    ce  tt  ºC    tempo tol
  rMPt,   cc,  Hi,  100,    1,  0,
  rMPt,   cc,  Hi,   1000,  900,  0,
  rMPt,   cc,  Hi,  200,    1,  0, 
  End,EndP+1,  0,   0,    1,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End ,   0,  0,    0,    0,  0,
  End,    0,  0,    0,    0,  0,
  0,0, 0,0,0,0,0,0,
no,  //condicion_emer
no,  //tipo_tolerancia
0,  //temperatura_inicial
0,  // nada
0,  //tolerancia
//Tipo    ce  tt  ºC    tempo tol
  MSt ,   0,  0,    0,    1,  0,
  rMPV,   0,  0,    0,  100,  0,
  MSt,    0,  0,    0,    1,  0,
  rMPt,   0,  0,   10,    1,  0		 
};       


///////////////////Se fija si el Segmento es END//////////
#ifdef jony_05_07
#pragma INLINE
static bool is_End(byte program, byte segment){
 return Programa[program].Segmento[segment].tipo_segmento==End;
}
#endif

void Prog_Init(void){
byte i;  
  for(i=0;i<CANTIDAD_SAL_CONTROL;i++){
    ProgramaActual[i]=255;	 
 		#ifdef jony_05_07
 		if (PRom[R_Programa+i]!=NO_PROGRAMA){
 		  
 		  ReestablecerPrograma(i);
			//Pongo el Limite del Segmento para ese canal
			#ifdef jony_18_07
			Lim_Segmento2[i]=get_Lim_Seg(PRom[R_Programa+i]-1);
			  #ifdef prog_viejo
        Lim_Segmento2[i]++;
        #endif
			#endif
 		#endif   
    }
  }
  #ifdef jony_25_06
  Timer_Run(1000,&Prog_actualizar,CONTINUO_SET);
  #endif

}
///////////////////Se fija si se termino de ejecutar el programa//////////
bool isProgFinish(byte chan){
byte p,s;
  p= ProgramaActual[chan];
  s= SegmentoActual[chan];
  if(p<255) 
  if(Programa[p].Segmento[s].tipo_segmento==End)
    return TRUE;
  return FALSE;
}

////  Actualiza los Set Point de todos los canales /////
#ifdef jony_25_06
void Prog_ActualizarSetPoints(void){
byte i;
  for(i=0;i<CANTIDAD_SAL_CONTROL;i++){     
    if(Prog_State[i]==PROG_RUNNING && SegVal[i].tipo_segmento!=End)  
      ActualizarSetPointPrograma(i);
  }
  Prog_actualizar=0;
}
#endif

#ifdef jony_18_07
//////////////////Devuelve el Limite del segmento para el canal
byte get_Lim_Seg(byte programa){

      byte lim_seg=0; 		  

 		  while( !is_End(programa,lim_seg)|| lim_seg==MAX_SEGMENTOS )
 		    lim_seg++;  
      
      return lim_seg-1;
}
#endif
//////////////////Devuelve el segmento actual corriendo en el canal channel/////////////////////////////////////////////////
#ifdef jony_05_07
const TSegmentos * get_ActualSegment(byte channel){
  return &Programa[ProgramaActual[channel]].Segmento[SegmentoActual[channel]];
}
#endif
/////////////Se fija si el canal esta dentro de los valores de la tolerancia de *pSegmento//////////////////////////////////////////////////
#ifdef jony_05_07
static bool is_inside_band(const TSegmentos * pSegmento,byte canal){
int tol = pSegmento->tolerancia;
 if(tol){
    
    
    switch(pSegmento->tipo_tolerancia){
      case Lo: if((ValFinal[canal]+tol)<=SetPointPrograma[canal])
                    return FALSE;
      				 break;
      case bAnd:if((ValFinal[canal]+tol)<=SetPointPrograma[canal])
                    return FALSE;			 
      case Hi: if((ValFinal[canal]-tol)>=SetPointPrograma[canal])
                    return FALSE;
    } 
  }
  return TRUE;
}
#endif
/////////////////////Calcula el valor del Set Point a partir de los segundos del segmento/////////////////
#ifdef jony_08_07
void Prog_CalculateSetPoint(byte canal){
bool CambioSeg=FALSE;
long int TmpSet;
  
  switch(SegVal[canal].tipo_segmento){
    case rMPt: TmpSet = (SegVal[canal].temperatura_f- TemperaturaAnterior[canal])*SegundosSegmento[canal];
               TmpSet /= ((long)SegVal[canal].tiempo*60);
               SetPointPrograma[canal] = (int)TmpSet+TemperaturaAnterior[canal];
    					#ifdef jony_22_08
    					 if(SegundosSegmento[canal] >= (long)SegVal[canal].tiempo*60)
                CambioSeg=TRUE;
   						break;
   						#endif
    case MSt:		
    
    case SteP:
              SetPointPrograma[canal] = TemperaturaAnterior[canal];
              if(SegundosSegmento[canal] >= (long)SegVal[canal].tiempo*60)
                CambioSeg=TRUE;
   						break;
    
    case rMPV:

             TmpSet =	 SegVal[canal].velocidad * SegundosSegmento[canal] /60;
             SetPointPrograma[canal] = (int)TmpSet;
             if (pendiente_negativa)
                  SetPointPrograma[canal]*=-1;
             SetPointPrograma[canal] += TemperaturaAnterior[canal];     
             if ((pendiente_negativa &&
             SetPointPrograma[canal]<=SegVal[canal].temperatura_f) ||
             (!pendiente_negativa && 
             SetPointPrograma[canal]>=SegVal[canal].temperatura_f)
             ){
                    SetPointPrograma[canal]=SegVal[canal].temperatura_f;  
                    CambioSeg=TRUE;
                  }

    					break;
   
    case End:
    return;		
  }

  if(CambioSeg){
  CambioSeg=FALSE;
  TemperaturaAnterior[canal] = (int)SetPointPrograma[canal];
  runPrograma(canal,ProgramaActual[canal],++SegmentoActual[canal],0);    
  }  
}
#endif
/////////////////////Actualiza el Set Point Actual (Se llama a cada segundo)//////////////////////////////
void ActualizarSetPointPrograma(byte canal){

  #ifdef jony_05_07
  if(! is_inside_band(&SegVal[canal],canal))
    return;
  #endif

//  if(SegVal[canal].tipo_segmento!=End)
    ++SegundosSegmento[canal];
		TiempoEnMinutos[canal]=SegundosSegmento[canal]/60;
		
  #ifdef jony_08_07
  Prog_CalculateSetPoint(canal);
  #endif
}


////////////////// Carga los valores iniciales de un programa/////////////
//Aclaracion: nunca entra con tipo_segmento= JMP
void runPrograma(byte canal,
                 byte programa, 
                 byte Segmento,
                 #ifdef jony_15_08
                 long segs
                 #endif
           //      #ifdef prog_viejo
           //      ,word tiempo
           //      #endif
                 ){
const TSegmentos * p,* segval;
byte ProgramaAux;
byte i=MAX_PROGRAMAS;
extern word TiempoProgramaCh[CANTIDAD_SAL_CONTROL];

	 

if (programa==0xFF){
  Prog_State[canal]=PROG_STOPED;
  SegundosSegmento[canal]=segs;
  TiempoEnMinutos[canal]=segs/60;
  ProgramaActual[canal]=programa;
  SegmentoActual[canal]=0;
  SegmentoMostrado[canal]=0;
  TiempoProgramaCh[canal]=0;  
  return;
}

if (!Segmento)
    ProgramReturn[canal][programa].Programa=0;

segval=&SegVal[canal];
    
  do{
  p=& Programa[programa].Segmento[Segmento];  
     
     switch(p->tipo_segmento){
       case SteP:
          SetPointPrograma[canal] = p->temperatura_f;
          break;
       case rMPV:
       case MSt:
       case rMPt: 
          if (!Segmento){
            #ifndef _APARATO_VIEJO
            if (Programa[programa].Gral.tipo_temperatura_inicial == AMb)
              SetPointPrograma[canal]=TemperaturaAnterior[canal] = ValFinal[canal];
            else //SEt
            #endif
              SetPointPrograma[canal]=TemperaturaAnterior[canal] = Programa[programa].Gral.temperatura_inicial;
          } 
          else 
              #if defined(jony_05_07) 
              TemperaturaAnterior[canal] = (p-1)->temperatura_f;
              #else
              #error
              #endif

        break;
       
       case JMP: 
        i--;
        ProgramReturn[canal][p->sub_programa].Programa = programa+1;
        ProgramReturn[canal][p->sub_programa].Segmento = Segmento;
        ProgramReturn[canal][p->sub_programa].Ciclos = (byte)p->ciclos;
       
        programa=(byte)p->sub_programa;
        Segmento=0;        
          if (!i)return;//ERROR ****AGREGAR MAS COSAS*****
			  break;
       
       case End:
          if(!ProgramReturn[canal][programa].Programa){
            segval->tipo_segmento = End;
            Prog_State[canal]=PROG_STOPED;
            SegmentoMostrado[canal]=Segmento+OFFSET_SEG1;
            SetPointPrograma[canal]= (p-1)->temperatura_f;  
            return;
          }
          if(ProgramReturn[canal][programa].Ciclos>1){
            ProgramReturn[canal][programa].Ciclos--;
            Segmento=0;
          }else{
            Segmento=ProgramReturn[canal][programa].Segmento+1; 
            ProgramaAux=programa;
            programa=ProgramReturn[canal][programa].Programa-1;
            ProgramReturn[canal][ProgramaAux].Programa=0;     
          }
       break;
     }
  }while(p->tipo_segmento==JMP || p->tipo_segmento==End);
   
if(p->tipo_segmento==rMPV)
  pendiente_negativa = p->temperatura_f < TemperaturaAnterior[canal];  
         
segval->tipo_segmento = p->tipo_segmento;
segval->temperatura_f = p->temperatura_f;
segval->tiempo  			= p->tiempo;

if(Programa[programa].Gral.tipo_tolerancia!=no)
{
  segval->tipo_tolerancia=Programa[programa].Gral.tipo_tolerancia;
  segval->tolerancia= Programa[programa].Gral.tolerancia;
}else{
  segval->tipo_tolerancia = p->tipo_tolerancia; 
  segval->tolerancia = p->tolerancia;
}
 

Prog_State[canal]=PROG_RUNNING;
SegundosSegmento[canal]=segs;
TiempoEnMinutos[canal]=segs/60;
ProgramaActual[canal]=programa;
SegmentoActual[canal]=Segmento;
SegmentoMostrado[canal]=Segmento+OFFSET_SEG1;
#ifdef jony_15_08
Prog_CalculateSetPoint(canal);
#endif
}

#ifdef jony_25_06
void Prog_Stop(void){
  Prog_State[0]=PROG_STOPED;
}
void Prog_Continue(void){
  Prog_State[0]=PROG_RUNNING;
}
void Prog_reset(void){
      #ifdef prog_viejo
      runPrograma(0,PRom[R_Programa]-1, 0,0);
      #else
      runPrograma(0,PRom[R_Programa], 0,0);
      #endif
      Prog_Stop();
}
#endif

#ifdef jony_08_07
t_prog_state get_ProgState(byte canal){
  return Prog_State[canal];  
}

void set_ProgState(t_prog_state state,byte canal){
  Prog_State[canal]=state;  
}
#endif

//////////Funcion al caerse la Energia.IMPLEMENTADO COMO MACRO
/* 
void GuardarPrograma(void){
(void)WriteArray(FLASH_APAGADO_START,0, 8*CANTIDAD_SAL_CONTROL+sizeof(ProgramReturn)/2, (word*)ProgramaActual);
}
  */
///////////////////////////Funcion al caerse la Energia/////
//Aclaracion: nunca entra con tipo_segmento= JMP
void ReestablecerPrograma(byte canal){
TSegmentos *p;
byte CondicionTmp;
register unsigned i;

//recupero los datos
memcpy(&progToSave,(const void*)FLASH_APAGADO_START,sizeof(progToSave));

if(TemperaturaAnterior[canal]==0xFFFF){
 /*HUBO UN ERROR!!!!!!!!!!!(No se llegó a grabar todo en la Flash antes de que se apague el aparato)*/

  #ifdef jony_05_07
  #ifdef prog_viejo
    runPrograma(canal,(byte)PRom[R_Programa+canal]-1,0,0);
  #else
    runPrograma(canal,(byte)PRom[R_Programa+canal],0,0);
  #endif		    
  #else 
  EscribirWord((word)&PRom[R_Programa+canal],NO_PROGRAMA);
  #endif
  return;
}
   
#ifndef jony_05_07
if (ProgramReturn[canal][ProgramaActual[canal]].Programa  // what is this???
  && ((CondicionTmp=Programa[ProgramReturn[canal][ProgramaActual[canal]].Programa-1].Segmento[ProgramReturn[canal][ProgramaActual[canal]].Segmento].condicion_emer)!=no)) {
  if (CondicionTmp!=cc){
    SegmentoActual[canal] = ProgramReturn[canal][ProgramaActual[canal]].Segmento;
    ProgramaActual[canal] = ProgramReturn[canal][ProgramaActual[canal]].Programa-1;
  }
}
#endif

#ifdef jony_05_07

//Reestablesco el SP
#ifdef jony_27_02_08
{
    dword segs=SegundosSegmento[canal];
    t_prog_state state=get_ProgState(canal);
    runPrograma(canal,ProgramaActual[canal],(byte) SegmentoActual[canal],segs);
   // SegundosSegmento[canal]= segs; 
   // Prog_CalculateSetPoint(canal);
    set_ProgState(state,canal);
}
#endif

if( is_inside_band(get_ActualSegment(canal),canal))
    CondicionTmp=cc;  
		
else{
  #ifdef jony_08_07 
    #ifndef prog_viejo
  CondicionTmp=(byte)Programa[ProgramaActual[canal]].Gral.condicion_emer;
  if (CondicionTmp==no)
    #endif
  #endif
    CondicionTmp = (get_ActualSegment(canal))->condicion_emer;
}
#endif

switch(CondicionTmp){
  case rS: 
          if(ProgramaActual[canal]>=0 && ProgramaActual[canal]<MAX_PROGRAMAS && SegmentoActual[canal]>=0 && SegmentoActual[canal]<MAX_SEGMENTOS)
            runPrograma(canal,ProgramaActual[canal],(byte) SegmentoActual[canal],0);
  				 break;
  case rP: 
           if(ProgramaActual[canal]>=0 && ProgramaActual[canal]<MAX_PROGRAMAS)
            runPrograma(canal,ProgramaActual[canal], 0,0);
  				 break;
  case EndP: 
  {
    #ifdef jony_27_02_08 
        #ifdef jony_18_07
      byte prog=PRom[R_Programa+canal];
          #ifdef prog_viejo
      prog-=1;
          #endif
        #endif
    
    runPrograma(canal,prog,get_Lim_Seg(prog)+1,0);
//    SegundosSegmento[canal]=
    #else
    EscribirWord((word)&PRom[R_Programa+canal],NO_PROGRAMA);          
    #endif
    break;
  }
  #ifdef jony_25_06
  case cc: {
    if(ProgramaActual[canal]>=0 && ProgramaActual[canal]<MAX_PROGRAMAS && SegmentoActual[canal]>=0 && SegmentoActual[canal]<MAX_SEGMENTOS
    && SegundosSegmento[canal]>0 && (Prog_State[canal]==PROG_STOPED ||Prog_State[canal]==PROG_RUNNING)){      
      dword segs=SegundosSegmento[canal];
      t_prog_state state=get_ProgState(canal);
      runPrograma(canal,ProgramaActual[canal],(byte) SegmentoActual[canal],segs);
     // SegundosSegmento[canal]= segs; 
     // Prog_CalculateSetPoint(canal);
      set_ProgState(state,canal);
    }
  }
  #endif
}
  

  #ifndef jony_15_08
(void) EraseSectorInternal(FLASH_APAGADO_START);
  #endif

}

void GuardarPrograma(void){
  WriteArray(FLASH_APAGADO_START,0, sizeof(progToSave), (word*)&progToSave);
}
#endif