/*
 * File:   initialisation.c
 * Author: tpeea
 *
 * Created on 18 décembre 2019, 18:46
 */


#include <xc.h>
#include "init.h"



///////////////////////////////////////////////////////
//////                                           //////
//////  Initialisation du LCD                    //////
//////                                           //////
///////////////////////////////////////////////////////

void LCDinit(void){
    //config SPIxCON
    _LATG9 = 0;
    TRISGbits.TRISG9=0;
    SPI2CONbits.PPRE=1;
    SPI2CONbits.SPRE=7;
    SPI2CONbits.CKE=0;
    SPI2CONbits.CKP=0;
    SPI2CONbits.MSTEN=1;
    SPI2STATbits.SPIEN=1;
}
///////////////////////////////////////////////////////
//////                                           //////
//////  Initialisation des interrupteurs         //////
//////                                           //////
///////////////////////////////////////////////////////
void initBouton(void){
    // pilotage des led par les SW
TRISDbits.TRISD0=0; // configuration LED1 en sortie
// initialisation INT1 sur SW1
INTCON2bits.INT1EP=1; // interruption sur front descendant.
IPC4bits.INT1IP=1; // priorit´e 1
IFS1bits.INT1IF=0; // raz FLAG
IEC1bits.INT1IE=1; // autorisation interruption INT1

TRISDbits.TRISD1=0; // configuration LED2 en sortie
// initialisation INT2 sur SW2
INTCON2bits.INT2EP=1; // interruption sur front descendant.
_INT2IP=1; // priorit´e 1
IFS1bits.INT2IF=0; // raz FLAG
IEC1bits.INT2IE=1; // autorisation interruption INT2
/*
TRISDbits.TRISD2=0; // configuration LED3 en sortie
// initialisation INT3 sur SW3
INTCON2bits.INT3EP=1; // interruption sur front descendant.
_INT3IP=1; // priorit´e 1
_INT3IF=0; // raz FLAG
_INT3IE=1; // autorisation interruption INT3

TRISDbits.TRISD3=0; // configuration LED4 en sortie
// initialisation INT4 sur SW4
INTCON2bits.INT4EP=1; // interruption sur front descendant.
_INT4IP=1; // priorit´e 1
_INT4IF=0; // raz FLAG
_INT4IE=1; // autorisation interruption INT4
*/
}

void initDCI(void){
    TRISFbits.TRISF6=1;
    DCICON2bits.COFSG=0xF;  //16 slots par trame
    DCICON2bits.WS=0xF;     //16 bit par slots
    DCICON3=6;      //en fait on ateint 8.2kHz ainsi
    DCICON1=0x0000;

    TSCON=0x101;     //0 et 8 en émission
    RSCON=0x101;    //de meme en recetpin

    DCICON2bits.BLEN=0x1;
    
    TXBUF0=1;
    TXBUF1=0x0300;
    
    PORTFbits.RF6=1 ;
    TRISFbits.TRISF6=0;
    PORTFbits.RF6=0;
    wait(1);
    PORTFbits.RF6=1 ;
  
    DCICON1bits.DCIEN=1;
   // --------------------
    while(DCISTATbits.TMPTY==0); //atente TXBUF vidé
   // ------------------
    TXBUF0=1;
    TXBUF1=0x0413;
    while(DCISTATbits.TMPTY==0); //atente TXBUF vidé
    //------------------
    TXBUF0=1;
    TXBUF1=0x010A;
    while(DCISTATbits.TMPTY==0); //atente TXBUF vidé       
    //------------------

    TXBUF0=1;
    TXBUF1=0x0547;
    while(DCISTATbits.TMPTY==0); //atente TXBUF vidé   
    //------------------
    TXBUF0=1;
    TXBUF1=0x065E;
    while(DCISTATbits.TMPTY==0); //atente TXBUF vidé
    //------------------
    TXBUF0=1;
    TXBUF1=0x075C;
    while(DCISTATbits.TMPTY==0); //atente TXBUF vidé 
    //------------------
    TXBUF0=1;
    TXBUF1=0x0900;
    while(DCISTATbits.TMPTY==0); //atente TXBUF vidé      

    wait(1000); //attente pour l'etablissement du calibrage

    IEC2bits.DCIIE=1;
    IFS2bits.DCIIF=0;       //raz flag
}


///////////////////////////////////////////////////////
//////                                           //////
//////  Programme d'attente                      //////
//////                                           //////
///////////////////////////////////////////////////////


void wait(unsigned int time){
    unsigned int l;
    for(l=0;l<time;l++)
    {
        asm("REPEAT #295"); // 295x34ns = 10µs
        asm("NOP");
    }
}




