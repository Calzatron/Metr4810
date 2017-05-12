/*
 * Project_0_1_1.c
 *
 * Created: 17/04/2017 6:40:49 PM
 * Author : s4357594
 */ 

/*	standard libraries	*/
#include <avr/io.h>
#include <avr/interrupt.h>			//enables interrupts
#include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
#include <stdio.h>					//input / output
#include <stdlib.h>					//standard function library; malloc / sort / rand etc
#include <math.h>
#include <string.h>


/*	custom libraries	*/
#include "Stepper.h"
#include "tcnt0.h"
#include "tcnt1.h"
#include "tcnt2.h"
#include "serialio.h"
#include "Project.h"

/*	set the UBRR0 register to BAUDREG (12 for 38.4k baudrate) */
#define F_CPU 8000000L				//Internal Calibrated RC Oscillator 8MHz
#define BAUDRATE 9600L				//from bluetooth datasheet 38400
#define BAUDREG ((F_CPU)/(BAUDRATE*16UL)-1) 


/*	function declarations	*/
info* makeInfo(void);
void custom_delay(uint32_t ticks);
void Sensor_input(void);
void step_clockwise(void);
void step_clockwise(void);
void initialise(info* info_ptr);
void input(info* info_ptr);


/* functional code	*/
int main(void) {
	
	info* info_ptr = makeInfo();
	initialise(info_ptr);
	DDRA |= (1<<PORTA0);
	PORTA |= (1<<PORTA0);
	
	while(1) {
		if(serial_input_available()){
			input(info_ptr);
		}
		if (info_ptr->motorAct){
			OCR2A = (255 * info_ptr->pwmSpeed)/100;
			OCR2B = (255 * info_ptr->pwmSpeed)/100;
			info_ptr->motorAct = 0;
			info_ptr->stupidCount = 0;
		}
		if (info_ptr->stopCommand){
			/* stop everything */
			PORTD &= ~(1<<PORTD2);
			PORTD &= ~(1<<PORTD3);
			PORTC &= ~(1<<PORTC0);
			PORTC &= ~(1<<PORTC1);
			if ((info_ptr->stepAct == 'A')||(info_ptr->stepAct == 'R')){
				hold_step();
				info_ptr->stepAct = '\0';
			}
			info_ptr->camera = 0;
			info_ptr->stopCommand = 0;
			info_ptr->stupidCount = 0;
		} else if ((info_ptr->stepAct == 'A')||(info_ptr->stepAct == 'R')){
			int16_t current = current_step();
			char buffer[20];
			sprintf(buffer, "step: %d %d %c\n", current, info_ptr->stepSpeed, info_ptr->stepAct);
			fputs(buffer, stdout);
			while ((info_ptr->stepSpeed < 0) && (info_ptr->stepAct == 'R') && (current >= info_ptr->maxStep)){
					step(info_ptr);
					current = current_step();
			}
			while ((info_ptr->stepSpeed > 0) && (info_ptr->stepAct == 'A') && (current <= 0)){
				step(info_ptr);
				current = current_step();
			}
			while ((current < info_ptr->maxStep) && (current >= 0)){
				if (serial_input_available()){
					char in = fgetc(stdin);
					if (in == ' '){
						info_ptr->stepAct = '\0';
						hold_step();
						fputs("canceled\n", stdout);
						break;
					}
				}
				step(info_ptr);
				current = current_step();				
			}
			fputs("end step\n", stdout);
			info_ptr->stepAct = '\0';
			info_ptr->stupidCount = 0;
		} else if (info_ptr->camera != 0){
			uint8_t pos = OCR0A;
			pos += info_ptr->camera;
			if (pos < 0){
					pos = 0;
			} else if (pos > 255){
					pos = 255;
			}
			/*	adjust camera position */
			OCR0A = pos;
			info_ptr->stupidCount = 0;
		}
		//TODO:: Please write your application code
	}
}

info* makeInfo(void) {
	/* initialize the game's information struct */
	info* info_ptr = malloc(sizeof(info));
	return info_ptr;
}


void input(info* info_ptr){
	
	if(!serial_input_available()){
		return;
		
	}

	char c = fgetc(stdin);
	int16_t speed;
	char buffer[20];
	
	switch(c){
		
		case 'a' :
			// turn left
			PORTD &= ~(1<<PORTD2);
			PORTD |= (1<<PORTD3);
			PORTC |= (1<<PORTC0);
			PORTC &= ~(1<<PORTC1);
			break;
		case 's' :
			// backwards
			PORTD &= ~(1<<PORTD2);
			PORTD |= (1<<PORTD3);
			PORTC &= ~(1<<PORTC0);
			PORTC |= (1<<PORTC1);
			break;
		case 'd' :
			// turn right
			PORTD |= (1<<PORTD2);
			PORTD &= ~(1<<PORTD3);
			PORTC &= ~(1<<PORTC0);
			PORTC |= (1<<PORTC1);
			break;
		case 'w' :
			// forward
			PORTD |= (1<<PORTD2);
			PORTD &= ~(1<<PORTD3);
			PORTC |= (1<<PORTC0);
			PORTC &= ~(1<<PORTC1);
			break;
		case 'r' :
			// release claw
			release_step();
			info_ptr->stepAct = '\0';
			break;
		case '+' :
			// rely to winch to lift up
			//fputc('+', stdout);
			break;
		case '=' :
			// someone panic'd; winch to lift up
			fputc('+', stdout);
			break;
		case '-' :
			// lower
			fputc('_', stdout);
			break;
		case 'e' :
			// activate claw
			if (info_ptr->stepSpeed < 0){
				info_ptr->stepSpeed = -info_ptr->stepSpeed;
			}

			sprintf(buffer, "activate %d\n", info_ptr->stepSpeed);
			fputs(buffer, stdout);
			info_ptr->stepAct = 'A';
			break;
		case 'q' :
			// retract claw
			if (info_ptr->stepSpeed > 0){
					info_ptr->stepSpeed = -info_ptr->stepSpeed;
			}
			/************sends negative to activate winch *******/
			sprintf(buffer, "retract %d\n", info_ptr->stepSpeed);
			fputs(buffer, stdout);
			info_ptr->stepAct = 'R';
			break;
		case '`' :
			info_ptr->pwmSpeed = 0;
			info_ptr->motorAct = 1;
			break;
		case '0' :
			info_ptr->pwmSpeed = 0;
			info_ptr->motorAct = 1;
			break;
		case '1' :
			info_ptr->pwmSpeed = 20;
			info_ptr->motorAct = 1;
			break;
		case '2' :
			info_ptr->pwmSpeed = 40;
			info_ptr->motorAct = 1;
			break;
		case '3' :
			info_ptr->pwmSpeed = 60;
			info_ptr->motorAct = 1;
			break;
		case '4' :
			info_ptr->pwmSpeed = 80;
			info_ptr->motorAct = 1;
			break;
		case '5' :
			info_ptr->pwmSpeed = 100;
			info_ptr->motorAct = 1;
			break;
		case '[' :
			speed = info_ptr->stepSpeed - 10;
			if (speed > -110){
				info_ptr->stepSpeed -= 10;
			}
			
			sprintf(buffer, "Step speed %d\n", info_ptr->stepSpeed);
			fputs(buffer, stdout);
			break;
		case ']' :
			speed = info_ptr->stepSpeed - 10;
			if (speed < 110){
				info_ptr->stepSpeed += 10;
			}
			
			sprintf(buffer, "Step speed %d\n", info_ptr->stepSpeed);
			fputs(buffer, stdout);
			break;
		case ' ' :
			info_ptr->pwmSpeed = 0;
			info_ptr->stopCommand = 1;
			info_ptr->motorAct = 0;
			info_ptr->stepAct = '\0';
			//fputc(' ', stdout);
			break;
		case 'h' :
			info_ptr->camera = 10;
			break;
		case 'g' :
			info_ptr->camera = -10;
			break;
		case '?' :
			// send sense command up
			//fputc('?', stdout);
			//info_ptr->stopCommand = 1;
			break;
		case 'o':
			// restart
			fputs("started\n", stdout);
			break;
		default:
			fputs("stupid.\n", stdout);
			++info_ptr->stupidCount;
			if (info_ptr->stupidCount >= 10){
					fputc('H', stdout);
					custom_delay(1000);
					info_ptr->stupidCount = 0;
			}
			break;
	}
	/* echo character to next micro */
	fputc(c, stdout);
	
}



void initialise(info* info_ptr){

	DDRA = (1<<PORTA0)|(1<<PORTA1);
	DDRB = (1<<PORTB3);			/*	PWM camera	*/
	DDRC = (1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4)|(1<<PORTC5)|(1<<PORTC0)|(1<<PORTC1);
	DDRD = (1<<PORTD1)|(1<<PORTD4)|(1<<PORTD3)|(1<<PORTD2)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7);
	DDRD &= ~(1<<PIND0);
	init_serial_stdio(9600,0);

	init_tcnt0();				/*	Camera PWM	*/
	init_tcnt1();				/*	Timer		*/
	init_tcnt2();				/*	Motor PWM	*/
	init_step();
	srand(get_tcnt1_ticks());
	sei();
	
		
	uint8_t check = 1;
	while (check){
			
		if (serial_input_available()){
			char start = fgetc(stdin);
			if (start == 'o'){
				fputs("Start\n", stdout);
				check = 0;
				break;			/*	make sure it exits	*/
			}
				
		}
			
	}

	info_ptr->start = 1;
	info_ptr->stepAct = '\0';
	info_ptr->maxStep = 150;
	info_ptr->camera = 0;
	info_ptr->pwmSpeed = 0;
	PORTD |= (1<<PORTD4)|(1<<PORTD5);	//enable stepper
	
}




void custom_delay(uint32_t ticks){
	/*	Custom delay function, waits for timer to change by 'ticks'
	*	note that tcnt1 updates every 0.002 seconds */
	
	uint32_t current_time;
	current_time = get_tcnt1_ticks();
	while((current_time + ticks) > get_tcnt1_ticks()){
		;
	}
}


void init_ADC(void){
	
	/* The ADC clock must be somewhere
	* between 50kHz and 200kHz. We will divide our 1MHz clock by 64
	* to give us 62.5kHz.)
	*/
	ADMUX = (1<<REFS0); 							//set's compare to AVcc with external capacitor at AREF pin to ground.
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);		//enable ADC, ADATE (not used) sets autotriggering (using ADCSRB), ADPS sets sample every clk/64
	
}


uint16_t sense(uint8_t adcPin){
	/* takes in an ADC pin for conversion to occur on, and returns the converted value */
	
	adcPin &= 0b00000111;					// only use lower three pins for conversion
	uint16_t value;
	ADMUX = ((ADMUX & 0xF8)| adcPin);		//sets the ADMUX to itself, whilst clearing the last 3 bits, to be reset by adcPin
	
	ADCSRA |= (1<<ADSC);					// start conversion
	while(ADCSRA & (1<<ADSC)) {
		; /* Wait until conversion finished */
	}
	/* ADC - a 10 bit register containing the 0-1024 value from the conversion */
	value = ADC;
	
	return value;
		//if (value < 409.6){PORTC = (1<<PORTC4);}
		//if (value > 409.6 && value < 614.4){PORTC = (1<<PORTC5);}
		//else {PORTD = (1<<PORTD7);}
}

