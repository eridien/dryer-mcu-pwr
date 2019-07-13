
#ifndef COMMAND_H
#define	COMMAND_H
 
#define NUM_OPT_BYTES_IO 30  // arduino buffer is only 32 bytes

// commands (second byte))
#define CMD_CLR_ERR       1  // 2 bytes
#define CMD_RESET_BOARD   2  // 2 bytes
#define CMD_SET_HTR       3  // 4 bytes -- 0 to 1023
#define CMD_SET_FAN       4  // 4 bytes -- 0 to 1023
#define CMD_SET_FLAP      5  // 3 bytes -- 0: beads to 255: filament
#define CMD_BEEP          6  // 4 bytes -- ms

#define CMD_SET_OPTIONS0  10 // 32 bytes 
#define CMD_SET_OPTIONS1  11
#define CMD_SET_OPTIONS2  12
#define CMD_SET_OPTIONS3  13

// registers (also second byte)
#define REG_CHIPID        20  // 10 bytes
#define REG_ERROR         21  // 2 bytes
#define REG_SENS          22  // 5 bytes
 
#define REG_OPTIONS0      30  // 31 bytes
#define REG_OPTIONS1      31
#define REG_OPTIONS2      32
#define REG_OPTIONS3      33
 
// error codes 
#define OVERFLOW_ERROR     1
#define RECV_BUF_ERROR     2
#define SEND_BUF_ERROR     3
#define REG_MISSING_ERROR  4
#define CMD_LEN_ERROR      5
#define WCOL_ERROR         6

extern volatile bool  haveCommand;
extern volatile uint8 curError;

void cmdInit(void);
void eventLoop (void);
void setError(uint8 err);
void setErrorInt(uint8 err);

#endif	/* COMMAND_H */

