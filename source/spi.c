#include "spi.h"

void spiInit(uint8_t SpiMode){
    // Setting RP pins for the SPI
    // TODO
    //RP11 = RB11 = 
    RPOR5bits.RP11R = 7;        // SPI1 Data Output (MOSI)
    // RP10 = RB10 = 
    RPOR5bits.RP10R = 8;        // Slave Select Output (CE)
    // RP7 = RB7 =
    RPOR3bits.RP6R = 9;         // SPI1 Clock Output (CLK)
    RPINR20bits.SDI1R = 7;      // SPI1 Data In (MISO) on RP7 / RB7
    
    // Set appropriate GPIO pins related with the MSSP and SPI1
    TRISBbits.TRISB6 = 0;      // GPIO RB6 set as an output (SCK1)
    TRISBbits.TRISB11 = 0;       // GPIO RB11 set as an output (SDO1)
    TRISBbits.TRISB10 = 0;      // Slave Select as Output
    TRISBbits.TRISB7 = 1;       // GPIO RB7 set as an input  (SDI1)
    // Disable analog features for the PORTB
    //ANSBbits.ANSB15 = 0;        // Disable analog features for the GPIO RB15
    //ANSAbits.ANSA1 = 0;         // Disable analog features for the GPIO RA1
    //ANSAbits.ANSA0 = 0;         // Disable analog features for the GPIO RA0
    
    // Configure MSSP in the fucntion of SPI1 - Connected to SSP2 (SSP1 used for I2C)
    //SSP2CON1 - Control register
    SPI1CON1Lbits.SPIEN = 0;     // Disable SPI1  module (used while setting up)
    
    SPI1CON1Lbits.MODE = 0b00;  // 8 bit mode
    
    SPI1BRGL = 15;                    // use FPB/32 clock frequency
    SPI1STATLbits.SPIROV = 0; // clear the Overflow      
    
    IEC0bits.SPI1TXIE = 0; // Disable interrupts
    
   // SSP2CON1bits.WCOL = 0;      // Clear collision detect bit
    //SSP2CON1bits.SSPM = 0b0010; // SPI Master mode, Clock = FOSC/32 
    SPI1CON1Lbits.MSTEN = 1;        // Master mode
    
    SPI1CON1Lbits.DISSDO = 0;       // SDO1 (MISO) pin is controlled by module
    SPI1CON1Lbits.DISSDI = 0;       // SDI1 pin is controlled by the module
    SPI1CON1Lbits.DISSCK = 0;       // SCK1 pin is controlled by the module
    
    SPI1CON1Lbits.ENHBUF = 0;       // Enhanced buffer mode is disabled
    SPI1CON1Hbits.AUDEN = 0;        // Audio protocol is distabled
    
    
    
    switch(SpiMode)
    {
    case 0:
      SPI1CON1Lbits.CKP = 0;   // Clock polarity select bit 0 = Idle state for clock is a low level
      SPI1CON1Lbits.CKE = 1;   // SPI Clock Select bit - 1 = Transmit occurs on transition from active to Idle clock state
      break;
    case 1:
      SPI1CON1Lbits.CKP = 0;   // Clock polarity select bit - Idle state for clock is a low level
      SPI1CON1Lbits.CKE = 0;   // SPI Clock Select bit - 0 = Transmit occurs on transition from Idle to active clock state
      break;
    case 2:
      SPI1CON1Lbits.CKP = 1;   // Clock polarity select bit 1 = Idle state for clock is a high level
      SPI1CON1Lbits.CKE = 1;   // SPI Clock Select bit - 1 = Transmit occurs on transition from active to Idle clock state
      break; 
    case 3:
      SPI1CON1Lbits.CKP = 1;   // Clock polarity select bit 1 = Idle state for clock is a high level
      SPI1CON1Lbits.CKE = 0;   // SPI Clock Select bit - 0 = Transmit occurs on transition from Idle to active clock state
      break;  
    default:
      SPI1CON1Lbits.CKP = 0;   // Clock polarity select bit - Idle state for clock is a low level
      SPI1CON1Lbits.CKE = 1;   // SPI Clock Select bit - 1 = Transmit occurs on transition from active to Idle clock state
      break;
}
    
    SPI1CON1Lbits.SMP = 0;       // Sample bit 0 = Input data is sampled at the middle of the data output time
    SPI1CON1Lbits.SPIEN = 1;     // Enable SPI1  module
}

uint8_t spiTransmit(uint8_t data){
    //SSP2CON1bits.WCOL = 0;      // Clear collision detect bit
    SPI1BUFL = data;             // Load data to the SPI sending buffer
    while (SPI1STATLbits.SPITBF == 0){}  // Wait until all data is sent
    return SPI1BUFL;
}

uint8_t WriteSPI2(uint8_t data){
    while (SPI1STATLbits.SPITBF == 1){}  // Wait for TX transfer to complete
    SPI1BUFL  = data;                     // Write to buffer for TX    
    while (SPI1STATLbits.SPIRBF == 0){}  // Wait for RX transfer to complete
    return SPI1BUFL ;                     // Read the received value and return it
}

/*uint8_t ReadSR( void)
{
    // Check the Serial EEPROM status register
    int i;  
    CSEE = 0;               // select the Serial EEPROM
    WriteSPI2( SEE_RDSR);   // send a READ STATUS COMMAND
    i = WriteSPI2( 0);      // send/receive
    CSEE = 1;               // deselect to terminate command
    return i;
} //ReadSR
*/
/*void WriteEnable( void)
{
    // send a Write Enable command
    CSEE = 0;               // select the Serial EEPROM
    WriteSPI2( SEE_WEN);    // write enable command
    CSEE = 1;               // deselect to complete the command
}//WriteEnable   

void iWriteSEE( uint16_t address, uint16_t data)
{ // write a 16-bit value starting at an even address

    // wait until any work in progress is completed
    while ( ReadSR() & 0x1);    // check the WIP flag
    
    // Set the Write Enable Latch
    WriteEnable();
    
    // perform a 16-bit write sequence (2 byte page write)
    CSEE = 0;                   // select the Serial EEPROM
    WriteSPI2( SEE_WRITE);      // write command
    WriteSPI2( address>>8);     // address MSB first
    WriteSPI2( address & 0xfe); // address LSB (word aligned)
    WriteSPI2( data >>8);       // send msb
    WriteSPI2( data & 0xff);    // send lsb
    CSEE = 1;
}//iWriteSEE

uint16_t iReadSEE( uint16_t address)
{ // read a 16-bit value starting at an even address

    int lsb, msb;

    // wait until any work in progress is completed
    while ( ReadSR() & 0x1);    // check the WIP flag
    
    // perform a 16-bit read sequence (two byte sequential read)
    CSEE = 0;                   // select the Serial EEPROM
    WriteSPI2( SEE_READ);       // read command
    WriteSPI2( address>>8);     // address MSB first
    WriteSPI2( address & 0xfe); // address LSB (word aligned)
    msb = WriteSPI2( 0);        // send dummy, read msb
    lsb = WriteSPI2( 0);        // send dummy, read lsb
    CSEE = 1;
    return ((msb<<8)+ lsb);    
}//iReadSEE
*/