
#ifndef MOTOR_H
#define	MOTOR_H

#include <xc.h>
#include "types.h"
#include "pins.h"

#define STEPS_PER_REV 2038
#define FLAP_STEPS    (STEPS_PER_REV/12) // flap turns 30 degrees

#define FLAP_FILAMENT 0
#define FLAP_BEADS    1
  
extern bool motorRunning;

void motorInit(void);
void chkMotor(void);
void motorOff(void);
void setFlapPos(uint8 pos); // 0: dry beads, 1: dry filament

#endif	/* MOTOR_H */

