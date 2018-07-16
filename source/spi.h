/* 
 * File:   spi.h
 * Author: stevend
 *
 * Created on 24 May 2018, 09:42
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <xc.h> // include processor files - each processor file is guarded.
    
#define CSEE    _RB13       // select line
    
// Define modes of SPI bus 
#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3
    
void spiInit(uint8_t SpiMode);
uint8_t spiTransmit(uint8_t data);
uint8_t WriteSPI2(uint8_t data);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

