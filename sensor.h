
#ifndef SENSOR_H
#define	SENSOR_H

#define A2D_SETTLE_TIME 100

extern volatile uint16 curSensorReading[2]; // in ADC counts

void sensorInit(void);
void chkSensors(void);

#endif	/* SENSOR_H */

