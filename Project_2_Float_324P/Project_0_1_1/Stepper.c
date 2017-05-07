/*
 * Stepper.c
 *
 * Created: 17/04/2017 6:42:29 PM
 *  Author: s4357594
 */ 

 // C libraries
#include <avr/io.h>
#include <avr/interrupt.h>			//enables interrupts
#include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
#include <stdio.h>					//input / output
#include <stdlib.h>					//standard function library; malloc / sort / rand etc

// Custom Libraries
#include "Stepper.h"
#include "tcnt0.h"

// Function Declarations
void step_clockwise(uint8_t time_on);
void step_anticlockwise(uint8_t time_on);
void step(int8_t speed);


// Global variables
 uint8_t steps;				// the current step position of the motor
 uint8_t phase;				// for controlling position in step

 uint8_t BLU;				// port for stepper blue wire
 uint8_t GRE;				// port for stepper green wire
 uint8_t YEL;				// port for stepper yellow wire
 uint8_t RED;				// port for stepper red wire


 void init_step(void){
	/* sets the ports for the stepper motor and initializes the steps and phase	*/
	steps = 0;
	phase = 0;

	BLU = PORTC4;
	GRE = PORTC5;
	YEL = PORTC2;
	RED = PORTC3;

	DDRC |= (1<<BLU)|(1<<GRE)|(1<<YEL)|(1<<RED);
 }

 uint8_t current_step(void){
	/* Returns the number of steps taken from the starting position	
	*	can be modified to return an angle from origin using gear
	*	ratio
	*/
	uint8_t returnValue = steps;
	return returnValue;
 }



 void step(int8_t speed){
	/*	clockwise defined positive for speed
	*	will work out how fast to turn motor based on speed by setting
	*	time taken to move one step
	*	time_on is between -124 and +124
	*/
	uint8_t time_on = 0;
	if (speed >= 0){
		if (speed < 30){
			 time_on = 15;
		} else if (speed < 60) {
			time_on = 10;
		} else if (speed < 90){
			time_on = 5;
		} else {
			time_on = 2;
		}
		step_clockwise(time_on);
	} else {
		if (speed > -30){
			time_on = 15;
		} else if (speed > -60) {
			time_on = 10;
		} else if (speed > -90){
			time_on = 5;
		} else {
			time_on = 2;
		}
		step_anticlockwise(time_on);
	}
 }

 void increment_step(void){
	/* the motor has turned one step, increase the count */
	++steps;
 }

 void decrement_step(void){
	/* the motor has turned back one step, decrease the count */
	--steps;
 }

 void step_clockwise(uint8_t time_on){
	/* turns stepper motor clockwise */

	 uint32_t current_time;
	 
	 if(phase == 0){
		 current_time = get_tcnt0_ticks();
		 while((current_time + time_on) > get_tcnt0_ticks()){
			 PORTC = (0<<BLU)|(1<<GRE)|(1<<YEL)|(0<<RED);
		 }
		 ++phase;
	 }
	 else if(phase == 1){
		 current_time = get_tcnt0_ticks();
		 while((current_time + time_on) > get_tcnt0_ticks()){
			 PORTC = (0<<BLU)|(1<<GRE)|(0<<YEL)|(1<<RED);
		 }
		 ++phase;
	 }
	 else if(phase == 2){
		 current_time = get_tcnt0_ticks();
		 while((current_time + time_on) > get_tcnt0_ticks()){
			 PORTC = (1<<BLU)|(0<<GRE)|(0<<YEL)|(1<<RED);
		 }
		 ++phase;
	 }
	 else if(phase == 3){
		 current_time = get_tcnt0_ticks();
		 while((current_time + time_on) > get_tcnt0_ticks()){
			 PORTC = (1<<BLU)|(0<<GRE)|(1<<YEL)|(0<<RED);
		 }
		 phase = 0;
	 }
 }


 void step_anticlockwise(uint8_t time_on){
	/* turns stepper motor clockwise */
	
	 uint32_t current_time;

	 if(phase == 0){
		 current_time = get_tcnt0_ticks();
		 while((current_time + time_on) > get_tcnt0_ticks()){
			 PORTC = (1<<BLU)|(0<<GRE)|(1<<YEL)|(0<<RED);
		 }
		 ++phase;
	 }
	 else if(phase == 1){
		 current_time = get_tcnt0_ticks();
		 while((current_time + time_on) > get_tcnt0_ticks()){
			 PORTC = (1<<BLU)|(0<<GRE)|(0<<YEL)|(1<<RED);
		 }
		 ++phase;
	 }
	 else if(phase == 2){
		 current_time = get_tcnt0_ticks();
		 while((current_time + time_on) > get_tcnt0_ticks()){
			 PORTC = (0<<BLU)|(1<<GRE)|(0<<YEL)|(1<<RED);
		 }
		 ++phase;
	 }
	 else if(phase == 3){
		 current_time = get_tcnt0_ticks();
		 while((current_time + time_on) > get_tcnt0_ticks()){
			 PORTC = (0<<BLU)|(1<<GRE)|(1<<YEL)|(0<<RED);
		 }
		 phase = 0;
	 }
 }