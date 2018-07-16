/*
 * File:   newmainXC16.c
 * Author: scdag
 *
 * Created on 10 July 2018, 20:55
 */
//#define DEBUG

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

//#define LED LATBbits.LATB6
//#define TRIS_LED TRISBbits.TRISB6

void portDelay(){
    Nop();
    Nop();
    Nop();
    Nop();
}

/*void delay(){
    int i = 0;
    int j = 0;
    for (i=0; i<1000; i++){
        for (j=0; j<1000; j++){
            Nop();
        }        
    }
}*/


INT second = 0;
INT minute = 0;
INT hour = 0;

int main(void) {
    
    // Setting up uart1
    initU1();
    putU1("Program starting up.\n\r");
    
    putU1S("Setting up I2C\n\r");
    i2c_init();
    
    putU1S("Setting up MCP\n\r");
    while(!initMCP()){
    putU1S("MCP setup fail.\n\r");}   
    
    
    putU1S("Init SPI module\n\r");
    spiInit(SPI_MODE0);
    
    RFM69(0,0,0);
    putU1S("RFM69 Initialised 1\n\r");
    while (!RFM69Initialize(FREQUENCY, MYNODEID, NETWORKID)){}
    encrypt(ENCRYPTKEY);
    putU1S("RFM69 Initlaised 2\n\r");


    
    while(1){
        mcpWake();
       // putU1S("Obtaining MCP temperature\n\r");        
        uint16_t T = readTemp();
        putU1(T >> 8);
        putU1(T & 0xff);
        mcpShutdown();
        __delay_ms(1000);
    }
    
    
    
    while(1){}
    
 
    // Setting up the RTCC
    __builtin_write_RTCWEN();
    RCFGCALbits.RTCEN = 1;      // RTCC module is enabled
    RCFGCALbits.RTCOE = 1;
    RTCPWCbits.RTCCLK = 0b01;   // Setting RTCC clock source as LPRC (internal oscillator). I might need to change this later for accuracy.
    //RTCPWCbits.
    
     // set RTCC time 2018-07-13 16-31-34
       RCFGCALbits.RTCPTR = 3;        // start the sequence
       RTCVAL = 0x18;    // YEAR
       RTCVAL = 0x713;    // MONTH-1/DAY-1
       RTCVAL = 0x516;    // WEEKDAY/HOURS
       RTCVAL = 0x3134;    // MINUTES/SECONDS
    
    RCFGCALbits.RTCWREN = 0;
    
   
    
    
    /*  // Set the RTCWREN bit
   __builtin_write_RTCWEN();

   RCFGCALbits.RTCEN = 0;
   
   if(!RTCCTimeInitialized())
   {
       // set RTCC time 2018-07-13 16-31-34
       RCFGCALbits.RTCPTR = 3;        // start the sequence
       RTCVAL = 0x18;    // YEAR
       RTCVAL = 0x713;    // MONTH-1/DAY-1
       RTCVAL = 0x516;    // WEEKDAY/HOURS
       RTCVAL = 0x3134;    // MINUTES/SECONDS
   }

   // RTCOUT Alarm Pulse; PWSPRE disabled; RTCLK LPRC; PWCPRE disabled; PWCEN disabled; PWCPOL disabled; 
   RTCPWC = 0x0400;

           
   // Enable RTCC, clear RTCWREN
   RCFGCALbits.RTCEN = 1;
   RCFGCALbits.RTCWREN = 0;*/
    
    ALCFGRPTbits.ALRMEN = 0;    // Alarm is disabled
    
    /*while(1){
       // LED ^= 1;
        //portDelay();
        
        //putU1(PORTBbits.RB6 + 0x30);
        
        RCFGCALbits.RTCPTR = 0b01;
        hour = RTCVAL & 0xff;
        
        timeString[8] = (hour & 0b00001111) + 0x30;
        timeString[7] = (hour >> 4) + 0x30;
        
        RCFGCALbits.RTCPTR = 0b00;
        minute = RTCVAL >> 8;
        
        timeString[21] = (minute & 0b00001111) + 0x30;
        timeString[20] = (minute >> 4) + 0x30;
        
        //RCFGCALbits.RTCPTR = 0b00;
        second = RTCVAL & 0xff;
        
        timeString[34] = (second & 0b00001111) + 0x30;
        timeString[33] = (second >> 4) + 0x30;
        
        putU1S(timeString);
        
        uint16_t tempC = readTempC();
        INT buffer1[] = "XXC\r\n";
        buffer1[0] = tempC >> 8;
        buffer1[1] = tempC & 0xff;
         putU1S(buffer1);
        
        delay();   
        
       // second++;
    }*/
    
    return 0;
}
