;////////////////////////////////////////
;//////				  /////
;////// Routine d'interruption DCI //////
;//////                            //////
;////////////////////////////////////////
/*    
    .include "p30f6014.inc"
    __DCIInterrupt
    
    push.d W0 ;//sauvegarde des registres utilisé par la suite 
    BRA NN,IFS2
    IEC2=1
    CORCON,IPL3
    
    
    
    
    BCLR IFS2
    

*/
