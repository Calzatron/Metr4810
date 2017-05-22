/*
 * tcnt0.c
 *
 * Created: 17/04/2017 6:43:58 PM
 *  Author: s4357594
 */ 
 #include <avr/io.h>
 #include <avr/interrupt.h>			//enables interrupts
 #include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
 #include <stdio.h>					//input / output
 #include <stdlib.h>					//standard function library; malloc / sort / rand etc
 //#include <util/delay.h>
 #include "tcnt1.h"

 // counter incrementing on OCRA
 static volatile uint32_t tcnt1_ticks;

 // variables used for checking if button is pressed
 static volatile uint8_t Button_on;

 void init_tcnt1(void){
	 /* initialises 8-bit timer to output compare at f(clk)/64 and OCRA @ 124 */
	 
	 //set button to be not pushed
	 Button_on = 0;
	 
	 // set global counter
	 tcnt1_ticks = 0L;
	 
	 // set timer / counter
	 TCNT1 = 0;
	 
	 // set output compare resgister; value to execute ISR
	 OCR1A = 124; // between 1 and 255
	 
	 // timer/counter control register A - set to clear when TCNT0 matches OCR0A
	 TCCR1A = (1<<WGM11);
	 
	 // set the timer to update at a fraction of a clock cycle
	 TCCR1B = (1<<CS11)|(1<<CS10);	// set to f(clk) / 64
	 
	 // timer/counter 1 interrupt mask register - enable OCR0A as output compare register, only works if OCF0A is set in TIFR0
	 TIMSK1 |= (1<<OCIE1A);
	 
	 // if it isn't already, clear the interrupt output compare flag by writing a 1 to OCF0A; switches when TCNT0 matches OCR0A
	 TIFR1 &= (1<<OCF1A);
 }


 uint32_t get_tcnt1_ticks(void) {
	 /* internal reference clock, times how long the system has been on for	*/

	 uint8_t interrupts_on = bit_is_set(SREG, SREG_I);
	 cli();
	 uint32_t return_value = tcnt1_ticks;
	 if(interrupts_on) {
		 sei();
	 }
	 return return_value;
 }



 ISR(TIMER1_COMPA_vect) {
	 /* Increment our clock tick count, check if pin value has changed */
	 
	 tcnt1_ticks++;

 }