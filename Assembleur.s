;////////////////////////////////////////
;//////				  /////
;////// Routine d'interruption DCI //////
;//////                            //////
;////////////////////////////////////////
    
    
    __DCIInterrupt
    
    push.d W0 ;//sauvegarde des registres utilisé par la suite 
    BRA NN,IFS2
    IEC2=1
    CORCON,IPL3
    
    
    
    
    BCLR IFS2
    


