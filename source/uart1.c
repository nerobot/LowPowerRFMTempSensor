#include "uart1.h"
//#include "libpic30.h"

#ifdef DEBUG

    void initU1(){
        //RPINR18bits.U1RXR5 = 0;
        RPOR2bits.RP5R = 3;
        
        U1BRG = BRATE;
        U1MODE = U1MODE_BOTH;
        U1STA = U1STA_BOTH;
        
        
    }

    void putU1(char c){
       while(U1STAbits.UTXBF);     // wait while Tx buffer is full
        U1TXREG = c;
    }

    void putU1S(char *s){
        while (*s != '\0'){
            putU1(*s++);
        }
       // __delay_ms(250);
    }

    void putU1S2(char *s, uint8_t size){
        uint8_t sizeT = 0;
        while (sizeT++ < size){
            putU1(*s++);
        }
    }
#else
    void initU1(){
    }

    void putU1(char c){
    }

    void putU1S(char *s){
    }

    void putU1S2(char *s, uint8_t size){
    } 
#endif
