
#include "types.h"
#include "pins.h"

uint8 curSensor    = 0;     // 0: SENSB_CHAN, 1: SENSH_CHAN
bool  sensDelaying = true;

volatile uint16 curSensorReading[2]; // in ADC counts
        
void sensorInit(void) {
  ADCON1bits.ADFM   = 1;  // bits right-justified
  ADCON1bits.ADPREF = 0;  // ref voltage is VDD
  ADCON1bits.ADCS   = 6;  // conversion rate per bit FOSC/64 (23 usecs/sample)
  ADACTbits.ADACT   = 0;  // no auto-conversion
  
  SENSH_TRIS = 1;
  SENSB_TRIS = 1;        
  SENSH_ANS  = 1; // heater pin analog input
  SENSB_ANS  = 1; // bottom pin analog input
  
  ADCON0bits.CHS     = SENSH_CHAN; // starting channel select
  ADCON0bits.GOnDONE = 0;          // no conversion yet
  ADCON0bits.ADON    = 1;          // turn on converter
}

// called from interrupt
void chkSensors(void) {
  // do nothing if in middle of conversion
  if(ADCON0bits.GOnDONE) return;
  
  if(!sensDelaying) {
    // conversion done, get reading
    curSensorReading[curSensor] = ((uint16) ADRESH << 8) | ADRESL;
    curSensor = 1 - curSensor;
    ADCON0bits.CHS = (curSensor? SENSH_CHAN : SENSB_CHAN);
    sensDelaying = true;
  } else {
    // channel delay is done, start conversion
    sensDelaying = false;
    ADCON0bits.GOnDONE = 1; 
  }
}
