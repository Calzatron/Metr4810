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
#include "tcnt1.h"
#include "Project.h"
#include "serialio.h"

// Function Declarations
void step_clockwise(uint8_t time_on);
void step_anticlockwise(uint8_t time_on);
void step(info* info_ptr);


// Global variables
 int16_t steps;				// the current step position of the motor
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

	//BLU = PORTC2;
	//GRE = PORTC3;
	//YEL = PORTC4;
	//RED = PORTC5;

	DDRC |= (1<<BLU)|(1<<GRE)|(1<<YEL)|(1<<RED);
 }
 

 int16_t current_step(void){
	/* Returns the number of steps taken from the starting position	
	*	can be modified to return an angle from origin using gear
	*	ratio
	*/
	int16_t returnValue = steps;
	return returnValue;
 }

void hold_step(void){
	
	PORTC |= (1<<BLU)|(1<<GRE)|(1<<YEL)|(1<<RED);
	
}

void release_step(void){
	
	PORTC &= ~((1<<BLU)|(1<<GRE)|(1<<YEL)|(1<<RED));
	
}

 void step(info* info_ptr){
	/*	clockwise defined positive for speed
	*	will work out how fast to turn motor based on speed by setting
	*	time taken to move one step
	*	time_on is between -124 and +124
	*/
	//if ((steps > info_ptr->maxStep) || (steps < 0)){
		///*	dont move	*/
		//return;
	//}
	
	int8_t speed = info_ptr->stepSpeed;
	uint8_t time_on;
	if (speed >= 0){
			if (speed < 10){
				time_on = 100;
			} else if (speed < 20) {
				time_on = 90;
			} else if (speed < 30) {
				time_on = 80;
			} else if (speed < 40) {
				time_on = 70;
			} else if (speed < 50){
				time_on = 60;
			} else if (speed < 60) {
				time_on = 50;
			} else if (speed < 70){
				time_on = 40;
			} else if (speed < 80) {
				time_on = 30;
			} else if (speed < 90){
				time_on = 20;
			} else {
				time_on = 10;
			}
			step_clockwise(time_on);
	} else {
			if (speed > -10){
				time_on = 100;
			} else if (speed > -20) {
				time_on = 90;
			} else if (speed > -30) {
				time_on = 80;
			} else if (speed > -40) {
				time_on = 70;
			} else if (speed > -50){
				time_on = 60;
			} else if (speed > -60) {
				time_on = 50;
			} else if (speed > -70){
				time_on = 40;
			} else if (speed > -80) {
				time_on = 30;
			} else if (speed > -90){
				time_on = 20;
			} else {
				time_on = 10;
			}
			step_anticlockwise(time_on);
	}
 }

/**************************************************************************************swapped increment and decrement so start with claw closed ****************/
 void decrement_step(void){
	/* the motor has turned one step, increase the count */
	++steps;
 }

 void increment_step(void){
	/* the motor has turned back one step, decrease the count */
	--steps;
 }
/**************************************************************************************************************************************************************/


 void step_clockwise(uint8_t time_on){
	/* turns stepper motor clockwise */

	 uint32_t current_time;
	 
	 if(phase == 0){
		 current_time = get_tcnt1_ticks();
		 while((current_time + time_on) > get_tcnt1_ticks()){
			 PORTC |= (1<<GRE)|(1<<YEL);
			 PORTC &= ~((1<<BLU)|(1<<RED));
		 }
		 ++phase;
	 }
	 else if(phase == 1){
		 current_time = get_tcnt1_ticks();
		 while((current_time + time_on) > get_tcnt1_ticks()){
			 PORTC |= (1<<GRE)|(1<<RED);
			 PORTC &= ~((1<<BLU)|(1<<YEL));
		 }
		 ++phase;
	 }
	 else if(phase == 2){
		 current_time = get_tcnt1_ticks();
		 while((current_time + time_on) > get_tcnt1_ticks()){
			 PORTC |= (1<<BLU)|(1<<RED);
			 PORTC &= ~((1<<GRE)|(1<<YEL));
		 }
		 ++phase;
	 }
	 else if(phase == 3){
		 current_time = get_tcnt1_ticks();
		 while((current_time + time_on) > get_tcnt1_ticks()){
			 PORTC |= (1<<BLU)|(1<<YEL);
			 PORTC &= ~((1<<GRE)|(1<<RED));
		 }
		 phase = 0;
	 }
	 increment_step();
 }


 void step_anticlockwise(uint8_t time_on){
	/* turns stepper motor clockwise */
	
	 uint32_t current_time;

	 if(phase == 0){
		 current_time = get_tcnt1_ticks();
		 while((current_time + time_on) > get_tcnt1_ticks()){
			 PORTC |= (1<<BLU)|(1<<YEL);
			 PORTC &= ~((1<<GRE)|(1<<RED));
		 }
		 ++phase;
	 }
	 else if(phase == 1){
		 current_time = get_tcnt1_ticks();
		 while((current_time + time_on) > get_tcnt1_ticks()){
			 PORTC |= (1<<BLU)|(1<<RED);
			 PORTC &= ~((1<<GRE)|(1<<YEL));
		 }
		 ++phase;
	 }
	 else if(phase == 2){
		 current_time = get_tcnt1_ticks();
		 while((current_time + time_on) > get_tcnt1_ticks()){
			 PORTC |= (1<<GRE)|(1<<RED);
			 PORTC &= ~((1<<BLU)|(1<<YEL));
		 }
		 ++phase;
	 }
	 else if(phase == 3){
		 current_time = get_tcnt1_ticks();
		 while((current_time + time_on) > get_tcnt1_ticks()){
			 PORTC |= (1<<GRE)|(1<<YEL);
			 PORTC &= ~((1<<BLU)|(1<<RED));
		 }
		 phase = 0;
	 }
	 decrement_step();
 }