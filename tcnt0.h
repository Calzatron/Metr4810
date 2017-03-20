/*
 * tcnt0.h
 *
 * Created: 20/03/2017 7:10:35 PM
 *  Author: Callum
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


#endif /* TCNT0_H_ */