/*
 * Stepper.h
 *
 * Created: 17/04/2017 6:43:14 PM
 *  Author: s4357594
 */ 


#ifndef STEPPER_H_
#define STEPPER_H_

#include <stdint.h>
#include "Project.h"

void init_step(void);

uint16_t current_step(void);

void step(info* info_ptr);

void hold_step(void);

void release_step(void);

#endif /* STEPPER_H_ */