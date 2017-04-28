/*
 * Project_2_0_0.c
 *
 * Created: 17/04/2017 7:48:32 PM
 * Author : s4357594
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>			//enables interrupts
#include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
#include <stdio.h>					//input / output
#include <stdlib.h>					//standard function library; malloc / sort / rand etc

#include "tcnt0.h"
#include "USART0_.h"

void initialize(void);

/*	set the UBRR0 register to BAUDREG (12 for 38.4k baudrate) */
#define F_CPU 8000000L		//Internal Calibrated RC Oscillator 8MHz
#define BAUDRATE 9600L			//from bluetooth datasheet38400
#define BAUDREG ((F_CPU)/(BAUDRATE*16UL)-1)

/*	 define pins for sensors	*/
#define TRIG0 PINC4
#define ECHO0 PINC5
#define TRIG1 PINA1
#define ECHO1 PINA2


/*
	8MHz clock

	two USARTS
	12 ADC
	2 timers with 2 output compare registers each

	PORTA3 -> PORTC2 == ADC
	PORTA7 && PORTB0 == RXD0 && TXD0
	PORTB1 && PORTB2 == RXD1 && TXD1
	PORTC0 && PORTA5 == OCOA && OCOB
	PORTB3 && PORTA6 == OC1A && OC1B

*/


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}


void initialize(void){
	
	DDRC = (1<<TRIG0)|(0<<ECHO0);
	DDRA = (1<<TRIG1)|(0<<ECHO1);
	
	init_tcnt0();
	sei();
	srand(get_tcnt0_ticks());
	
	
}


void custom_delay(uint32_t ticks){
	/*	Custom delay function, waits for timer to change by 'ticks'
	*	note that tcnt0 updates every 0.002 seconds */
	
	uint32_t current_time;
	current_time = get_tcnt0_ticks();
	while((current_time + ticks) > get_tcnt0_ticks()){
		//wait
	}
}


uint8_t sense(void){
	/* sends the sensor readings through serial, returns 0 on successfully 
	*  retreiving data from sensor */
	uint32_t duration;
	uint8_t distance;
	uint8_t returnValue = 0;
	// sensor 0 first
	PORTC = (0<<TRIG0);
	custom_delay(10);
	PORTC = (1<<TRIG0);
	custom_delay(10);
	PORTC = (0<<TRIG0);
	
	//wait for the echo pin to go high, time the duration, calculate and transmit result
	uint32_t currentTime = get_tcnt0_ticks();
	uint32_t delay = 1000;
	while ((PINC & (1<<ECHO0)) == 0x00){
		//wait for echo pin to go high, but don't hold for too long
		if (currentTime + delay < get_tcnt0_ticks()){
			returnValue = 1;
		}
	}
	uint32_t start = get_tcnt0_ticks();
	while ((PINC & (1<<ECHO0)) && !(returnValue)){
		//wait for echo pin to go low, but don't hold for too long
		if (start + delay < get_tcnt0_ticks()){
			returnValue = 1;
		}
	}
	
	if (!returnValue){
		duration = get_tcnt0_ticks() - start;
		// multiply by the speed of sound and half for one way travel, then send to serial.
		distance = duration*0.034/2;
		output_char(distance);
	}
	
	
	// sensor 1 now
	PORTA = (0<<TRIG1);
	custom_delay(10);
	PORTA = (1<<TRIG1);
	custom_delay(10);
	PORTA = (0<<TRIG1);
	
	//wait for the echo pin to go high, time the duration, calculate and transmit result
	currentTime = get_tcnt0_ticks();
	while ((PINA & (1<<ECHO1)) == 0x00){
		//wait for echo pin to go high, but don't hold for too long
		if (currentTime + delay < get_tcnt0_ticks()){
			returnValue = 2;
		}
	}
	start = get_tcnt0_ticks();
	while ((PINA & (1<<ECHO0)) && (returnValue != 2)){
		//wait for echo pin to go low, but don't hold for too long
		if (start + delay < get_tcnt0_ticks()){
			returnValue = 2;
		}
	}
	
	if (returnValue != 2){
		duration = get_tcnt0_ticks() - start;
		// multiply by the speed of sound and half for one way travel, then send to serial.
		distance = duration*0.034/2;
		output_char("-");
		output_char(distance);
	}
	output_char("\n");
	// all succeeded
	return 0;
}