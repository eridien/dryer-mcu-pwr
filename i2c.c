
#include <xc.h>
#include "types.h"
#include "pins.h"
#include "i2c.h"
#include "command.h"
#include "error.h"

uint8 i2cAddrBase; 

volatile uint8 i2cRecvBytes[RECV_BUF_SIZE];
volatile uint8 i2cRecvBytesPtr;
volatile uint8 i2cSendBytes[NUM_SEND_BYTES];
volatile uint8 i2cSendBytesPtr;

void i2cInit(void) { 
  SSP1CON1bits.SSPM = 6;          // slave mode, 7-bit
  SSP1MSK           = 0xfe;       // address mask, check all bits
  SSP1ADD           = I2C_ADDR;   // slave address (7-bit addr << 1)
  SSP1STATbits.SMP  = 0;          // slew-rate enabled
  SSP1STATbits.CKE  = 1;          // smb voltage levels
  SSP1CON2bits.SEN  = 0;          // no clock stretching 
  SSP1CON3bits.AHEN = 0;          // no clock stretch before addr ack
  SSP1CON3bits.DHEN = 0;          // no clock stretch before data ack
  SSP1CON3bits.BOEN = 1;          // enable buffer overwrite check
  SSP1CON1bits.SSPEN = 1;         // Enable the serial port
}

void __interrupt() i2cInterrupt() {
  SSP1IF = 0;
  if(!SSP1STATbits.DA) {
    // address byte
    i2cSendBytesPtr = 0;
    i2cRecvBytesPtr = 0;
    
  } else if(SSP1STATbits.P) {
    // finished packet
    if(!RdNotWrite) 
      handleCmd();

  } else if(!RdNotWrite) {
    // received byte (i2c write to slave)
    if (haveCommand) {
      // last command for this motor not handled yet by event loop
      setErrorInt(OVERFLOW_ERROR);
    } else {
      if(i2cRecvBytesPtr < RECV_BUF_SIZE + 1) 
        i2cRecvBytes[i2cRecvBytesPtr++] = SSP1BUF;
    }
    
  } else {
    // sent byte (i2c read from slave), load buffer for next send
    SSP1BUF = i2cSendBytes[i2cSendBytesPtr++];
  }
}
