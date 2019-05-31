
#include <xc.h>
#include "types.h"
#include "clock.h"
#include "pins.h"
#include "motor.h"

void clkInit(void) {
  // timer 0 interrupts every ms
  T016BIT     =  1;
  T0ASYNC     =  0; // sync mode
  T0CON1bits.T0CS       =  4; // Clock Source Internal clock (LFINTOSC, 32 Khz)
  T0CON1bits.T0CKPS     =  5; // input prescaler  is 1:32 (1 ms)
  T0CON0bits.T0OUTPS    =  0; // output prescaler is 1:1 
  TMR0IF       =  0; // int flag
  TMR0IE       =  1; // enable timer int
  T0EN       =  1; // enable timer0
}

volatile uint16 timeTicks;     // units: 1 ms, wraps at 65 seconds

// clock interrupt routine is in motor.c
