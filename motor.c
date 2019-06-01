
#include <xc.h>
#include "types.h"
#include "pins.h"
#include "motor.h"

#define CW  1
#define CCW 0

uint8  phase  = 0; // always matches motor phase
uint16 curPos = 0; // 0 to FLAP_STEPS-1
uint16 tgtPos = 0; // 0 to FLAP_STEPS-1
bool motorRunning = false;

void motorOff(void) {
  MA_LAT = 0;  // all pins off
  MB_LAT = 0;
  MC_LAT = 0;
  MD_LAT = 0;
  motorRunning = false;
}

void motorOffInt(void) {
  MA_LAT = 0;  // all pins off
  MB_LAT = 0;
  MC_LAT = 0;
  MD_LAT = 0;
  motorRunning = false;
}

void setFlapPos(uint8 pos) {
  tgtPos = (pos == FLAP_BEADS ? 0 : FLAP_STEPS-1);
  motorRunning = true;
}

void step(uint8 cw) {
  phase += (cw ? 1 : -1);
  if (phase > 3) phase = (cw ? 0 : 3);
  switch (phase) {
    case 0: MA_LAT=1; MB_LAT=0; MC_LAT=0; MD_LAT=0; break;
    case 1: MA_LAT=0; MB_LAT=1; MC_LAT=0; MD_LAT=0; break;
    case 2: MA_LAT=0; MB_LAT=0; MC_LAT=1; MD_LAT=0; break;
    case 3: MA_LAT=0; MB_LAT=0; MC_LAT=0; MD_LAT=1; break;
  }
  curPos += (cw ? 1 : -1);
  if(curPos == FLAP_STEPS) curPos = FLAP_STEPS-1; 
  if(curPos == 0xffff)     curPos = 0;
}

void motorInit(void) {
  motorOff();
  MA_TRIS = 0;
  MB_TRIS = 0;
  MC_TRIS = 0;
  MD_TRIS = 0;
  // start homing
  curPos  = 0;               // FLAP_BEADS
  setFlapPos(FLAP_FILAMENT); // start going to FLAP_STEPS-1, may go past end
}

// called from interrupts every 2 ms
void chkMotor(void) {
  if(motorRunning) {
    if      (curPos < tgtPos) step(CW);
    else if (curPos > tgtPos) step(CCW);
    else motorOffInt();
  }
}
