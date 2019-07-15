
#ifndef SENSOR_H
#define	SENSOR_H

#define A2D_SETTLE_TIME 100 // ms
#define SENSB 0
#define SENSH 1
#define READINGS 11 // keep this num of readings in history, must be odd


void sensorInit(void);
void chkSensors(void);

uint16 getMedianSensorReading(uint8 sensor);

#endif	/* SENSOR_H */

