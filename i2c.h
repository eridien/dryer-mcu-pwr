#ifndef I2C_H
#define	I2C_H

#include <xc.h>
#include "types.h"
#include "motor.h"

#define I2C_ADDR 0x10;  // actual addr is 0x08

#define RECV_BUF_SIZE   10 // temp
#define NUM_SEND_BYTES  10 // temp

#define RdNotWrite SSP1STATbits.I2C_READ
#define I2C_SSPOV  SSP1CON1bits.SSPOV

extern volatile uint8 i2cRecvBytes[10];  // ten bytes in recv buffer
extern volatile uint8 i2cRecvBytesPtr;
extern volatile uint8 i2cSendBytes[10];// ten bytes in send buffer
extern volatile uint8 i2cSendBytesPtr;

void i2cInit(void);
void i2cInterrupt(void);

#endif	/* I2C_H */
