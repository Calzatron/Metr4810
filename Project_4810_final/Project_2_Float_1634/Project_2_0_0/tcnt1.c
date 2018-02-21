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
 #include "tcnt1.h"


void pwm_initialiser(void){ 
	// set timer / counter
	TCNT1 = 0;
	OCR1A = 0;
	OCR1B = 0;
	//Set Initial Timer value
	TCNT1=0;
	//set non inverted PWM on OC1A pin
	//and inverted on OC1B
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<COM1B0);
	//set top value to ICR1
	ICR1 = 0x00FF;
	//set correct phase and frequency PWM mode
	TCCR1B|=(1<<WGM13);
	TCCR1B|=(1<<CS11)|(1<<CS10);
	OCR1A = 0x00FF;
	OCR1B = 0x00FF;
	// Use OCR1A to set TOP for PWM
}
