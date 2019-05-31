
#ifndef BUZZER_H
#define	BUZZER_H

extern volatile uint16 buzzCountdown;

void buzzerInit(void);
void buzz(uint16 ms);
void buzzEnd(void);
void buzzEndInt(void);

#endif	/* BUZZER_H */

