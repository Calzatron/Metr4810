/*
 * tcnt0.h
 *
 * Created: 17/04/2017 6:44:42 PM
 *  Author: s4357594
 */ 


#ifndef TCNT0_H_
#define TCNT0_H_

#include <stdint.h>

/* Set up our timer to give us an interrupt every millisecond
 * and update our time reference.
 */
void init_tcnt0(void);

/* Return the current clock tick value - milliseconds since the timer was
 * initialised.
 */
uint32_t get_tcnt0_ticks(void);

uint8_t	get_button_(void);

void custom_delay(uint32_t ticks);



#endif /* TCNT0_H_ */