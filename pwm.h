
#ifndef PWM_H
#define	PWM_H

#define HPWM 0   // pwm param in setDutyCycle
#define FPWM 1

void pwmInit(void);
void setDutyCycle(uint8 pwm, uint16 count);

#endif	/* PWM_H */

