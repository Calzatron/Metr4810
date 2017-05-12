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

/*	include custom libraries	*/
#include "tcnt0.h"
#include "tcnt1.h"
#include "serialio.h"


/*	set the UBRR0 register to BAUDREG (12 for 38.4k baudrate) */
#define F_CPU 8000000L		//Internal Calibrated RC Oscillator 8MHz
#define BAUDRATE 9600L			//from bluetooth datasheet38400
#define BAUDREG ((F_CPU)/(BAUDRATE*16UL)-1)

/*	 define pins for sensors	*/
#define TRIG0 PINC4
#define ECHO0 PINC5
#define TRIG1 PINA1
#define ECHO1 PINA2

#define FAST 1

typedef struct info {
	double sonicD0;
	double sonicD1;
} info;


/*	function declarations	*/
info* makeInfo(void);
void initialise(info* info_ptr);
uint8_t sonic(info* info_ptr);
void do_sonic(info* info_ptr);
void custom_delay(uint32_t ticks);

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


int main(void) {

	info* info_ptr = makeInfo();
	initialise(info_ptr);
	//uint16_t count = 0;
	while(1){
		if (serial_input_available()){
			char buffer[20];
			char in = fgetc(stdin);
			if (in == '+'){
				/*	winch up	*/
				PORTC |= (1<<PORTC0);
				PORTC &= ~(1<<PORTC1);
				OCR1A = 250;
				sprintf(buffer, "lifting\n");
				} else if (in == '_'){
				/*	winch down	*/
				OCR1A = 250;
				PORTC |= (1<<PORTC1);
				PORTC &= ~(1<<PORTC0);
				sprintf(buffer, "lowering\n");
				} else if (in == ' '){
				/*	stop operation	*/
				PORTC = 0x00;
				OCR1A = 0;
				//fputc('?', stdout);
				} else if (in == '?'){
				sprintf(buffer, "sensing\n");
				do_sonic(info_ptr);
				char buffer0[20];
				dtostrf(info_ptr->sonicD0, 3, 5, buffer0); fputs("Sensor0: ", stdout); fputs(buffer0, stdout); fputc('	', stdout);
				char buffer1[20];
				dtostrf(info_ptr->sonicD1, 3, 5, buffer1); fputs("Sensor1: ", stdout); fputs(buffer1, stdout); fputc('	', stdout);
				char buffer3[10];
				uint8_t timer = OCR0A;
				fputs("timer: ", stdout);
				ltoa(timer, buffer3, 10); fputs(buffer3, stdout); fputc('\n',stdout);
				} else if (in == 'H'){
				PORTA &= ~(1<<PORTA5);
				custom_delay(1000);
				PORTA |= (1<<PORTA5);
			}
			/*	echo characters back to terminal	*/
			if ((in != '_') && (in != '=') && (in != 'S')){
				fputc(in, stdout);
			}
		}
	}
}


void initialise(info* info_ptr){
	
	
	/* sensor pins	*/
	DDRC |= (1<<TRIG0);
	DDRC &= ~(1<<ECHO0);
	
	DDRA |= (1<<TRIG1);
	DDRA &= ~(1<<ECHO1);

	/* transmission pins */
	DDRB |= (1<<PORTB0);
	DDRA &= ~(1<<PINA7);

	/*	Motor pins */
	DDRA |= (1<<PORTA6)|(1<<PORTA3)|(1<<PORTA4);
	DDRB |= (1<<PORTB3);
	DDRC |= (1<<PORTC1)|(1<<PORTC2);
	
	/* initialise timers / pwm */
	pwm_initialiser();				/* initially OCR2A = 0 */
	init_tcnt0(!FAST);
	init_serial_stdio(9600,0);

	/* set Global Interrupt Enable flag */
	srand(get_tcnt0_ticks());
	sei();

	custom_delay(100);

	/* wait for communication to start from host */
	custom_delay(100);
	uint8_t check = 0;
	while (check < 6){
		if (serial_input_available()){
			char in = fgetc(stdin);
			fputc(in, stdout);
			if (in == '\n'){
				check = 10;
				break;
			}
			++check;
		}
	}
	custom_delay(1000);
	fputs("Float ready\n", stdout);
	

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
		;
	}
}


void do_sonic(info* info_ptr){
	/* performs moving average filter on sensor data
	*  updates stored values in struct, 0 if unsuccessful */
	
	/*	speed up clock for sensor readings */
	uint8_t interrupts_on = bit_is_set(SREG, SREG_I);
	cli();
	init_tcnt0(FAST);
	if(interrupts_on) {
		sei();
	}
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
		
	/*	continue normal operation */
	interrupts_on = bit_is_set(SREG, SREG_I);
	cli();
	init_tcnt0(!FAST);
	if(interrupts_on) {
		sei();
	}
}

uint8_t sonic(info* info_ptr){
	/* sends the sensor readings through serial, returns 0 on successfully 
	*  retrieving data from sensor */
	uint32_t duration;
	double distance;
	uint8_t returnValue = 0;

	// sensor 0 first
	PORTC &= ~(1<<TRIG0);
	custom_delay(400);
	PORTC |= (1<<TRIG0);
	custom_delay(50);
	PORTC &= ~(1<<TRIG0);
	
	//wait for the echo pin to go high, time the duration, calculate and transmit result
	uint32_t currentTime = get_tcnt0_ticks();
	uint32_t delay = 800;
	while ((PINC & (1<<ECHO0)) == 0x00){
		//wait for echo pin to go high, but don't hold for too long
		if (currentTime + delay < get_tcnt0_ticks()){
			returnValue = 1;
		}
	}
	uint32_t start = get_tcnt0_ticks();
	//uint16_t pulse = get_pulse() * 2 / 100;
	while ((PINC & (1<<ECHO0)) && !(returnValue)){
		//wait for echo pin to go low, but don't hold for too long
		if (start + delay < get_tcnt0_ticks()){
			returnValue = 1;
		}
	}
	if (!returnValue){
		duration = (get_tcnt0_ticks() - start);
		/* multiply by the speed of sound and half for one way travel, then send to serial. */
		distance = duration*3.4/2;
		info_ptr->sonicD0 = distance;
		
	}
	
	// sensor 1 now
	PORTA &= ~(1<<TRIG1);
	custom_delay(400);
	PORTA |= (1<<TRIG1);
	custom_delay(50);
	PORTA &= ~(1<<TRIG1);
	
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
		info_ptr->sonicD1 = distance;
	}
	// all succeeded
	return 0;
}