
#include <xc.h>
#include "types.h"
#include "pins.h"
#include "i2c.h"
#include "command.h"

uint8 i2cAddrBase; 
volatile uint8 dummy;

volatile uint8 i2cRecvBytes[RECV_BUF_SIZE];
volatile uint8 i2cRecvBytesPtr;
volatile uint8 i2cSendBytes[SEND_BUF_SIZE];
volatile uint8 i2cSendBytesPtr;

void i2cInit(void) { 
  SSP1CLKPPS         = 0x10;       // clk input  is C0
  SSP1DATPPS         = 0x11;       // dat input  is C1
  RC0PPS             = 0x15;       // clk output is C0
  RC1PPS             = 0x16;       // dat output is C1
  SSP1CON1bits.SSPM  = 6;          // slave mode, 7-bit
  SSP1MSK            = 0xfe;       // address mask, check all bits
  SSP1ADD            = I2C_ADDR;   // slave address (7-bit addr << 1)
  SSP1STATbits.SMP   = 0;          // slew-rate enabled
  SSP1STATbits.CKE   = 0;          // smb voltage levels
  SSP1CON2bits.SEN   = 0;          // no clock stretching 
  SSP1CON2bits.ACKDT = 0;          // send ack
  SSP1CON3bits.AHEN  = 0;          // no clock stretch before addr ack
  SSP1CON3bits.DHEN  = 0;          // no clock stretch before data ack
  SSP1CON3bits.BOEN  = 1;          // enable buffer overwrite check
  SSP1CON1bits.SSPEN = 1;          // Enable the serial port
  SSP1IF = 0;                      // int flag
  SSP1IE = 1;                      // enable ints
}

void __interrupt() i2cInterrupt() {
  SSP1IF = 0;
  if(!SSP1STATbits.DA) {
    // address byte
    i2cSendBytesPtr = 0;
    i2cRecvBytesPtr = 0;
    if(RdNotWrite)
      SSP1BUF = i2cSendBytes[i2cSendBytesPtr++];
    else
      dummy = SSP1BUF;  // clear BF flag
    
  } else if(!RdNotWrite) {
    
    // received byte (i2c write to slave)
    if (haveCommand) {
      // last command for this motor not handled yet by event loop
      setErrorInt(OVERFLOW_ERROR);
      
    } else if(i2cRecvBytesPtr >=  RECV_BUF_SIZE) 
        // too many bytes in command
        setErrorInt(RECV_BUF_ERROR);
    else {
       i2cRecvBytes[i2cRecvBytesPtr++] = SSP1BUF;
       if(i2cRecvBytesPtr == i2cRecvBytes[0])
         haveCommand = true;
    }

  } else {
    
    // sent byte (i2c read from slave), load buffer for next send
    if(i2cSendBytesPtr >= SEND_BUF_SIZE) {
      // asked foe too many bytes
      setErrorInt(SEND_BUF_ERROR);
      SSP1BUF = 0;
    } else
      SSP1BUF = i2cSendBytes[i2cSendBytesPtr++];
  }
}
