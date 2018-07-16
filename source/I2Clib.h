/* 
 * File:   I2Clib.h
 * Author: stevend
 *
 * Created on July 10, 2018, 8:59 PM
 */

#ifndef I2CLIB_H
#define	I2CLIB_H

// Initializing I2C with PIC:
#include "PIC24FJ64GA202.h"
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <libpic30.h>


void i2c_init(void);



// Initiating an I2C start condition:
void i2c_start_condition( void );

// Initiating an I2C stop condition:
void i2c_stop_condition( void );

// Writing a bit in I2C:
void i2c_write_bit( uint8_t b );

// Reading a bit in I2C:
uint8_t i2c_read_bit( void );

//    if( i2c_write_byte( address << 1, true, false ) )   // start, send address, write

// Writing a byte with I2C:
bool i2c_write_byte( uint8_t B,
                     bool start,
                     bool stop );

// Reading a byte with I2C:
uint8_t i2c_read_byte( bool ack,
                       bool stop );

// Sending a byte with I2C:
bool i2c_send_byte( uint8_t address,
                    uint8_t data );

// Receiving a byte with a I2C:
uint8_t i2c_receive_byte( uint8_t address );
// Sending a byte of data with I2C:
bool i2c_send_byte_data( uint8_t address,
                         uint8_t reg,
                         uint8_t data );

// Receiving a byte of data with I2C:
uint8_t i2c_receive_byte_data( uint8_t address,
                               uint8_t reg );

/**
 * Reads a 16bit values from the I2C module.
 * 
 * @param address   8bit address of the I2C devices
 * @param reg       8bit register that the function will write to.
 * @return          The 16bit data from the I2C device
 */
uint16_t i2c_receive_16bit_data(uint8_t address, uint8_t reg);

/**
 * Writes 16bit word to the I2C module.
 * 
 * @param address   8bit address of the I2C device
 * @param reg       8bit register that the 16bit work is to be read from on the I2C device
 * @param data      16bit data to be written to the I2C device
 * @return          Success / failure to send the data.
 */
bool i2c_write_16bit_data(uint8_t address, uint8_t reg, uint16_t data);

#endif	/* XC_HEADER_TEMPLATE_H */