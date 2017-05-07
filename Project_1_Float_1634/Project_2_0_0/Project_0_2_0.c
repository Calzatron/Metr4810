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

/*	set the UBRR0 register to BAUDREG (12 for 38.4k baudrate) */
#define F_CPU 8000000L		//Internal Calibrated RC Oscillator 8MHz
#define BAUDRATE 9600L			//from bluetooth datasheet38400
#define BAUDREG ((F_CPU)/(BAUDRATE*16UL)-1)

/*	 define pins for sensors	*/
#define TRIG0 PINC4
#define ECHO0 PINC5
#define TRIG1 PINA1
#define ECHO1 PINA2

typedef struct info {
	double sonicD0;
	double sonicD1;
} info;


void initialize(void);
info* makeInfo(void);
uint8_t sonic(info* info_ptr);
void do_sonic(info* info_ptr);

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
	
	/*	echo received data from 324P to blue-tooth	*/
	init_serial_stdio(9600,0);

	init_tcnt0();
	sei();
	srand(get_tcnt0_ticks());
	
	
}


info* makeInfo(void){
	/* initialize the game's information struct */
	info* info_ptr = malloc(sizeof(info));
	return info_ptr;
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


void do_sonic(info* info_ptr){
	/* performs moving average filter on sensor data
	*  updates stored values in struct, 0 if unsuccessful */
	double sonicDist0[3];
	double sonicDist1[3];
	uint8_t check[3];
	for (int i = 0; i < 3; i++){

		check[i] = sonic(info_ptr);
		sonicDist0[i] = info_ptr->sonicD0;
		sonicDist1[i] = info_ptr->sonicD1;

	}
	info_ptr->sonicD0 = 0;
	info_ptr->sonicD1 = 0;

	uint8_t count = 0;
	for (int j = 0; j < 3; j++){
		if(!check[j]){
			info_ptr->sonicD0 += sonicDist0[j];
			info_ptr->sonicD1 += sonicDist1[j];
			++count;
		}
	}
	if (count > 0){
		info_ptr->sonicD0 = info_ptr->sonicD0/count;
		info_ptr->sonicD1 = info_ptr->sonicD1/count;
	}
}


uint8_t sonic(info* info_ptr){
	/* sends the sensor readings through serial, returns 0 on successfully 
	*  retrieving data from sensor */
	uint32_t duration;
	double distance;
	uint8_t returnValue = 0;

	// sensor 0 first
	PORTC = (0<<TRIG0);
	custom_delay(400);
	PORTC = (1<<TRIG0);
	custom_delay(50);
	PORTC = (0<<TRIG0);
	
	//wait for the echo pin to go high, time the duration, calculate and transmit result
	uint32_t currentTime = get_tcnt0_ticks();
	uint32_t delay = 8000;
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
		duration = (get_tcnt0_ticks() - start);
		// multiply by the speed of sound and half for one way travel, then send to serial.
		distance = duration*3.4/2;

		char buffer[10];
		dtostrf(distance, 3, 5, buffer); fputs(buffer, stdout); fputc('\n', stdout);

		info_ptr->sonicD0 = distance;
	}
	
	
	// sensor 1 now
	PORTA = (0<<TRIG1);
	custom_delay(400);
	PORTA = (1<<TRIG1);
	custom_delay(50);
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
	while ((PINA & (1<<ECHO1)) && (returnValue != 2)){
		//wait for echo pin to go low, but don't hold for too long
		if (start + delay < get_tcnt0_ticks()){
			returnValue = 2;
		}
	}

	if (returnValue != 2){
		duration = (get_tcnt0_ticks() - start);
		// multiply by the speed of sound and half for one way travel, then send to serial.
		distance = duration*3.4/2;
		//output
		char buffer[10];
		dtostrf(distance, 3, 5, buffer); fputs(buffer, stdout); fputc('\n', stdout);

		info_ptr->sonicD1 = distance;
	}
	// all succeeded
	return 0;
}