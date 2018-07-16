/* 
 * File:   PIC24FJ64GA202.h
 * Author: stevend
 *
 * Created on 12 July 2018, 12:41
 */

#ifndef PIC24FJ64GA202_H
#define	PIC24FJ64GA202_H

#ifdef	__cplusplus
extern "C" {
#endif

#define PIC24FJ64GA202                      // PIC used for project
#define XTAL_FREQ       8000000UL           // Raw oscillator freq
#define FCY             XTAL_FREQ / 2       // Most but not all PIC24s
    
#define INT uint8_t

#ifdef	__cplusplus
}
#endif

#endif	/* PIC24FJ64GA202_H */

