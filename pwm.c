
#include <xc.h>
#include "types.h"
#include "pins.h"
#include "pwm.h"

void setDutyCycle(uint8 pwm, uint16 count) {
  // pwm:  0:fan pwm, 1: heater pwm
  // count ranges from 0 to 1023
  if(pwm == HPWM) {
    // heater
    PWM3DCH   =  count > 2;
    PWM3DCL   = (count & 0x03) << 6;
  } else {
    // fan
    PWM4DCH   =  count > 2;
    PWM4DCL   = (count & 0x03) << 6;
   }
}

void pwmInit(void) {
  // timer2 is base for both PWMs
  T2PR            = 255; // 10-bit resolution
  T2CLKCONbits.CS = 1;   // must use FOsc/4 for PWM clocking
  T2CONbits.CKPS  = 7;   // prescaler is 1:2^^CKPS,    1:128
  T2CONbits.OUTPS = 15;  // postscaler is (1:OUTPS+1), 1:16
  
  T2PSYNC  = 1;       // prescaler synced to FOsc/4
  T2CKPOL  = 0;       // Rising edge of input clock clocks timer/prescaler
  T2CKSYNC = 1;       // ON register bit is synchronized to TMR2_clk input
  T2HLTbits.MODE = 0; // Software gate, timer runs when T2ON == 1
  
  // PWM 3 is heater pwm and PWM 4 is fan pwm
  HPWM_TRIS = 1;
  FPWM_TRIS = 1;
  
  PWM3POL   = 0;  // active-high output
  PWM4POL   = 0;
  
  PWM3OUT   = 0;  // PWM Module Output Level when Bit is Read (??))
  PWM4OUT   = 0;
  
  setDutyCycle(FPWM, 0);   // start fan    duty cycle with 0
  setDutyCycle(HPWM, 0);   // start heater duty cycle with 0
  
  RC2PPS = 0x0b;   // PWM4 fan    is output on C2
  RC3PPS = 0x0c;   // PWM3 heater is output on C3
  
  PWM3EN  = 1; // enable heater pwm module
  PWM4EN  = 1; // enable fan pwm module
  
  T2ON = 1;      // start timer and pwms
}