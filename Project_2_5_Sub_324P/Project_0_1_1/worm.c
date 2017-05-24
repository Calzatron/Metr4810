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
#include "Stepper.h"
#include "tcnt1.h"
#include "Project.h"
#include "serialio.h" 


/*	define ports to be used for winch here, but check stop command pins as well in main
*	to ensure the right pins are getting turned off	*/
#define RED PORTC5
#define BLACK PORTC2

void worm_off(void){
		/*	turn the motor off	*/
		PORTC &= ~(1<<RED);
		PORTC &= ~(1<<BLACK);
		fputs("Worm off\n", stdout);
		
}


void move_worm(uint8_t percent, int8_t direction){
	/*	effective pwm to control DC motor for the claw */
	
	uint8_t duty = 20;
	
		if (direction){
				// forward
				PORTC |= (1<<RED);
				PORTC &= ~(1<<BLACK);
				custom_delay((duty*percent)/100);
				PORTC &= ~(1<<RED);
		} else {
				// backwards
				PORTC |= (1<<BLACK);
				PORTC &= ~(1<<RED);
				custom_delay((duty*percent)/100);
				PORTC &= ~(1<<RED);
			
		}
		
}
