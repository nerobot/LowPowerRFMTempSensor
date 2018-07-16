/* 
 * File:   uart1.h
 * Author: stevend
 *
 * Created on 12 July 2018, 12:42
 */

#ifndef UART1_H
#define	UART1_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "PIC24FJ64GA202.h"
#include <xc.h>
    
#define DEBUG     
    
#define BR              9600                // Baud rate
#define BRATE           ((FCY/BR)/16)-1     // U1BRG

#define U1MODE_HIGH     0b10000000  // Bits 8-15
#define U1MODE_LOW      0b00000000  // Bits 0-7
#define U1MODE_BOTH     (U1MODE_HIGH << 8) + U1MODE_LOW
#define U1STA_HIGH      0b00000100  // Bits 8-15
#define U1STA_LOW       0b00000000  // Bits 0-7
#define U1STA_BOTH      (U1STA_HIGH << 8) + U1STA_LOW

void initU1();                          //! Initialise the uart1 module
void putU1(char c);                     //! Put a single byte on the uart
void putU1S(char *s);                   //! Print a string of unknown length, ending in \0
void putU1S2(char *s, uint8_t size);    //! Print a string of known length    
    
#ifdef	__cplusplus
}
#endif

#endif	/* UART1_H */

