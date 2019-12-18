/* 
 * File:   Nmain.c
 * Author: Dumont Guillaume
 * Created on 16 octobre 2019, 16:47
 */
// DSPIC30F6014 Configuration Bit Settings

// 'C' source line config statements

// FOSC
#pragma config FPR = XT_PLL16           // Primary Oscillator Mode (XT w/PLL 16x)
#pragma config FOS = PRI                // Oscillator Source (Primary Oscillator)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Enabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = NONE            // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
////inclusion/////////////////////////////////////////////
#include <xc.h>
#include <stdio.h>
#include "init.h"
//////////////////////define des fonctions LCD////////////////////////////
#define RESET       0x80
#define CLEAR       0x81
#define HOMECLEAR   0x82
#define LCDHomeClear()  LCDWriteCmd(HOMECLEAR)
#define LCDHomeReset()  LCDWriteCmd(RESET)
#define LCDClear()      LCDWriteCmd(CLEAR)

///////////////////////////////////////////////////////
//////                                           //////
//////       Programmation de l'écriture         //////
//////          sur le LCD                       //////
///////////////////////////////////////////////////////

static void LCDWriteCmd(char c){ 
    SPI2BUF = c;
    while(SPI2STATbits.SPITBF);
}
void LCDgoto(int l,int c){
  
}
void LCDWriteChar(char c){
    LCDWriteCmd(0xA8);
    LCDWriteCmd(c);
}

void LCDWriteString(char * str){
    int k = 0;
    while(str[k] != 0){
        LCDWriteChar(str[k++]);
        }    
}
/////////////////////////////////////////////////////////////////////////
/////////Déclaration de variables////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
int VolPlus;
int VolMoins;
int vol=23;////déclaration de la variable Volume (0-31) 
int i=0;//bouclage du sinus
int A[] = {0x0000,0x278D,0x4B3B,0x678D,0x79BB,0x7FFF,0x79BB,0x678D,0x4B3B,0x278D,0x0000,0xD873,0xB4C5,0x9873,0x8645,0x8001,0x8645,0x9873,0xB4C5,0xD873};//initialisation du sinus
int Commande, Envoi;//on définie deux variables secondaires

 /////////
///////////////////////////////////////////////////////
//////                                           //////
//////  Interruption sur les Interrupteurs SW1-4 //////
//////                                           //////
///////////////////////////////////////////////////////

//Commutation de LED1/2/3/4 (RD0/1/2/3) sur appui SW1/2/3/4(INT1/2/3)
void __attribute__((interrupt,auto_psv)) _INT1Interrupt(void)
{
PORTDbits.RD0=~PORTDbits.RD0;
VolPlus=1; // on aciteve le flag du volume
vol=(vol+1)%31;
_INT1IF=0; // acquittement FLAG
}
void __attribute__((interrupt,auto_psv)) _INT2Interrupt(void)
{
PORTDbits.RD1=~PORTDbits.RD1;
VolMoins=1; //// on aciteve le flag du volume
//on décrémente le volume
if (vol!=0){
    vol=(vol-1);
}
_INT2IF=0; // acquittement FLAG
}
void __attribute__((interrupt,auto_psv)) _INT3Interrupt(void)
{
PORTDbits.RD2=~PORTDbits.RD2;
_INT3IF=0; // acquittement FLAG
}
void __attribute__((interrupt,auto_psv)) _INT4Interrupt(void)
{
PORTDbits.RD3=~PORTDbits.RD3;
_INT4IF=0; // acquittement FLAG
}

///////////////////////////////////////////////////////
//////                                           //////
//////     définition de l'interrption           //////
//////                                           //////
///////////////////////////////////////////////////////

void __attribute__((interrupt,auto_psv)) _DCIInterrupt(void){
    A[i]=RXBUF0; //on lit dans un premier temps l'arrivé depuis le GBF
     Envoi=A[i]&~0x0001; // On fait un masque pour le bit de poid faible
     Commande=0;////on initilise commande a 0 
     while(DCISTATbits.TMPTY==0); //atente TXBUF vidé   
     i=(i+1)%20;//on incrémente
     if (VolPlus==1){//si l'interruption de SW1 a été déclanché alors :
         Envoi|=1;//on change la commande d'envoi
         Commande=0x0700+(0x00ff&vol)*4;// on place dans Commande le nouveau Volume
         VolPlus=0;//on acquit le flag
     }
     if(VolMoins==1){
         Envoi|=1;//on change la commande d'envoi
         Commande=0x0700+(0x00ff&vol)*4; // on place dans Commande le nouveau Volume
         VolMoins=0;//on acquit le flag
        }  
     TXBUF0=Envoi;//on place nos variables secondaires dans le buffer
     TXBUF1=Commande;//
     IFS2bits.DCIIF=0; //aquitement flag     
}
 ///////////////////////////////////////////////////////
//////                                            //////
//////  Programme Principal                       //////
//////                                            //////
////////////////////////////////////////////////////////

int main ( void )
{
    initBouton();
    initDCI();
    LCDinit();
    LCDHomeClear();
    char Nbaff[3]={0x30,0x30,0x00};
    LCDWriteString("Volume : ");
    Nbaff[0]=0x30+vol/10;
    Nbaff[1]=0x30+vol%10;
    LCDWriteString(Nbaff);
 
    while(1){
        if (VolPlus || VolMoins ){
            LCDHomeClear();
            LCDWriteString("Volume : ");
            Nbaff[0]=0x30+(vol)/10;
            Nbaff[1]=0x30+(vol)%10;
            LCDWriteString(Nbaff);
        }
     }  
}

