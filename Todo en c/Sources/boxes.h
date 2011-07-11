#ifndef Boxes_H
#define Boxes_H
/* archivos include */
#include "display.h"
#include "cnfbox.h"
#include "PE_Error.h"
#include "PWM.h"
#include "TmDt1.h"
#include "IFsh10.h"
//#include "Timer.h"

extern bool FstTime;
extern bool save_parametros;
extern byte Tecla;
extern char is_box_principal;


byte Escribir(TDato * sDato,int valor);

void Boxes_Init(void);

#ifdef jony_28_06
void set_MainText(const char * str);  /* Para poner Textos en el Display inferior*/
void set_MainText1(const char * str);  /* Para poner Textos en el Display inferior*/


#endif

void Exit(void);

void TitleHandler(void);
		
void TitleProgHandler(void);

void EstadoHandler (void);

void TxtHandler(void);

void CondEGralHandler(void);

void NumHandler(void);

void NumExpoHandler(void);
void NumExpoHandler1(void);

void TieSegHandler(void);

void ABHandler(void);

void ABAHandler(void);

void CodHandler(void);

void ProgramaHandler(void);

void SegmentoHandler(void);

void TipoSegHandler(void);

void Num_realtime_Handler(void);

void DiaHandler(void);

void HoraHandler (void);

void ProgHandler(void);

void Temperatura1Handler(void);

void Temperatura2Handler(void);

void CondEHandler(void);

void TiempoHandler(void);

void ToleranciaHandler(void);

void MainHandler(void);

void NumHandlerEsp(void);

void NumHandlervf(void);

void NumHandlervfPrincipal(void);

void NumHandlervfInicial(void);

void NumHandlerLOBOV(void);

byte EscribirSiempre(TDato * sDato,int valor);

void NumHandlerLux(void);

void YearHandler(void);

void MonthHandler(void);

void TxtHandlerAdq(void);

void TxtHandlerAdqHAbilitacion(void);

//void detectoVersionPote (void);

//funciones de accion para el ingreso en los boxes/////////


/* Funcion de accion para Correr un programa*/
/********************************************/

byte CorrerPrograma(int iPrograma,byte chan);
 
byte CambiarSegmento(int iSegmento,byte chan);
 		    
byte CambiarTiempo(int iTiempo,byte chan);

byte A_Sensor(int sensor,byte chan);

byte A_Decimales(int decimales,byte chan);

byte A_Selftun(int valor, byte salida);

void A_Adquiriendo(void);

void setLimitesRPM ();

#endif