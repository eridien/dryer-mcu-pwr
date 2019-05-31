
#include "types.h"
#include "pins.h"
#include "buzzer.h"

volatile uint16 buzzCountdown;

void buzzerInit(void) {
  BUZZ_LAT  = 0;
  BUZZ_TRIS = 0;
}

void buzz(uint16 ms) {
  buzzEnd();
  if(ms) {
    BUZZ_LAT = 1;
    GIE = 0;
    buzzCountdown = ms;
    GIE = 1;
  }
}

void buzzEnd(void){
  GIE = 0;
  buzzCountdown = 0;
  GIE = 1;
  BUZZ_LAT = 0;
}
void buzzEndInt(void){
  buzzCountdown = 0;
  BUZZ_LAT = 0;
}