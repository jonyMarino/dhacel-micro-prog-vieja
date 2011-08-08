#ifndef Boxes_H
#define Boxes_H

#include "display.h"
#include "cnfbox.h"
#include "PE_Error.h"
#include "PWM.h"
#include "IFsh10.h"
#include "TeclasYDisplay.h"


extern bool FstTime;
extern bool save_parametros;
extern byte Tecla;
extern char is_box_principal;
extern int bufferSensor[CANTIDAD_CANALES];
extern bool Mostrar_Proc;
extern bool flagTimeReturn;
extern word screen_cont;
extern int bufferNro;
extern TDato *buffersDato;
extern PunteroF PtrTmp;
extern TPtrBox PtrBox;

byte Escribir(TDato * sDato,int valor);

void Boxes_Init(void);

void isTimeReturnPrincipal();

void set_MainText(const char * str);  /* Para poner Textos en el Display inferior*/

void AnalisisPuntoDecimal();

void handlerTeclasUPoDown(TipoBox tipo);

void Exit(void);

void executeBoxes();

void TitleHandler(void);
		
void TitleProgHandler(void);

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

void setBufferSensor();

#endif