/*
 * File:   newmainXC16.c
 * Author: stevend
 *
 * Created on 16 July 2018
 */

#include "PIC24FJ64GA202.h"
#include "xc.h"
#include "config.h"
#include "uart1.h"
#include "I2Clib.h"
#include "mcp9808.h"
#include <stdio.h>
#include "RFM69.h"

// Typedef to be updated as needed when new nodes are created. Gateway node will also need updating.
typedef enum {
    GATEWAY_ID,
    WEATHER_STATION_ID,	       
    TEST_STATION_1_ID,
    TEST_STATION_2_ID
} id_t;

#define NETWORKID       0                           // Must be the same for all nodes
#define MYNODEID        TEST_STATION_2_ID           // My node ID
#define TONODEID        GATEWAY_ID                  // Destination node ID

// RFM69 frequency, uncomment the frequency of your module:
#define FREQUENCY       RF69_433MHZ

// AES encryption (or not):
#define ENCRYPT         true                        // Set to "true" to use encryption
#define ENCRYPTKEY      "TOPSECRETPASSWRD"          // Use the same 16-byte key on all nodes

void portDelay(){
    Nop();
    Nop();
    Nop();
    Nop();
}

int main(void) {
    
    // Setting up UART1 module
    initU1();
    putU1("Program starting up.\n\r");       
    
    // Setting up SPI1 module
    putU1S("Init SPI module\n\r");
    spiInit(SPI_MODE0);
    
    // Setting up the RFM69 module
    RFM69(0,0,0);
    putU1S("RFM69 Initialised 1\n\r");
    while (!RFM69Initialize(FREQUENCY, MYNODEID, NETWORKID)){}
    encrypt(ENCRYPTKEY);
    putU1S("RFM69 Initialised 2\n\r");

    // Setting up the I2C module
    i2c_init();
    
    // Setting up the MCP module
    while(!initMCP()){
        putU1S("MCP setup fail.\n\r");
    }
    putU1S("MCP Set and ready to go.\n\r");
    
    while(1){
        // Obtaining the temperature from the MCP module. First wake it up, then obtain temperature, then put it back to sleep.
        mcpWake();
        putU1S("Obtaining MCP temperature\n\r");        
        uint16_t T = readTemp();
       // putU1(T >> 8);
       // putU1(T & 0xff);
        mcpShutdown();
        __delay_ms(1000);
    }
    
    return 0;
}


/*! @mainpage
 * 
 * @author Steven Daglish
 * 
 * @date 16 July 2018
 * 
 * @version 0.1
 * 
 * @section secOverview Overview
 * 
 * Low power RFM temperature sensor to be used in remote weather station (that will be left after moving house).
 *  
 * @section secHistory History
 * 
 *  # 16 July 2018
 *
 *  * RTCC has been tested and it does appear to be working well.
 *  * The I2C MCP module is setup and working well.
 *  * I am now starting the implementation of the SPI / RFM69 interface. This has not been tested yet.
 * 
 */