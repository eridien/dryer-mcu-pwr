
#include <xc.h>
#include "types.h"
#include "pins.h"
#include "motor.h"
#include "clock.h"

void motorInit(void) {
}

void clockInterrupt(void) {
  TMR0IF = 0;
  timeTicks++;
  
}
