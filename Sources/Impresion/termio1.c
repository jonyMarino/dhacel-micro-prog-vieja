/*****************************************************
  Demo files: Terminal port defintion
 ----------------------------------------------------
   Copyright (c) HIWARE AG, Basel, Switzerland
               All rights reserved
                  Do not modify!
 *****************************************************/

/************************************************************************
   This example shows how to access a virtual on chip IO.
   Calls for terminal output is done via on chip SCI.
   
**************************************************************************/

#include <hidef.h>
#include <termio.h>

#define NOCHAR  -1

typedef struct {
    unsigned char SCxBDH;
    unsigned char SCxBDL;
    unsigned char SCxCR1;
    unsigned char SCxCR2;
    unsigned char SCxSR1;
    unsigned char SCxSR2;
    unsigned char SCxDRH;
    unsigned char SCxDRL;
} SCIStruct;

/* terminal.wnd */
#define SCI_ADDR 0x00c8
#define  SCI (*((SCIStruct*)(SCI_ADDR))) 
 

int TERMIO_GetBusyChar(void) {
   /* checks if there's a character from the terminal channel           */	   	
   if (!(SCI.SCxSR1 & 0x20)) return NOCHAR; /* if any returs the character  */
   return SCI.SCxDRL;                    /*  if not returns EOF  */
 }

char TERMIO_GetChar(void) {
  /* receives character from the terminal channel */
    while (!(SCI.SCxSR1 & 0x20)); /* wait for input */
    return SCI.SCxDRL;
 
}

void TERMIO_PutChar(char ch) {
  /* sends a character to the terminal channel */
    while (!(SCI.SCxSR1 & 0x80));  /* wait for output buffer empty */
    SCI.SCxDRL = ch;
}

void TERMIO_Init(void) {
  /* initializes the communication channel */ 
    *((uint*)&SCI.SCxBDH) = 104;     
    SCI.SCxCR2 = 0x0C;    /* 8 bit, TE and RE set */   
 
}


