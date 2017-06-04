/*
 * CFile1.c
 *
 * Created: 17/04/2017 6:45:21 PM
 *  Author: s4357594
 */ 
 
 #include <avr/io.h>
 #include <avr/interrupt.h>			//enables interrupts
 #include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
 #include <stdio.h>					//input / output
 #include <stdlib.h>				//standard function library; malloc / sort / rand etc
#include "tcnt2.h"
	

void init_tcnt2(void){ 
   /*
   The below settings are for
   Timer Clock = CPU Clock (No Prescalling)
   Mode        = Fast PWM
   PWM Output  = Non Inverted

   */
	// set timer / counter
	
	TCNT2 = 0;
	OCR2A = 124;
	OCR2B = 124;
	
	TCCR2A |= _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20) | _BV(WGM21);
	TCCR2B |= _BV(CS20);
}
