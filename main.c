

// PIC16F15323 Configuration Bit Settings

#pragma config FEXTOSC = OFF    // External Oscillator mode selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINTPLL// Power-up default value for COSC bits (HFINTOSC with 2x PLL, with OSCFRQ = 16 MHz and CDIV = 1:1 (FOSC = 32 MHz))
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; i/o or oscillator function on OSC2)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (FSCM timer enabled)
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR pin function is reset)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config LPBOREN = OFF    // Low-Power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = ON       // Brown-out reset enable bits (Brown-out Reset Enabled, SBOREN bit is ignored)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) set to 1.9V on LF, and 2.45V on F Devices)
#pragma config ZCD = OFF        // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR.)
#pragma config PPS1WAY = ON     // Peripheral Pin Select one-way control (The PPSLOCK bit can be cleared and set only once in software)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a reset)
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled, SWDTEN is ignored)
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)
#pragma config BBSIZE = BB512   // Boot Block Size Selection bits (512 words boot block size)
#pragma config BBEN = OFF       // Boot Block Enable bit (Boot Block disabled)
#pragma config SAFEN = ON       // SAF Enable bit (SAF enabled) -- eeprom substitute
#pragma config WRTAPP = OFF     // Application Block Write Protection bit (Application Block not write protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block not write protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration Register not write protected)
#pragma config WRTSAF = OFF     // Storage Area Flash Write Protection bit (SAF not write protected)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (High Voltage on MCLR/Vpp must be used for programming)
#pragma config CP = OFF         // UserNVM Program memory code protection bit (UserNVM code protection disabled)

#include <xc.h>
#include "pins.h"
#include "types.h"
#include "clock.h"
#include "pwm.h"
#include "sensor.h"
#include "buzzer.h"
#include "i2c.h"
#include "motor.h"

int main(int argc, char** argv) {
  ANSELA = 0; // allow gpio before setting ANSELA
  ANSELC = 0;
  WPUAbits.WPUA3 = 1;  // mclr is input and not used so enable pull-up
  
  // all ints on
  GIE = 1;
  PEIE = 1;
  
  clkInit();
  pwmInit();
  sensorInit();
  motorInit();
  buzzerInit();
  i2cInit();
  cmdInit();
  
  // beep at wakeup
  buzz(100); // 20 msec
  
  // test pwm
  // at grill: 128:43,  256:60, 320:90, 384:144
  setDutyCycle(FPWM, 1023); //set fan to 100%
  setDutyCycle(HPWM, 320); //set htr to 100%
          
  // foreground code loop, never returns
  eventLoop();
  
  return 0;
}

// global interrupt routine
void __interrupt() globalInt() {
  if(TMR0IF) {
    clockInterrupt();
  }
  if(SSP1IF) {
    i2cInterrupt();
  }
}


