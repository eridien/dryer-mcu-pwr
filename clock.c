
#include <xc.h>
#include "types.h"
#include "clock.h"
#include "pins.h"
#include "motor.h"
#include "buzzer.h"


volatile uint16 timeTicks; 

void clkInit(void) {
  // timer 0 interrupts every ms
  T016BIT               =  0;
  T0ASYNC               =  0;  // sync mode
  T0CON1bits.T0CS       =  4;  // Clock Source LFINTOSC (32 KHz)
  TMR0H                 =  31; // counts to 32
  T0CON1bits.T0CKPS     =  0;  // input prescaler  is 1:1
  T0CON0bits.T0OUTPS    =  0;  // output prescaler is 1:1 
  TMR0IF                =  0;  // int flag
  TMR0IE                =  1;  // enable timer int
  T0EN                  =  1;  // enable timer0
}

volatile uint16 timeTicks;     // units: 1 ms

void clockInterrupt(void) {
  TMR0IF = 0;
  timeTicks++;
  
  // buzzer
  if(buzzCountdown) {
    if(buzzCountdown-- == 1) 
      buzzEndInt();
  }
  
  // motor
}
