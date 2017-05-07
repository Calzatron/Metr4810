/*
 * Stepper.h
 *
 * Created: 17/04/2017 6:43:14 PM
 *  Author: s4357594
 */ 


#ifndef STEPPER_H_
#define STEPPER_H_

#include <stdint.h>

void init_step(void);

uint8_t current_step(void);

void step(int8_t speed);



#endif /* STEPPER_H_ */