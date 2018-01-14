/*
 * Servo.c
 *
 * Created: 25/05/2017 11:57:28 PM
 *  Author: Owner
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>			//enables interrupts
#include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
#include <stdio.h>					//input / output
#include <stdlib.h>					//standard function library; malloc / sort / rand etc
#include "Project.h"
static uint16_t dutyVal;

void init_servo(void){
	/*	set up servo motor PWM for 50Hz on OC1A pin		*/
	dutyVal = 19999;
	TCCR1A |= (1<<WGM11) | (1<<COM1A1) | (1<<COM1A0);		/* Inverted Mode */
	TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS11);		/* Fast PWM setup. Prescaler of 8 */
	ICR1 = dutyVal;		/* Maximum timer count */
}

void move_up(void) {
	/*	move the camera upwards an increment		*/
	uint16_t outputCom = OCR1A;
	if ((outputCom + 200) > (dutyVal - 800)){				// (20199-200) > 19199
		OCR1A = dutyVal - 800;								//	make equal 19199
	//} else if ((outputCom + 200) < (dutyVal - 2200)) {
	} else if ((outputCom + 200) < (dutyVal - 3000)) {
		OCR1A = dutyVal - 3000;
		//OCR1A = dutyVal - 2200;
	} else {
		OCR1A += 200;										// otherwise increase between 19199 -> 17799
	}
}

void move_down(void) {
	/* move the camera downwards	*/
	uint16_t outputCom = OCR1A;
	if ((outputCom - 200) > (dutyVal - 800)){				// (20199-200) > 19199
		OCR1A = dutyVal - 800;								//	make equal 19199
	//} else if ((outputCom - 200) < (dutyVal - 2200)) {
	} else if ((outputCom - 200) < (dutyVal - 3000)) {
		OCR1A = dutyVal - 3000;
	} else {
		OCR1A -= 200;										// otherwise decrease between 19199 -> 17799
	}
}