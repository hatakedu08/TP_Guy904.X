;////////////////////////////////////////
;//////				  /////
;////// Routine d'interruption DCI //////
;//////                            //////
;////////////////////////////////////////
/*    
    .include "p30f6014.inc"
    __DCIInterrupt
    
    push.d W0 ;//sauvegarde des registres utilisé par la suite 
    move RXBUF0,w1 // on enregistre l'arrivé du signal
    MOVE 0x0001,W2 // on prépare le masque
    COM W2,W2 // on inverse la valeur // on aurait pu la valeur en le précisant au dessus
    AND W1,W2,W3// on effectue le masque
    MOVE 0x0000,W4// on prépare la valeur a envoyé dans le buf1
    
   
    
ELSE CMP TMPTY,0 // on attend que le buffer soit vide
    JNZ ELSE
    
    MOVE W3,TXBUF0 // on envois la valeur dans le buffer de sortie
    MOVE W4,TXBUF1// on envoie la commande
    
    MOVE 0x0000,DCIIF // on reset le flag 
    BCLR IFS2 // On vide IFS2 
    
    end
    

*/
