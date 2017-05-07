/*
 * Project.h
 *
 * Created: 26/04/2017 8:05:48 PM
 *  Author: s4357594
 */ 


#ifndef PROJECT_H_
#define PROJECT_H_


void custom_delay(uint32_t ticks);

typedef struct info {
	int16_t prevSample;
	int16_t sample;
	char* mess;
	uint16_t messLen;
	char buffer[8];
	int16_t ax;
	int16_t ay;
	int16_t az;
	int16_t gx;
	int16_t gy;
	int16_t gz;
	double axg;
	double ayg;
	double azg;
	double gxds;
	double gyds;
	double gzds;
	double sonicD0;
	double sonicD1;
} info;


#endif /* PROJECT_H_ */