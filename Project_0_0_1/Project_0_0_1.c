/*
 * Project_0_0_1.c
 *
 * Created: 20/03/2017 5:50:49 PM
 *  Author: Callum Rohweder
 */ 

/*	standard libraries	*/
#include <avr/io.h>
#include <avr/interrupt.h>			//enables interrupts
#include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
#include <stdio.h>					//input / output
#include <stdlib.h>					//standard function library; malloc / sort / rand etc
#include <util/delay.h>

/*	custom libraries	*/
#include "tcnt0.h"
#include "Motors.h"
/*	function declarations	*/
void initialise(void);
void custom_delay(uint32_t ticks);
void Sensor_input(void);

uint8_t phase;


#define F_CPU 8000000L
#define BAUD 96900
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

/* functional code	*/
int main(void)
{
	initialise();
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}


void initialise(void){
	
	init_tcnt0();
	//set Global Interrupt Enable flag
	sei();
	srand(get_tcnt0_ticks());

	USART_init(BAUDRATE);
	
	init_step();
	// set pins C to input; default
	DDRC = 0x00;
	
	while(!get_button_()){; /* wait to be turned on by PINC5 */}

	// initialise constants
	phase = 0;
	
}

/*void USART_init(unsigned int ubrr){
	// UBRR0 is split into two; store the high 4 bits of the baud rate 
	// in UBRR0H and then rest in UBRR0L
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)(ubrr);

	UCSR0B |= (1<<RXEN0);
	UCSR0B |= (1<<TXCIE0);
	UCSR0A |= (1<<TXC);

	UCSR0C = (1<<USBS0)|(3<<UCSZ00);

}*/


void custom_delay(uint32_t ticks){
	uint32_t current_time;
	current_time = get_tcnt0_ticks();
	while((current_time + ticks) > get_tcnt0_ticks()){
		//wait
	}
}

void step_clockwise(void){
	DDRD = 0xFF;
	uint32_t current_time;
	uint8_t time_on = 100;

	if(phase = 0){
		current_time = get_tcnt0_ticks();
		while((current_time + time_on) > get_tcnt0_ticks()){
			PORTD = (1<<PORTD2)|(0<<PORTD3)|(0<<PORTD4)|(0<<PORTD5);
		}
		++phase;
	}
	else if(phase = 1){
		current_time = get_tcnt0_ticks();
		while((current_time + time_on) > get_tcnt0_ticks()){
			PORTD = (0<<PORTD2)|(1<<PORTD3)|(0<<PORTD4)|(0<<PORTD5);
		}
		++phase;
	}
	else if(phase = 2){
		current_time = get_tcnt0_ticks();
		while((current_time + time_on) > get_tcnt0_ticks()){
			PORTD = (0<<PORTD2)|(0<<PORTD3)|(1<<PORTD4)|(0<<PORTD5);
		}
		++phase;
	}
	else if(phase = 3){
		current_time = get_tcnt0_ticks();
		while((current_time + time_on) > get_tcnt0_ticks()){
			PORTD = (0<<PORTD2)|(0<<PORTD3)|(0<<PORTD4)|(1<<PORTD5);
		}
		phase = 0;
	}
}


void step_clockwise(void){
	DDRD = 0xFF;
	uint32_t current_time;
	uint8_t time_on = 100;

	if(phase = 0){
		current_time = get_tcnt0_ticks();
		while((current_time + time_on) > get_tcnt0_ticks()){
			PORTD = (1<<PORTD2)|(0<<PORTD3)|(0<<PORTD4)|(0<<PORTD5);
		}
		++phase;
	}
	else if(phase = 1){
		current_time = get_tcnt0_ticks();
		while((current_time + time_on) > get_tcnt0_ticks()){
			PORTD = (0<<PORTD2)|(1<<PORTD3)|(0<<PORTD4)|(0<<PORTD5);
		}
		++phase;
	}
	else if(phase = 2){
		current_time = get_tcnt0_ticks();
		while((current_time + time_on) > get_tcnt0_ticks()){
			PORTD = (0<<PORTD2)|(0<<PORTD3)|(1<<PORTD4)|(0<<PORTD5);
		}
		++phase;
	}
	else if(phase = 3){
		current_time = get_tcnt0_ticks();
		while((current_time + time_on) > get_tcnt0_ticks()){
			PORTD = (0<<PORTD2)|(0<<PORTD3)|(0<<PORTD4)|(1<<PORTD5);
		}
		phase = 0;
	}
}



void sensor_input(void){}
//{
	//uint16_t value;
	//DDRC = 0x3C;		// 00111100
	//DDRD = 0x80;		// 10000000
	///* Set up the serial port for stdin communication at 19200 baud, no echo */
	//// Set up ADC - AVCC reference, right adjust
	//// Input selection doesn't matter yet - we'll swap this around in the while
	//// loop below.d
	//ADMUX = (1<<REFS0); //set's compare to voltage, and default ADC0 -> PIN23
	//// Turn on the ADC (but don't start a conversion yet). Choose a clock
	//// divider of 16. (The ADC clock must be somewhere
	//// between 50kHz and 200kHz. We will divide our 1MHz clock by 64
	//// to give us 62.5kHz.)
	//ADMUX |= (1<<MUX1);			/*											ADDED THIS, MAKES ADC2 INPUT (PIN25, PC2); do a check*/
	//ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);
	//while(button1 & (1<<PORTB3)) {
		//button_test();
		//ADCSRA |= (1<<ADSC);
		//while(ADCSRA & (1<<ADSC)) {
			//; /* Wait until conversion finished */
		//}
		//value = ADC;
		//if (value < 409.6){PORTC = (1<<PORTC4);}
		//if (value > 409.6 && value < 614.4){PORTC = (1<<PORTC5);}
		//else {PORTD = (1<<PORTD7);}
	//}
//}