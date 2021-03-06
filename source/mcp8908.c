#include "mcp9808.h"
#include "PIC24FJ64GA202.h"
#include "I2Clib.h"
#include "uart1.h"

//*************************************************************************
// Set Sensor to Shutdown-State or wake up (Conf_Register BIT8)
// 1= shutdown / 0= wake up
//*************************************************************************
//#define i2cStart()      i2c_start_condition()


/*

float readTempC( void )
{
  uint16_t t = read16(MCP9808_REG_AMBIENT_TEMP);

  float temp = t & 0x0FFF;
  temp /=  16.0;
  if (t & 0x1000) temp -= 256;

  return temp;
}
*/

uint8_t initMCP(){
    if (i2c_receive_16bit_data(MCP9808_I2CADDR_DEFAULT, MCP9808_REG_MANUF_ID) != 0x0054) return 0;
    if (i2c_receive_16bit_data(MCP9808_I2CADDR_DEFAULT, MCP9808_REG_DEVICE_ID) != 0x0400) return 0;
    
    //write16( MCP9808_REG_CONFIG, 0x0);
    return 1;
}

uint16_t readTemp( void )
{
  uint16_t t = i2c_receive_16bit_data(MCP9808_I2CADDR_DEFAULT, MCP9808_REG_AMBIENT_TEMP);
  return t;
}

void mcpShutdown(void)
{
  mcpShutdown_wake(1);
}

void mcpShutdown_wake(uint8_t sw_ID)
{
    uint16_t conf_shutdown ;
    uint16_t conf_register = i2c_receive_16bit_data(MCP9808_I2CADDR_DEFAULT, MCP9808_REG_CONFIG);
    if (sw_ID == 1)
    {
       conf_shutdown = conf_register | MCP9808_REG_CONFIG_SHUTDOWN ;
       i2c_write_16bit_data(MCP9808_I2CADDR_DEFAULT, MCP9808_REG_CONFIG, conf_shutdown);
    }
    if (sw_ID == 0)
    {
       conf_shutdown = conf_register & ~MCP9808_REG_CONFIG_SHUTDOWN ;
       i2c_write_16bit_data(MCP9808_I2CADDR_DEFAULT, MCP9808_REG_CONFIG, conf_shutdown);
    }
}

void mcpWake(void)
{
  mcpShutdown_wake(0);
  __delay_ms(250);
}
