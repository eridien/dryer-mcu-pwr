
#include "types.h"
#include "pins.h"
#include "sensor.h"
#include "command.h"

uint8  curSensor;    // 0: SENSB_CHAN, 1: SENSH_CHAN
uint16 sensDelaying; // ms countdown

uint16 curSensorReading[2][READINGS]; // last 5 in ADC counts
uint8  sensorPtr = 0;
uint16 minReading[2] = {0xffff, 0xffff};
uint16 maxReading[2] = {0, 0};
uint16 readings[READINGS]; // used only in getMedianSensorReading

void sensorInit(void) {
  ADCON1bits.ADFM   = 1;  // bits right-justified
  ADCON1bits.ADPREF = 0;  // ref voltage is VDD
  ADCON1bits.ADCS   = 6;  // FOSC/64, 2us/bit, 25us conversion time
  ADACTbits.ADACT   = 0;  // no auto-conversion
  
  SENSH_TRIS = 1;
  SENSB_TRIS = 1;        
  SENSH_ANS  = 1; // heater sensor pin analog input
  SENSB_ANS  = 1; // bottom sensor pin analog input
  
  curSensor          = 0;          // start with bottom sensor
  ADCON0bits.CHS     = SENSB_CHAN; // starting channel select
  sensDelaying       = A2D_SETTLE_TIME; 
  ADCON0bits.GOnDONE = 0;          // no first conversion
  ADCON0bits.ADON    = 1;          // turn on converter
}

// called from interrupt every 1 ms
void chkSensors(void) {
  if(sensDelaying) {
    if(--sensDelaying == 0) {
      // sample delay is done, start conversion
      ADCON0bits.GOnDONE = 1; 
    }
  } else if(!ADCON0bits.GOnDONE) {
    // conversion done, get reading
    uint16 reading = ((uint16) ADRESH << 8) | ADRESL;
    
    if(reading < 50) while(true);
    
    curSensorReading[curSensor][sensorPtr] = reading;
    if(curSensor && ++sensorPtr == READINGS) sensorPtr = 0;
    
    if(reading < minReading[curSensor]) minReading[curSensor] = reading;
    if(reading > maxReading[curSensor]) maxReading[curSensor] = reading;

    curSensor      = 1 - curSensor;
    ADCON0bits.CHS = (curSensor? SENSH_CHAN : SENSB_CHAN);

    sensDelaying = A2D_SETTLE_TIME;
  }
}

uint16 getMedianSensorReading(uint8 sensor) {
  GIE = 0;
  for(int i=0; i < READINGS; i++)
    readings[i] = curSensorReading[sensor][i];
  GIE = 1;
  // bubble sort (quicksort requires recursion)
  uint16 temp;
  bool hadFlip = true;
  while(hadFlip) {
    hadFlip = false;
    for(int i=0; i < READINGS-1; i++) {
      if(readings[i] > readings[i+1]) {
        temp = readings[i+1];
        readings[i+1] = readings[i];
        readings[i] = temp;
        hadFlip = true;
      }
    }
  }
  return readings[READINGS/2];
}
