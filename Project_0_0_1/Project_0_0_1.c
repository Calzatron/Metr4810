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
//#include <util/delay.h>

/*	custom libraries	*/
#include "tcnt0.h"
#include "Motors.h"
#include "USART_Receiver.h"

/*	function declarations	*/
void initialise(void);
void custom_delay(uint32_t ticks);
void Sensor_input(void);
void step_clockwise(void);
void step_clockwise(void);



/*	set the UBRR0 register to BAUDREG (12 for 38.4k baudrate) */
#define F_CPU 8000000L		//Internal Calibrated RC Oscillator 8MHz
#define BAUDRATE 38400L			//from bluetooth datasheet
#define BAUDREG ((F_CPU)/(BAUDRATE*16UL)-1) 



/* functional code	*/
int main(void)
{
	//initialise();
	
	initialise();
    while(1)
    {
		
		
        //TODO:: Please write your application code 
    }
}


void initialise(void){
	

	DDRB = 0xFF;
	DDRA = 0xFF;

	DDRD |= (1<<PORTD7)|(1<<PORTD6);
	DDRC = 0xFF;

	init_tcnt0();
	//set Global Interrupt Enable flag
	sei();
	srand(get_tcnt0_ticks());

	USART_init(BAUDREG);
	
	//init_step();


	while(1){


		if(get_button_() == 0){
				PORTB = (1<<PORTB1);
				PORTA = (0<<PORTA0);
		}

		else{
				PORTB = (0<<PORTB1);
				PORTA = (1<<PORTA0);
		}
		
	}

	
}




void custom_delay(uint32_t ticks){
	uint32_t current_time;
	current_time = get_tcnt0_ticks();
	while((current_time + ticks) > get_tcnt0_ticks()){
		//wait
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