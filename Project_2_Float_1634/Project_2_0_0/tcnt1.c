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
 #include <stdlib.h>					//standard function library; malloc / sort / rand etc
#include "tcnt1.h"


void pwm_initialiser(void){ 
   /*
   TCCR2A - Timer Counter Control Register (TIMER/COUNTER2)
   -----------------------------------------------
   BITS DESCRIPTION
   NO:   NAME   DESCRIPTION
   --------------------------
   BIT 7 : COM2A1   Compare Output Mode for Channel A [n = 1:0] [Set to 1]
   BIT 6 : COM2A0	"	[Set to 0]
   BIT 5 : COM2B1	"	[Set to 1]
   BIT 4 : COM2B0	"	[Set to 0]

   BIT 3 :
   BIT 2 :
   BIT 1 : WGM21   Waveform Generation Mode [n = 1:0]	[SET to 1]
   BIT 0 : WGM20   "	[SET to 1]


    TCCR2B - TC2 Control Register B
    -----------------------------------------------
    BITS DESCRIPTION
    NO:   NAME   DESCRIPTION
    --------------------------
    BIT 7 : FOC2A   
    BIT 6 : FOC2B	
    BIT 5 : COM2B1	"	[Set to 1]
    BIT 4 : COM2B0	"	[Set to 0]

    BIT 3 : WGM22	Waveform Generation Mode	[Set to 1]
    BIT 2 : CS22	Clock Select 2 [n = 0..2] [Set to 1]
    BIT 1 : CS11   "	[SET to 0]
    BIT 0 : CS00   "	[SET to 0]  

   --------------------------
   The above settings are for
   Timer Clock = CPU Clock (No Prescalling)
   Mode        = Fast PWM
   PWM Output  = Non Inverted

*/
   
   
   
//DDRB=0x07;         // set OC1A pin output (among others)

// Timer 1
//TCNT1=0;           // clear counter
//ICR1=36363;        // 439.9956 Hz from 16 MHz clock
//
//TCCR1A= 0b10000011; // non-inverting, fast PWM
//TCCR1B= 0b00011001; // fast PWM, full speed
//
//OCR1A= 0;         // 1 % strobe



   
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
		//set corrcet phase and frequency PWM mode
	TCCR1B|=(1<<WGM13);
	TCCR1B|=(1<<CS11)|(1<<CS10);
	//TCCR2A |= (1<<COM2A1) | (1<<WGM20);
	//TCCR2B |= (1<<CS20) | (1<<WGM22);
	//TCCR1A |= (1<<COM1A1) | (1<<COM1B1) | (1<<WGM10);
	//TCCR1B |= (1<<CS10) | (1<<WGM12);
	
	//PRR0 |= (1<<PRTIM2);
	//TCCR2A |= (1<<COM2A1)|(1<<COM2B1)|(1<<WGM21)|(1<<WGM20);
	//TCCR2B = 0b00001100;
	
	OCR1A = 0x00FF;
	OCR1B = 0x00FF;
	
	// Use OCR1A to set TOP for PWM
}
