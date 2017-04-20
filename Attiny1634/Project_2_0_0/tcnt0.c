/*
 * tcnt0.c
 *
 * Created: 17/04/2017 8:07:51 PM
 *  Author: s4357594
 */ 


 #include <avr/io.h>
 #include <avr/interrupt.h>			//enables interrupts
 #include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
 #include <stdio.h>					//input / output
 #include <stdlib.h>					//standard function library; malloc / sort / rand etc
 //#include <util/delay.h>
 
 #include "tcnt0.h"


// counter incrementing on OCRA
static volatile uint32_t tcnt0_ticks;


void init_tcnt0(void){
	/*	Ftimer = fclk / (2*N*(1+OCRn)) */
	
	
	TCNT0 = 0;
	OCR0A = 124;
	
	TCCR0A = (0<<COM0A0)|(0<<COM0A1)|(0<<COM0B0)|(0<<COM0B1);		// leave the functionality of I/O pins OC0A alone
	TCCR0A |= (1<<WGM01);											// Compare match to OCRA as Max
	TCCR0B = (1<<CS01)|(1<<CS00);									// 64 pre-scaler
	
	TIMSK |= (1<<OCIE0A);											// interrupt on compare match
	
	TIFR &= (1<<OCF0A);												// no comparison initially
	
}


uint32_t get_tcnt0_ticks(void) {
	/* internal reference clock, times how long the system has been on for	*/
	uint8_t interrupts_on = bit_is_set(SREG, SREG_I);
	cli();
	uint32_t return_value = tcnt0_ticks;
	if(interrupts_on) {
		sei();
	}
	return return_value;
}



ISR (TIMER0_COMPA_vect){
	
	tcnt0_ticks++;

}