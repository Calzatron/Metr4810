/*
 * tcnt1.c
 *
 * Created: 7/05/2017 12:59:35 AM
 *  Author: s4357594
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>			//enables interrupts
#include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
#include <stdio.h>					//input / output
#include <stdlib.h>


 static volatile int i = 0;			//  integer  to access all though the program
 static volatile uint16_t pulse = 0;		//  integer  to access all though the program


 uint16_t get_pulse(void){
	uint16_t returnVal;
	returnVal = pulse;
	return returnVal;

 }



 ISR(INT0_vect)//interrupt service routine when there is a change in logic level

 {

	 if (i==1)//when logic from HIGH to LOW

	 {

		 TCCR1B=0;//disabling counter

		 pulse=TCNT1;//count memory is updated to integer

		 TCNT1=0;//resetting the counter memory

		 i=0;

	 }

	 if (i==0){//when logic change from LOW to HIGH

		 TCCR1B|=(1<<CS10);//enabling counter

		 i=1;

	 }

 }