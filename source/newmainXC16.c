/*
 * File:   newmainXC16.c
 * Author: scdag
 *
 * Created on 10 July 2018, 20:55
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

#define NETWORKID     0   					// Must be the same for all nodes
#define MYNODEID      TEST_STATION_2_ID   	// My node ID
#define TONODEID      GATEWAY_ID 			// Destination node ID

// RFM69 frequency, uncomment the frequency of your module:
#define FREQUENCY   RF69_433MHZ

// AES encryption (or not):
#define ENCRYPT       true // Set to "true" to use encryption
#define ENCRYPTKEY    "TOPSECRETPASSWRD" // Use the same 16-byte key on all nodes

void portDelay(){
    Nop();
    Nop();
    Nop();
    Nop();
}

int main(void) {
    
    // Setting up uart1
    initU1();
    putU1("Program starting up.\n\r");       
    
    putU1S("Init SPI module\n\r");
    spiInit(SPI_MODE0);
    
    RFM69(0,0,0);
    putU1S("RFM69 Initialised 1\n\r");
    while (!RFM69Initialize(FREQUENCY, MYNODEID, NETWORKID)){}
    encrypt(ENCRYPTKEY);
    putU1S("RFM69 Initialised 2\n\r");

    i2c_init();
    
    while(!initMCP()){
        putU1S("MCP setup fail.\n\r");
    }
    putU1S("MCP Set and ready to go.\n\r");
    
    while(1){
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
