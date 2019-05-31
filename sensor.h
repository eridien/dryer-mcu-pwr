
#ifndef SENSOR_H
#define	SENSOR_H

extern volatile uint16 curSensorReading[2]; // in ADC counts

void sensorInit(void);
void chkSensors(void);

#endif	/* SENSOR_H */

