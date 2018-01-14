/*
 * tcnt0.c
 *
 * Created: 17/04/2017 6:43:58 PM
 *  Author: s4357594
 */ 

 #include <avr/io.h>
 #include <avr/interrupt.h>			//enables interrupts
 #include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
 #include <stdio.h>					//input / output
 #include <stdlib.h>					//standard function library; malloc / sort / rand etc
 //#include <util/delay.h>
 #include "tcnt0.h"

 // counter incrementing on OCRA
 static volatile uint32_t tcnt0_ticks;


 void init_tcnt0(void){
	 /* initialises 8-bit timer to output compare at f(clk)/64 and OCRA @ 124 */
	 // set global counter
	 tcnt0_ticks = 0L;
	 // set timer / counter
	 TCNT0 = 0;
	 // set output compare resgister; value to execute ISR
	 OCR0A = 124;
	 // timer/counter control register A - set to clear when TCNT0 matches OCR0A
	 TCCR0A = (1<<WGM01);
	 // set the timer to update at a fraction of a clock cycle
	 TCCR0B = (1<<CS01)|(1<<CS00);	// set to f(clk) / 64
	 // timer/counter 1 interrupt mask register - enable OCR0A as output compare register, only works if OCF0A is set in TIFR0
	 TIMSK0 |= (1<<OCIE0A);
	 // if it isn't already, clear the interrupt output compare flag by writing a 1 to OCF0A; switches when TCNT0 matches OCR0A
	 TIFR0 &= (1<<OCF0A);
 }


 uint32_t get_tcnt0_ticks(void) {
	 /* internal reference clock, times how long the system has been on for	*/
	 uint8_t interrupts_on = bit_is_set(SREG, SREG_I);
	 cli();
	 uint32_t return_value = tcnt0_ticks;
	 if(interrupts_on) {
		 sei();
	 }
	 return return_value;
 }


 ISR(TIMER0_COMPA_vect) {
	 /* Increment our clock tick count, check if pin value has changed */
	 tcnt0_ticks++;
 }








///*
 //* tcnt1.c
 //*
 //* Created: 3/05/2017 11:13:59 PM
 //*  Author: Owner
 //*/ 
 //#include <avr/io.h>
 //#include <avr/interrupt.h>			//enables interrupts
 //#include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
 //#include <stdio.h>					//input / output
 //#include <stdlib.h>					//standard function library; malloc / sort / rand etc
 //#include "tcnt0.h"
//
//void init_tcnt0(void){ 
   ///*
   //TCCR2A - Timer Counter Control Register (TIMER/COUNTER2)
   //-----------------------------------------------
   //BITS DESCRIPTION
   //NO:   NAME   DESCRIPTION
   //--------------------------
   //BIT 7 : COM2A1   Compare Output Mode for Channel A [n = 1:0] [Set to 1]
   //BIT 6 : COM2A0	"	[Set to 0]
   //BIT 5 : COM2B1	"	[Set to 1]
   //BIT 4 : COM2B0	"	[Set to 0]
//
   //BIT 3 :
   //BIT 2 :
   //BIT 1 : WGM21   Waveform Generation Mode [n = 1:0]	[SET to 1]
   //BIT 0 : WGM20   "	[SET to 1]
//
//
    //TCCR2B - TC2 Control Register B
    //-----------------------------------------------
    //BITS DESCRIPTION
    //NO:   NAME   DESCRIPTION
    //--------------------------
    //BIT 7 : FOC2A   
    //BIT 6 : FOC2B	
    //BIT 5 : COM2B1	"	[Set to 1]
    //BIT 4 : COM2B0	"	[Set to 0]
//
    //BIT 3 : WGM22	Waveform Generation Mode	[Set to 1]
    //BIT 2 : CS22	Clock Select 2 [n = 0..2] [Set to 1]
    //BIT 1 : CS11   "	[SET to 0]
    //BIT 0 : CS00   "	[SET to 0]  
//
   //--------------------------
   //The above settings are for
   //Timer Clock = CPU Clock (No Prescalling)
   //Mode        = Fast PWM
   //PWM Output  = Non Inverted
//
   //*/
	//// set timer / counter
	//
	//TCNT0 = 0;
	//OCR0A = 0;
	////OCR0B = 124;
	//
//
	//TCCR0A |= (1<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | _BV(WGM00) | _BV(WGM01);
	//TCCR0B |= (1<<CS10);
//
//}
