/*
 * worm.c
 *
 * Created: 22/05/2017 4:31:02 AM
 *  Author: Callum
 */ 
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>			//enables interrupts
#include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
#include <stdio.h>					//input / output
#include <stdlib.h>					//standard function library; malloc / sort / rand etc

// Custom Libraries
#include "Project.h"
#include "serialio.h" 

/*	define ports to be used for winch here, but check stop command pins as well in main
*	to ensure the right pins are getting turned off	*/
#define RED_P PORTC3
#define BLACK_P PORTC4

void pivot_off(void){
		/*	turn the motor off	*/
		if (PORTC & (1<<RED_P)){
			PORTC &= ~(1<<RED_P);
		}
		if (PORTC & (1<<BLACK_P)){
			PORTC &= ~(1<<BLACK_P);
		}
		
		fputs("Pivot motor off\n", stdout);
}

void move_pivot(int8_t direction){
	/*	actuates DC motor to tilt the submersible */
	if (direction){
			// forward
			PORTC |= (1<<RED_P);
			PORTC &= ~(1<<BLACK_P);
	} else {
			// backwards
			PORTC |= (1<<BLACK_P);
			PORTC &= ~(1<<RED_P);
	}
}
