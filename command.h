
#ifndef COMMAND_H
#define	COMMAND_H
 
#define NUM_OPTION_BYTES 64 // first half of SAF safe application flash nmmory

// commands (second byte))
#define CMD_CLR_ERR       1
#define CMD_RESET_BOARD   2
#define CMD_SET_HTR       3  // 0 to 1023
#define CMD_SET_FAN       4  // 0 to 1023
#define CMD_SET_FLAP      5  // 0: beads, 1: filament
#define CMD_BEEP          6  // ms
#define CMD_SET_OPTIONS   7  // 64 bytes of options
 
// registers (also second byte)
#define REG_CHIPID       20
#define REG_ERROR        21
#define REG_SENS         22
#define REG_OPTIONS      23

// error codes
#define OVERFLOW_ERROR    1
#define RECV_BUF_ERROR    2
#define SEND_BUF_ERROR    3
#define REG_MISSING_ERROR 4
#define CMD_LEN_ERROR     5

extern volatile bool  haveCommand;
extern volatile uint8 curError;

void cmdInit(void);
void eventLoop (void);
void setError(uint8 err);
void setErrorInt(uint8 err);

#endif	/* COMMAND_H */

