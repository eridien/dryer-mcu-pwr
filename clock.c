
#include <xc.h>
#include "types.h"
#include "clock.h"
#include "pins.h"
#include "motor.h"
#include "buzzer.h"
#include "sensor.h"

volatile uint8 timeTicks; 

void clkInit(void) {
  // timer 0 interrupts every ms
  T016BIT               =  0;  // 8-bit counter
  T0ASYNC               =  0;  // sync mode
  T0CON1bits.T0CS       =  4;  // Clock Source LFINTOSC (32 KHz)
  TMR0H                 =  31; // counts to 32 (1 KHz)
  T0CON1bits.T0CKPS     =  0;  // input prescaler  is 1:1
  T0CON0bits.T0OUTPS    =  0;  // output prescaler is 1:1 
  TMR0IF                =  0;  // int flag
  TMR0IE                =  1;  // enable timer int
  T0EN                  =  1;  // enable timer0
}

// interrupts every ms


void clockInterrupt(void) {
  TMR0IF = 0;
  timeTicks++;
  
  chkBuzzer();
  chkSensors();
  // chk motor every 2 ms (500 Hz)
  if(timeTicks & 0x0001)
    chkMotor();
}
