/* 
 * File:   Nmain.c
 * Author: Dumont Guillaume
 *
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

#include <xc.h>


#define RESET       0x80
#define CLEAR       0x81
#define HOMECLEAR   0x82



#define LCDHomeClear()  LCDWriteCmd(HOMECLEAR)
#define LCDHomeReset()  LCDWriteCmd(RESET)
#define LCDClear()      LCDWriteCmd(CLEAR)

int VolPlus;
int VolMoins;


///////////////////////////////////////////////////////
//////                                           //////
//////  Interruption sur les Interrupteurs SW1-4 //////
//////                                           //////
///////////////////////////////////////////////////////

//Commutation de LED1/2/3/4 (RD0/1/2/3) sur appui SW1/2/3/4(INT1/2/3)
void __attribute__((interrupt,auto_psv)) _INT1Interrupt(void)
{
PORTDbits.RD0=~PORTDbits.RD0;
VolPlus=1;
_INT1IF=0; // acquittement FLAG
}
void __attribute__((interrupt,auto_psv)) _INT2Interrupt(void)
{
PORTDbits.RD1=~PORTDbits.RD1;
VolMoins=1;
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
//////       Programmation de l'écriture         //////
//////          sur le LCD                       //////
///////////////////////////////////////////////////////

static void LCDWriteCmd(char c)
{   SPI2BUF = c;
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

///////////////////////////////////////////////////////
//////                                           //////
//////         Initialisation du DCI             //////
//////                                           //////
///////////////////////////////////////////////////////


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
    
 int i=0;
 int A[] = {0x0000,0x278D,0x4B3B,0x678D,0x79BB,0x7FFF,0x79BB,0x678D,0x4B3B,0x278D,0x0000,0xD873,0xB4C5,0x9873,0x8645,0x8001,0x8645,0x9873,0xB4C5,0xD873};
 int vol;

 
 ///////////////////////////////////////////////////////
//////                                            //////
//////  Programme Principal                       //////
//////                                            //////
////////////////////////////////////////////////////////

int main ( void )
{
    vol=23;
    initBouton();
 
    initDCI();

    LCDinit();
    LCDHomeClear();
    //LCDWriteString("sinus !");

while(1){
    LCDWriteString(vol);
    LCDHomeClear();
 }
}
///////////////////////////////////////////////////////
//////                                           //////
//////     définition de l'interrption           //////
//////                                           //////
///////////////////////////////////////////////////////

int Commande, Envoi;
void __attribute__((interrupt,auto_psv)) _DCIInterrupt(void){
     
     
     Envoi=A[i]&~0x0001;
     Commande=0;
     while(DCISTATbits.TMPTY==0); 
     
     i=(i+1)%20;
   
     
     
     if (VolPlus==1){
         vol++;
         Envoi|=1;
         Commande=0x0700+(0x00ff&vol)*4;
         VolPlus=0;
     }
     if(VolMoins==1){
         vol--;
         Envoi|=1;
         Commande=0x0700+(0x00ff&vol)*4;
         VolMoins=0;
        } 
     
     TXBUF0=Envoi;
     TXBUF1=Commande;
     
     IFS2bits.DCIIF=0; //aquitement flag     
     
}