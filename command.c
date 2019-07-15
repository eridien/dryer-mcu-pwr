
#include "types.h"
#include "command.h"
#include "i2c.h"
#include "pwm.h"
#include "motor.h"
#include "sensor.h"
#include "buzzer.h"

volatile bool  haveCommand  = false;
volatile uint8 delayedError = 0;
volatile uint8 curError     = 0;

uint8 curRegister = 0;

void setSendBytes(void);

void resetBoard(void) {
  buzzEnd();
  motorOff();
  setDutyCycle(HPWM, 0);
  setDutyCycle(FPWM, 0);
}

void cmdInit(void) {
  resetBoard();
  haveCommand = false;
  curError    = 0;
  curRegister = 0;
}

void setError(uint8 err) {
  delayedError = err;
}

void setErrorInt(uint8 err) {
  delayedError = err;
}

void setSendBytes(void) {
  uint16 medVal[2];
  
  GIE = 0;
  i2cSendBytes[0] = curRegister;
  
  if (curRegister >= REG_OPTIONS0 && curRegister <= REG_OPTIONS3) {
    NVMCON1bits.NVMREGS = 1;
    NVMADRH = 0x07;
    NVMADRL = 0x80 + (curRegister - REG_OPTIONS0) * 32;
    for(uint8 i=0; i < NUM_OPT_BYTES_IO; i++) {
      NVMCON1bits.RD = 1;
      i2cSendBytes[i+1] = NVMDATL;
    }
  }
  
  else switch (curRegister) {
    case 0: break;
    
    case REG_CHIPID:
      // read-only chip id at addr 0x8100
      NVMCON1bits.NVMREGS = 1;
      NVMADRH = 0x01;
      NVMADRL = 0x00;
      for(uint8 i=0; i<9; i++) {
        NVMCON1bits.RD = 1;
        i2cSendBytes[i+1] = NVMDATL;
      }
      break;
    
    case REG_ERROR:
       i2cSendBytes[1] = curError;
       break;

    case REG_SENS:
      medVal[SENSB] = getMedianSensorReading(SENSB);
      medVal[SENSH] = getMedianSensorReading(SENSH);
      i2cSendBytes[1] = (medVal[SENSB] >> 8   );
      i2cSendBytes[2] = (medVal[SENSB] &  0xff);
      i2cSendBytes[3] = (medVal[SENSH] >> 8   );
      i2cSendBytes[4] = (medVal[SENSH] &  0xff);
      break;
      
    default: 
      setError(REG_MISSING_ERROR);
  }
  GIE = 1;
}

void handleDelayedError() {
  resetBoard();
  if(delayedError != curError)
    buzz(2000); // 2 secs
  curError = delayedError;
  delayedError = 0;
  curRegister = REG_ERROR;
  setSendBytes();
}

uint8 cmdLen;

bool cmdLenIs(uint8 len) {
  if(cmdLen != len) {
    setError(CMD_LEN_ERROR);
    return false;
  }
  return true;
}

// foreground code loop, never returns
void eventLoop (void) {
  while(true) {
    if(delayedError) handleDelayedError();

    else if(haveCommand) {
      cmdLen        = i2cRecvBytes[0];
      uint8 cmdByte = i2cRecvBytes[1];
      
      if (cmdByte >= CMD_SET_OPTIONS0 && cmdByte <= CMD_SET_OPTIONS3) {
        if(cmdLenIs(2+NUM_OPT_BYTES_IO)) {
          uint8 cmdBufIdx = 2;
          uint16 flashAddr = 
                0x780 + (cmdByte - CMD_SET_OPTIONS0) * 32;
          while(cmdBufIdx < 2+NUM_OPT_BYTES_IO) {
            // erase one 32-word row
            NVMADRH = (flashAddr >> 8);
            NVMADRL = (flashAddr & 0x00ff);
            NVMCON1bits.NVMREGS = 0;
            NVMCON1bits.FREE    = 1;
            NVMCON1bits.WREN    = 1;
            // unlock and erase row
            GIE = 0;
            NVMCON2 = 0x55;
            NVMCON2 = 0xaa;
            NVMCON1bits.WR = 1;
            GIE = 1;
            NVMCON1bits.WREN = 0;

            // write one 32-word row
            NVMCON1bits.WREN = 1;
            NVMCON1bits.NVMREGS = 0;
             // fill one row of data latches
            for(uint8 j=0; j<NUM_OPT_BYTES_IO; j++, flashAddr++) {
              NVMCON1bits.LWLO = (j < NUM_OPT_BYTES_IO-1); // write on last word
              NVMADRH = (flashAddr >> 8);
              NVMADRL = (flashAddr & 0x00ff);
              NVMDATH = 0; // only 8 bits per 14-bit word
              NVMDATL = i2cRecvBytes[cmdBufIdx++];
              // unlock and set data latch
              GIE = 0;
              NVMCON2 = 0x55;
              NVMCON2 = 0xaa;
              NVMCON1bits.WR = 1;
              GIE = 1;
            }
            // finished writing one 32-word row
            NVMCON1bits.WREN = 0;
          }
        }

      } else switch (cmdByte) {
        case CMD_CLR_ERR: 
          if(cmdLenIs(2)) {
            curRegister = 0;
            cmdByte     = 0;
          }
          break;

        case CMD_RESET_BOARD: 
          if(cmdLenIs(2)) {
            resetBoard();
            cmdByte = 0;
          }
          break;

        case CMD_SET_HTR:
          if(cmdLenIs(4))
            setDutyCycle(HPWM, ((uint16) i2cRecvBytes[2] << 8) | i2cRecvBytes[3]);
          break;

        case CMD_SET_FAN:
          if(cmdLenIs(4))
            setDutyCycle(FPWM, ((uint16) i2cRecvBytes[2] << 8) | i2cRecvBytes[3]);
          break;

        case CMD_SET_FLAP:
          if(cmdLenIs(3))
            setFlapPos(i2cRecvBytes[2]);
          break;

        case CMD_BEEP: 
          if(cmdLenIs(3))
            buzz(i2cRecvBytes[2]);
          break;

        default:
          // cmd just sets register for next send
          if(cmdLenIs(2) && curRegister != REG_ERROR)
            curRegister = cmdByte;
          break;
      }
      setSendBytes();
      haveCommand = false;
    }
  }
}
