/*
 * tcnt0.h
 *
 * Created: 17/04/2017 8:08:11 PM
 *  Author: s4357594
 */ 


#ifndef TCNT0_H_
#define TCNT0_H_

void init_tcnt0(void);

/* Return the current clock tick value - milliseconds since the timer was
 * initialised.
 */
uint32_t get_tcnt0_ticks(void);



#endif /* TCNT0_H_ */