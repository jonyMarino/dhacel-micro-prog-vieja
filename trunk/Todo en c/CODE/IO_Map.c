/** ###################################################################
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : IO_Map.C
**     Project   : cntr_26_07
**     Processor : MC9S12GC32CFU16
**     Beantype  : IO_Map
**     Version   : Driver 01.06
**     Compiler  : CodeWarrior HC12 C Compiler
**     Date/Time : 20/08/2009, 10:06
**     Abstract  :
**         IO_Map.h - implements an IO device's mapping. 
**         This module contains symbol definitions of all peripheral 
**         registers and bits. 
**     Settings  :
**
**     Contents  :
**         No public methods
**
**     (c) Copyright UNIS, spol. s r.o. 1997-2007
**     UNIS, spol. s r.o.
**     Jundrovska 33
**     624 00 Brno
**     Czech Republic
**     http      : www.processorexpert.com
**     mail      : info@processorexpert.com
** ###################################################################*/
/* Based on CPU DB MC9S12GC16_80, version 2.87.392 (RegistersPrg V2.20) */
/* DataSheet : 9S12C128DGV1/D V01.05 */

#include "PE_types.h"
#include "IO_Map.h"

/*lint -save -esym(765, *) */


/* * * * *  8-BIT REGISTERS  * * * * * * * * * * * * * * * */
volatile PORTESTR _PORTE;                                  /* Port E Register; 0x00000008 */
volatile DDRESTR _DDRE;                                    /* Port E Data Direction Register; 0x00000009 */
volatile PEARSTR _PEAR;                                    /* Port E Assignment Register; 0x0000000A */
volatile MODESTR _MODE;                                    /* Mode Register; 0x0000000B */
volatile PUCRSTR _PUCR;                                    /* Pull-Up Control Register; 0x0000000C */
volatile RDRIVSTR _RDRIV;                                  /* Reduced Drive of I/O Lines; 0x0000000D */
volatile EBICTLSTR _EBICTL;                                /* External Bus Interface Control; 0x0000000E */
volatile INITRMSTR _INITRM;                                /* Initialization of Internal RAM Position Register; 0x00000010 */
volatile INITRGSTR _INITRG;                                /* Initialization of Internal Registers Position Register; 0x00000011 */
volatile MISCSTR _MISC;                                    /* Miscellaneous System Control Register; 0x00000013 */
volatile ITCRSTR _ITCR;                                    /* Interrupt Test Control Register; 0x00000015 */
volatile ITESTSTR _ITEST;                                  /* Interrupt Test Register; 0x00000016 */
volatile VREGCTRLSTR _VREGCTRL;                            /* VREG_3V3 - Control Register; 0x00000019 */
volatile MEMSIZ0STR _MEMSIZ0;                              /* Memory Size Register Zero; 0x0000001C */
volatile MEMSIZ1STR _MEMSIZ1;                              /* Memory Size Register One; 0x0000001D */
volatile INTCRSTR _INTCR;                                  /* Interrupt Control Register; 0x0000001E */
volatile HPRIOSTR _HPRIO;                                  /* Highest Priority I Interrupt; 0x0000001F */
volatile DBGC1STR _DBGC1;                                  /* Debug Control Register 1; 0x00000020 */
volatile DBGSCSTR _DBGSC;                                  /* Debug Status and Control Register; 0x00000021 */
volatile DBGCNTSTR _DBGCNT;                                /* Debug Count Register; 0x00000024 */
volatile DBGCCXSTR _DBGCCX;                                /* Debug Comparator C Extended Register; 0x00000025 */
volatile DBGC2STR _DBGC2;                                  /* Debug Control Register 2; 0x00000028 */
volatile DBGC3STR _DBGC3;                                  /* Debug Control Register 3; 0x00000029 */
volatile DBGCAXSTR _DBGCAX;                                /* Debug Comparator A Extended Register; 0x0000002A */
volatile DBGCBXSTR _DBGCBX;                                /* Debug Comparator B Extended Register; 0x0000002D */
volatile PPAGESTR _PPAGE;                                  /* Page Index Register; 0x00000030 */
volatile SYNRSTR _SYNR;                                    /* CRG Synthesizer Register; 0x00000034 */
volatile REFDVSTR _REFDV;                                  /* CRG Reference Divider Register; 0x00000035 */
volatile CRGFLGSTR _CRGFLG;                                /* CRG Flags Register; 0x00000037 */
volatile CRGINTSTR _CRGINT;                                /* CRG Interrupt Enable Register; 0x00000038 */
volatile CLKSELSTR _CLKSEL;                                /* CRG Clock Select Register; 0x00000039 */
volatile PLLCTLSTR _PLLCTL;                                /* CRG PLL Control Register; 0x0000003A */
volatile RTICTLSTR _RTICTL;                                /* CRG RTI Control Register; 0x0000003B */
volatile COPCTLSTR _COPCTL;                                /* CRG COP Control Register; 0x0000003C */
volatile ARMCOPSTR _ARMCOP;                                /* CRG COP Timer Arm/Reset Register; 0x0000003F */
volatile TIOSSTR _TIOS;                                    /* Timer Input Capture/Output Compare Select; 0x00000040 */
volatile CFORCSTR _CFORC;                                  /* Timer Compare Force Register; 0x00000041 */
volatile OC7MSTR _OC7M;                                    /* Output Compare 7 Mask Register; 0x00000042 */
volatile OC7DSTR _OC7D;                                    /* Output Compare 7 Data Register; 0x00000043 */
volatile TSCR1STR _TSCR1;                                  /* Timer System Control Register1; 0x00000046 */
volatile TTOVSTR _TTOV;                                    /* Timer Toggle On Overflow Register; 0x00000047 */
volatile TCTL1STR _TCTL1;                                  /* Timer Control Register 1; 0x00000048 */
volatile TCTL2STR _TCTL2;                                  /* Timer Control Register 2; 0x00000049 */
volatile TCTL3STR _TCTL3;                                  /* Timer Control Register 3; 0x0000004A */
volatile TCTL4STR _TCTL4;                                  /* Timer Control Register 4; 0x0000004B */
volatile TIESTR _TIE;                                      /* Timer Interrupt Enable Register; 0x0000004C */
volatile TSCR2STR _TSCR2;                                  /* Timer System Control Register 2; 0x0000004D */
volatile TFLG1STR _TFLG1;                                  /* Main Timer Interrupt Flag 1; 0x0000004E */
volatile TFLG2STR _TFLG2;                                  /* Main Timer Interrupt Flag 2; 0x0000004F */
volatile PACTLSTR _PACTL;                                  /* 16-Bit Pulse Accumulator A Control Register; 0x00000060 */
volatile PAFLGSTR _PAFLG;                                  /* Pulse Accumulator A Flag Register; 0x00000061 */
volatile ATDSTAT0STR _ATDSTAT0;                            /* ATD Status Register 0; 0x00000086 */
volatile ATDTEST1STR _ATDTEST1;                            /* ATD Test Register; 0x00000089 */
volatile ATDSTAT1STR _ATDSTAT1;                            /* ATD Status Register 1; 0x0000008B */
volatile ATDDIENSTR _ATDDIEN;                              /* ATD Input Enable Register; 0x0000008D */
volatile PORTAD0STR _PORTAD0;                              /* Port AD0 Register; 0x0000008F */
volatile SCICR1STR _SCICR1;                                /* SCI Control Register 1; 0x000000CA */
volatile SCICR2STR _SCICR2;                                /* SCI Control Register 2; 0x000000CB */
volatile SCISR1STR _SCISR1;                                /* SCI Status Register 1; 0x000000CC */
volatile SCISR2STR _SCISR2;                                /* SCI Status Register 2; 0x000000CD */
volatile SCIDRHSTR _SCIDRH;                                /* SCI Data Register High; 0x000000CE */
volatile SCIDRLSTR _SCIDRL;                                /* SCI Data Register Low; 0x000000CF */
volatile SPICR1STR _SPICR1;                                /* SPI Control Register; 0x000000D8 */
volatile SPICR2STR _SPICR2;                                /* SPI Control Register 2; 0x000000D9 */
volatile SPIBRSTR _SPIBR;                                  /* SPI Baud Rate Register; 0x000000DA */
volatile SPISRSTR _SPISR;                                  /* SPI Status Register; 0x000000DB */
volatile SPIDRSTR _SPIDR;                                  /* SPI Data Register; 0x000000DD */
volatile PWMESTR _PWME;                                    /* PWM Enable Register; 0x000000E0 */
volatile PWMPOLSTR _PWMPOL;                                /* PWM Polarity Register; 0x000000E1 */
volatile PWMCLKSTR _PWMCLK;                                /* PWM Clock Select Register; 0x000000E2 */
volatile PWMPRCLKSTR _PWMPRCLK;                            /* PWM Prescale Clock Select Register; 0x000000E3 */
volatile PWMCAESTR _PWMCAE;                                /* PWM Center Align Enable Register; 0x000000E4 */
volatile PWMCTLSTR _PWMCTL;                                /* PWM Control Register; 0x000000E5 */
volatile PWMSCLASTR _PWMSCLA;                              /* PWM Scale A Register; 0x000000E8 */
volatile PWMSCLBSTR _PWMSCLB;                              /* PWM Scale B Register; 0x000000E9 */
volatile PWMSDNSTR _PWMSDN;                                /* PWM Shutdown Register; 0x000000FE */
volatile FCLKDIVSTR _FCLKDIV;                              /* Flash Clock Divider Register; 0x00000100 */
volatile FSECSTR _FSEC;                                    /* Flash Security Register; 0x00000101 */
volatile FCNFGSTR _FCNFG;                                  /* Flash Configuration Register; 0x00000103 */
volatile FPROTSTR _FPROT;                                  /* Flash Protection Register; 0x00000104 */
volatile FSTATSTR _FSTAT;                                  /* Flash Status Register; 0x00000105 */
volatile FCMDSTR _FCMD;                                    /* Flash Command Buffer and Register; 0x00000106 */
volatile PTTSTR _PTT;                                      /* Port T I/O Register; 0x00000240 */
volatile PTITSTR _PTIT;                                    /* Port T Input Register; 0x00000241 */
volatile DDRTSTR _DDRT;                                    /* Port T Data Direction Register; 0x00000242 */
volatile RDRTSTR _RDRT;                                    /* Port T Reduced Drive Register; 0x00000243 */
volatile PERTSTR _PERT;                                    /* Port T Pull Device Enable Register; 0x00000244 */
volatile PPSTSTR _PPST;                                    /* Port T Polarity Select Register; 0x00000245 */
volatile MODRRSTR _MODRR;                                  /* Module Routing Register; 0x00000247 */
volatile PTSSTR _PTS;                                      /* Port S I/O Register; 0x00000248 */
volatile PTISSTR _PTIS;                                    /* Port S Input Register; 0x00000249 */
volatile DDRSSTR _DDRS;                                    /* Port S Data Direction Register; 0x0000024A */
volatile RDRSSTR _RDRS;                                    /* Port S Reduced Drive Register; 0x0000024B */
volatile PERSSTR _PERS;                                    /* Port S Pull Device Enable Register; 0x0000024C */
volatile PPSSSTR _PPSS;                                    /* Port S Polarity Select Register; 0x0000024D */
volatile WOMSSTR _WOMS;                                    /* Port S Wired-Or Mode Register; 0x0000024E */
volatile PTMSTR _PTM;                                      /* Port M I/O Register; 0x00000250 */
volatile PTIMSTR _PTIM;                                    /* Port M Input Register; 0x00000251 */
volatile DDRMSTR _DDRM;                                    /* Port M Data Direction Register; 0x00000252 */
volatile RDRMSTR _RDRM;                                    /* Port M Reduced Drive Register; 0x00000253 */
volatile PERMSTR _PERM;                                    /* Port M Pull Device Enable Register; 0x00000254 */
volatile PPSMSTR _PPSM;                                    /* Port M Polarity Select Register; 0x00000255 */
volatile WOMMSTR _WOMM;                                    /* Port M Wired-Or Mode Register; 0x00000256 */
volatile PTPSTR _PTP;                                      /* Port P I/O Register; 0x00000258 */
volatile PTIPSTR _PTIP;                                    /* Port P Input Register; 0x00000259 */
volatile DDRPSTR _DDRP;                                    /* Port P Data Direction Register; 0x0000025A */
volatile RDRPSTR _RDRP;                                    /* Port P Reduced Drive Register; 0x0000025B */
volatile PERPSTR _PERP;                                    /* Port P Pull Device Enable Register; 0x0000025C */
volatile PPSPSTR _PPSP;                                    /* Port P Polarity Select Register; 0x0000025D */
volatile PIEPSTR _PIEP;                                    /* Port P Interrupt Enable Register; 0x0000025E */
volatile PIFPSTR _PIFP;                                    /* Port P Interrupt Flag Register; 0x0000025F */
volatile PTJSTR _PTJ;                                      /* Port J I/O Register; 0x00000268 */
volatile PTIJSTR _PTIJ;                                    /* Port J Input Register; 0x00000269 */
volatile DDRJSTR _DDRJ;                                    /* Port J Data Direction Register; 0x0000026A */
volatile RDRJSTR _RDRJ;                                    /* Port J Reduced Drive Register; 0x0000026B */
volatile PERJSTR _PERJ;                                    /* Port J Pull Device Enable Register; 0x0000026C */
volatile PPSJSTR _PPSJ;                                    /* Port J Polarity Select Register; 0x0000026D */
volatile PIEJSTR _PIEJ;                                    /* Port J Interrupt Enable Register; 0x0000026E */
volatile PIFJSTR _PIFJ;                                    /* Port J Interrupt Flag Register; 0x0000026F */
volatile PTADSTR _PTAD;                                    /* Port AD I/O Register; 0x00000270 */
volatile PTIADSTR _PTIAD;                                  /* Port AD Input Register; 0x00000271 */
volatile DDRADSTR _DDRAD;                                  /* Port AD Data Direction Register; 0x00000272 */
volatile RDRADSTR _RDRAD;                                  /* Port AD Reduced Drive Register; 0x00000273 */
volatile PERADSTR _PERAD;                                  /* Port AD Pull Device Enable Register; 0x00000274 */
volatile PPSADSTR _PPSAD;                                  /* Port AD Polarity Select Register; 0x00000275 */
/* NVFPROT0 - macro for reading non volatile register      Non volatile Block 0 Flash Protection Register; 0x0000FF0D */
/* NVFSEC - macro for reading non volatile register        Non volatile Flash Security Register; 0x0000FF0F */


/* * * * *  16-BIT REGISTERS  * * * * * * * * * * * * * * * */
volatile PORTABSTR _PORTAB;                                /* Port AB Register; 0x00000000 */
volatile DDRABSTR _DDRAB;                                  /* Port AB Data Direction Register; 0x00000002 */
volatile PARTIDSTR _PARTID;                                /* Part ID Register; 0x0000001A */
volatile DBGTBSTR _DBGTB;                                  /* Debug Trace Buffer Register; 0x00000022 */
volatile DBGCCSTR _DBGCC;                                  /* Debug Comparator C Register; 0x00000026 */
volatile DBGCASTR _DBGCA;                                  /* Debug Comparator A Register; 0x0000002B */
volatile DBGCBSTR _DBGCB;                                  /* Debug Comparator B Register; 0x0000002E */
volatile TCNTSTR _TCNT;                                    /* Timer Count Register; 0x00000044 */
volatile TC0STR _TC0;                                      /* Timer Input Capture/Output Compare Register 0; 0x00000050 */
volatile TC1STR _TC1;                                      /* Timer Input Capture/Output Compare Register 1; 0x00000052 */
volatile TC2STR _TC2;                                      /* Timer Input Capture/Output Compare Register 2; 0x00000054 */
volatile TC3STR _TC3;                                      /* Timer Input Capture/Output Compare Register 3; 0x00000056 */
volatile TC4STR _TC4;                                      /* Timer Input Capture/Output Compare Register 4; 0x00000058 */
volatile TC5STR _TC5;                                      /* Timer Input Capture/Output Compare Register 5; 0x0000005A */
volatile TC6STR _TC6;                                      /* Timer Input Capture/Output Compare Register 6; 0x0000005C */
volatile TC7STR _TC7;                                      /* Timer Input Capture/Output Compare Register 7; 0x0000005E */
volatile PACNTSTR _PACNT;                                  /* Pulse Accumulators Count Register; 0x00000062 */
volatile ATDCTL23STR _ATDCTL23;                            /* ATD Control Register 23; 0x00000082 */
volatile ATDCTL45STR _ATDCTL45;                            /* ATD Control Register 45; 0x00000084 */
volatile ATDDR0STR _ATDDR0;                                /* ATD Conversion Result Register 0; 0x00000090 */
volatile ATDDR1STR _ATDDR1;                                /* ATD Conversion Result Register 1; 0x00000092 */
volatile ATDDR2STR _ATDDR2;                                /* ATD Conversion Result Register 2; 0x00000094 */
volatile ATDDR3STR _ATDDR3;                                /* ATD Conversion Result Register 3; 0x00000096 */
volatile ATDDR4STR _ATDDR4;                                /* ATD Conversion Result Register 4; 0x00000098 */
volatile ATDDR5STR _ATDDR5;                                /* ATD Conversion Result Register 5; 0x0000009A */
volatile ATDDR6STR _ATDDR6;                                /* ATD Conversion Result Register 6; 0x0000009C */
volatile ATDDR7STR _ATDDR7;                                /* ATD Conversion Result Register 7; 0x0000009E */
volatile SCIBDSTR _SCIBD;                                  /* SCI Baud Rate Register; 0x000000C8 */
volatile PWMCNT01STR _PWMCNT01;                            /* PWM Channel Counter 01 Register; 0x000000EC */
volatile PWMCNT23STR _PWMCNT23;                            /* PWM Channel Counter 23 Register; 0x000000EE */
volatile PWMCNT45STR _PWMCNT45;                            /* PWM Channel Counter 45 Register; 0x000000F0 */
volatile PWMPER01STR _PWMPER01;                            /* PWM Channel Period 01 Register; 0x000000F2 */
volatile PWMPER23STR _PWMPER23;                            /* PWM Channel Period 23 Register; 0x000000F4 */
volatile PWMPER45STR _PWMPER45;                            /* PWM Channel Period 45 Register; 0x000000F6 */
volatile PWMDTY01STR _PWMDTY01;                            /* PWM Channel Duty 01 Register; 0x000000F8 */
volatile PWMDTY23STR _PWMDTY23;                            /* PWM Channel Duty 23 Register; 0x000000FA */
volatile PWMDTY45STR _PWMDTY45;                            /* PWM Channel Duty 45 Register; 0x000000FC */
/* BAKEY0 - macro for reading non volatile register        Backdoor Access Key 0; 0x0000FF00 */
/* BAKEY1 - macro for reading non volatile register        Backdoor Access Key 1; 0x0000FF02 */
/* BAKEY2 - macro for reading non volatile register        Backdoor Access Key 2; 0x0000FF04 */
/* BAKEY3 - macro for reading non volatile register        Backdoor Access Key 3; 0x0000FF06 */

/*lint -restore */

/* EOF */
/*
** ###################################################################
**
**     This file was created by UNIS Processor Expert 2.98 [03.98]
**     for the Freescale HCS12 series of microcontrollers.
**
** ###################################################################
*/
