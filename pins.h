
#ifndef PINS_H
#define	PINS_H

#include <xc.h>

#define SENSH_TRIS  TRISA2
#define SENSB_TRIS  TRISA4
#define BUZZ_TRIS   TRISA5
#define SCL_TRIS    TRISC0
#define SDA_TRIS    TRISC1
#define HPWM_TRIS   TRISC2
#define FPWM_TRIS   TRISC3
#define MA_TRIS     TRISA1
#define MB_TRIS     TRISA2
#define MC_TRIS     TRISC5
#define MD_TRIS     TRISC4

#define SENSH_LAT  LATA2
#define SENSB_LAT  LATA4
#define BUZZ_LAT   LATA5
#define SCL_LAT    LATC0
#define SDA_LAT    LATC1
#define HPWM_LAT   LATC2
#define FPWM_LAT   LATC3
#define MA_LAT     LATA1
#define MB_LAT     LATA2
#define MC_LAT     LATC5
#define MD_LAT     LATC4

#define SENSH_ANS  ANSELAbits.ANSA2
#define SENSB_ANS  ANSELAbits.ANSA4

#define SENSH_CHAN 2  // for CHS channel select
#define SENSB_CHAN 4

#endif	/* PINS_H */
