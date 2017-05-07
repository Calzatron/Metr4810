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
	uint8_t pwmSpeed;
	int8_t stepSpeed;
	uint16_t maxStep;
	char stepAct;
	uint8_t stopCommand;
	int8_t camera;
	
} info;


#endif /* PROJECT_H_ */