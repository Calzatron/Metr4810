/*
 * tcnt2.c
 *
 * Created: 1/04/2017 2:59:59 PM
 *  Author: Ryan
 */ 

 #include <avr/io.h>
 #include <avr/interrupt.h>			//enables interrupts
 #include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
 #include <stdio.h>					//input / output
 #include <stdlib.h>					//standard function library; malloc / sort / rand etc
 //#include <util/delay.h>
 //#include <string.h>
#include "tcnt2.h"
	

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
	// set timer / counter
	
	TCNT2 = 0;
	OCR2A = 160;
	OCR2B = 120;
	
	DDRD |= (1<<PORTD7)|(1<<PORTD6);
	
	//TCCR2A |= (1<<COM2A1) | (1<<WGM20);
	//TCCR2B |= (1<<CS20) | (1<<WGM22);
	TCCR2A |= _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20) | _BV(WGM21);
	TCCR2B |= _BV(CS20);
	
	//PRR0 |= (1<<PRTIM2);
	//TCCR2A |= (1<<COM2A1)|(1<<COM2B1)|(1<<WGM21)|(1<<WGM20);
	//TCCR2B = 0b00001100;
	
	
	
	// Use OCR2A to set TOP for PWM
}


