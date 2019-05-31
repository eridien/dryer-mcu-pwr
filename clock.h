
#ifndef CLOCK_H
#define	CLOCK_H

#include "motor.h"

extern volatile uint16 timeTicks; 

void clkInit(void);
void clockInterrupt(void);


#endif	/* CLOCK_H */

