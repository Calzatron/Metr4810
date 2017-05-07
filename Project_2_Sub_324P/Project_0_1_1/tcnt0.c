/*
 * tcnt1.c
 *
 * Created: 3/05/2017 11:13:59 PM
 *  Author: Owner
 */ 
 #include <avr/io.h>
 #include <avr/interrupt.h>			//enables interrupts
 #include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
 #include <stdio.h>					//input / output
 #include <stdlib.h>					//standard function library; malloc / sort / rand etc
 #include "tcnt0.h"

void init_tcnt0(void){ 
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
	
	TCNT0 = 0;
	OCR0A = 0;
	//OCR0B = 124;
	

	TCCR0A |= (1<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | _BV(WGM00) | _BV(WGM01);
	TCCR0B |= (1<<CS10);

}
