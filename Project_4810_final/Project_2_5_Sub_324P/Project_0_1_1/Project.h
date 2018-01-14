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
	/* Variables used for tracking system information */
	uint8_t pwmSpeed;
	int8_t stepSpeed;
	uint16_t maxStep;
	char stepAct;
	uint8_t stopCommand;
	//int8_t camera;
	uint8_t start;
	uint8_t motorAct;
	uint8_t stupidCount;
	char wormCom;
	char wormStop;
	int8_t wormButtonOff;
	char pivotCom;
} info;

#endif /* PROJECT_H_ */