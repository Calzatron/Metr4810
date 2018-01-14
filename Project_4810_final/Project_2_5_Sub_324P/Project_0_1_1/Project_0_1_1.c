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
#include "tcnt0.h"
#include "tcnt2.h"
#include "Stepper.h"
#include "serialio.h"
#include "Project.h"
#include "worm.h"
#include "servo.h"
#include "pivot.h"

/*	set the UBRR0 register to BAUDREG (12 for 38.4k baudrate) */
#define F_CPU 8000000L				//Internal Calibrated RC Oscillator 8MHz
#define BAUDRATE 9600L				//from bluetooth datasheet 38400
#define BAUDREG ((F_CPU)/(BAUDRATE*16UL)-1) 


/*	function declarations	*/
info* makeInfo(void);
void custom_delay(uint32_t ticks);
void initialise(info* info_ptr);
void input(info* info_ptr);


/* functional code	*/
int main(void) {
	/* initialise the submersible's information struct */
	info* info_ptr = makeInfo();
	initialise(info_ptr);
	
	/* Reads serial input and executes commands based on input */	
	while(1) {
		/* Executes submersible commands and transmits serial to float micro controller  */
		if(serial_input_available()){
			input(info_ptr);
		}
		/* Controls propulsion motor speeds from received serial */
		if (info_ptr->motorAct){
			OCR2A = (255 * info_ptr->pwmSpeed)/100;
			OCR2B = (255 * info_ptr->pwmSpeed)/100;
			info_ptr->motorAct = 0;
			info_ptr->stupidCount = 0;
		}
		/* Stops all submersible systems, holds claw in place */
		if (info_ptr->stopCommand){
			/* stop everything */
			PORTD &= ~(1<<PORTD2);
			PORTD &= ~(1<<PORTD3);
			PORTC &= ~(1<<PORTC6);
			PORTC &= ~(1<<PORTC7);
			if ((info_ptr->stepAct == 'A')||(info_ptr->stepAct == 'R')){
				hold_step();
				info_ptr->stepAct = '\0';
			}
			info_ptr->stopCommand = 0;
			info_ptr->stupidCount = 0;
			//worm_off();						/*	claw off, should already be 	*/
			//pivot_off();					/*	pivot control motor off */
		}
		else if ((info_ptr->stepAct == 'A')||(info_ptr->stepAct == 'R')){
			/*	run stepper motor	*/
			int16_t current = current_step();
			char buffer[20];
			sprintf(buffer, "step: %d %d %c\n", current, info_ptr->stepSpeed, info_ptr->stepAct);
			fputs(buffer, stdout);
			while ((info_ptr->stepSpeed < 0) && (info_ptr->stepAct == 'R') && (current >= info_ptr->maxStep)){
				/*	stepper is at a limit, but wants to turn the right way -> move claw	off limit   */
					step(info_ptr);
					current = current_step();
			}
			while ((info_ptr->stepSpeed > 0) && (info_ptr->stepAct == 'A') && (current <= 0)){
				/*	stepper is at a limit, but wants to turn the right way -> move claw off limit	*/
				step(info_ptr);
				current = current_step();
			}
			while ((current < info_ptr->maxStep) && (current >= 0)){
				/*	stepper is not at a limit and wants to move -> allow it		*/
				if (serial_input_available()){
					char in = fgetc(stdin);
					if (in == ' '){
						/*	cancel command received, hold stepper in current position	*/
						info_ptr->stepAct = '\0';
						hold_step();
						fputs("canceled\n", stdout);
						break;
					}
				}
				step(info_ptr);
				current = current_step();				
			}
			current = current_step();
			char buffer1[30];
			sprintf(buffer1, "step POS: %d %d %c\n", current, info_ptr->stepSpeed, info_ptr->stepAct);
			fputs(buffer1, stdout);
			fputs("end step\n", stdout);
			info_ptr->stepAct = '\0';		/*	stepper reached it's final position -> not activating or retracting */
			info_ptr->stupidCount = 0;		/*	valid command received and acted on */
			hold_step();
		} else if (info_ptr->wormCom == 'k'){
			/*	received command to move claw	*/
			if (info_ptr->wormButtonOff){
				/* buttons disabled */
				continue;
			}
			uint8_t percent = 60;
			int8_t speed = 1;
			char in;
			char buffer[18];
			move_worm(percent, speed);
			while (info_ptr->wormStop != 'k') {
				if(serial_input_available()){
					in = fgetc(stdin);
					if (in == ' '){
						/*	cancel command received -> stop claw	*/
						fputs("claw off\n", stdout);
						worm_off();
						break;
					} else if (in == 'k'){
						/*	same command received, increase speed	*/
						if ((percent + 10) < 100){
							percent += 10;
							sprintf(buffer, "Worm speed %d\n", percent);
							fputs(buffer, stdout);
						}
					} else if (in == 'j'){
						/*	decrease speed command received	*/
						if ((percent -10) >=0){
							percent -= 10;
							sprintf(buffer, "Worm speed %d\n", percent);
							fputs(buffer, stdout);
						}
					}
				}
				/*	check if button pressed */
				//if (!(PINB & (1<<PINB1)) || !(PINB & (1<<PINB0))){
					//info_ptr->wormStop = 'k';
					//worm_off();
					//break;
				//}
				/*	checks and speeds are in place, move worm gear for claw */
				move_worm(percent, speed);
			}
			info_ptr->wormCom = '\0';
			info_ptr->stupidCount = 0;
		} else if (info_ptr->wormCom == 'j'){
			/*		move the claw opposite to 'k' command	*/
			if (info_ptr->wormButtonOff){
				/* buttons disabled */
				continue;
			}
			uint8_t percent = 60;
			int8_t speed = 0;
			char in;
			char buffer[18];
			move_worm(percent, speed);
			while (info_ptr->wormStop != 'j') {
				if(serial_input_available()){
					in = fgetc(stdin);
					if (in == ' '){
						fputs("claw1 off\n", stdout);
						worm_off();
						break;
						} else if (in == 'k'){
							if ((percent + 10) < 100){
								percent += 10;
								sprintf(buffer, "Worm speed %d\n", percent);
								fputs(buffer, stdout);
							}
						} else if ( in == 'j'){
						if ((percent -10) >=0){
							percent -= 10;
							sprintf(buffer, "Worm speed %d\n", percent);
							fputs(buffer, stdout);
						}
					}
				}
				//if ((!(PINB & (1<<PINB1))) || (!(PINB & (1<<PINB0)))){
					///*	button actuated -> stop claw from moving and exit loop	*/
					//info_ptr->wormStop = 'j';
					//worm_off();
					//break;
				//}
				move_worm(percent, speed);
			}
			info_ptr->wormCom = '\0';
			info_ptr->stupidCount = 0;
		} else if ((info_ptr->pivotCom == 'z') || (info_ptr->pivotCom == 'x')){
			/*	move pivot motor down	*/
			int8_t direction;
			if (info_ptr->pivotCom == 'z'){
					direction = 0;
			} else {
					direction = 1;
			}
			/*	actuate pivot motor at full power	*/
			move_pivot(direction);
		}
		
		/*	check that a button hasn't been hit -> claw can operate	*/
		//if ((!(PINB & (1<<PINB1))) || (!(PINB & (1<<PINB0)))){
			////if ((!(PORTC & (1<<PORTC2))) && (!(PORTC & (1<<PORTC5)))){
				////worm_off();
			////}
		//} else {
			/*	worm gear hasn't hit button and is free to use in both directions	*/
			info_ptr->wormStop = '\0';
		//}

	}
}

info* makeInfo(void) {
	/* initialise the submersible's information struct,
	* which will contain all the necessary information to operate */
	info* info_ptr = malloc(sizeof(info));
	return info_ptr;
}

void input(info* info_ptr){
	if(!serial_input_available()){
		return;
	}
	char c = fgetc(stdin);
	
	/* echo character to next micro */
	if ((c != '-') && (c != '=')){
		fputc(c, stdout);
		fputs(": ", stdout);	
	}
	/*	initialise case constants	*/
	int16_t speed;								/*	speed of stepper / direction of worm gear (depending on which is used)	*/
	/*	output buffer	*/
	char buffer[20];
	/*	check received command	*/
	switch(c){
		case 'a' :
			// turn left
			PORTD &= ~(1<<PORTD2);
			PORTD |= (1<<PORTD3);
			PORTC |= (1<<PORTC6);
			PORTC &= ~(1<<PORTC7);
			break;
		case 'w' :
			// backwards
			PORTD &= ~(1<<PORTD2);
			PORTD |= (1<<PORTD3);
			PORTC &= ~(1<<PORTC6);
			PORTC |= (1<<PORTC7);
			break;
		case 'd' :
			// turn right
			PORTD |= (1<<PORTD2);
			PORTD &= ~(1<<PORTD3);
			PORTC &= ~(1<<PORTC6);
			PORTC |= (1<<PORTC7);
			break;
		case 's' :
			// forward
			PORTD |= (1<<PORTD2);
			PORTD &= ~(1<<PORTD3);
			PORTC |= (1<<PORTC6);
			PORTC &= ~(1<<PORTC7);
			break;
		case 'r' :
			// release claw
			release_step();
			info_ptr->stepAct = '\0';
			break;
		case '+' :
			// relay to winch to lift up
			break;
		case '=' :
			// someone panic'd; winch to lift up
			fputc('+', stdout);
			fputs(": ", stdout);
			break;
		case '-' :
			// lower winch
			fputc('_', stdout);
			fputs(": ", stdout);
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
		case '{' :
			speed = info_ptr->stepSpeed - 10;
			if (speed > -110){
				info_ptr->stepSpeed -= 10;
			}
			sprintf(buffer, "Step speed %d\n", info_ptr->stepSpeed);
			fputs(buffer, stdout);
			break;
		case '}' :
			speed = info_ptr->stepSpeed - 10;
			if (speed < 110){
				info_ptr->stepSpeed += 10;
			}
			sprintf(buffer, "Step speed %d\n", info_ptr->stepSpeed);
			fputs(buffer, stdout);
			break;
		case '[' :
			move_up();
			fputs("Camera up \n", stdout);
			break;
		case ']' :
			move_down();
			fputs("Camera down \n", stdout);
			break;
		case ' ' :
			/*	stop command activate	*/
			info_ptr->pwmSpeed = 0;
			info_ptr->stopCommand = 1;
			info_ptr->motorAct = 0;
			info_ptr->stepAct = '\0';
			//fputc(' ', stdout);
			break;
		case 'l' :
			/*	toggle light	*/
			if (PORTA & (1<<PORTA1)){
				PORTA &= ~(1<<PORTA1);
			} else {
				PORTA |= (1<<PORTA1);
			}
			break;
		case 'L' :
			PORTA |= (1<<PORTA1);
			break;
		case 'j' : 
			/* move claw backwards	*/
			info_ptr->wormCom = 'j';
			break;
		case 'k' : 
			/* move claw forwards	*/
			info_ptr->wormCom = 'k';
			break;
		case 'z' :
			/*	drive the submersible's nose downwards	*/
			info_ptr->pivotCom = 'z';
			break;
		case 'x' :
			/*	drive the submersible's nose upwards	*/
			info_ptr->pivotCom = 'x';
			break;
		case 'v' :
			/*		cancel pivot motor actuation	*/
			//PORTC |= (1<<PORTC3)|(1<<PORTC4);
			pivot_off();
			break;
		case 'c' :
			/*		cancel pivot motor actuation	*/
			//PORTC |= (1<<PORTC3)|(1<<PORTC4);
			pivot_off();
			break;
		case '?' :
			// send sense command up
			break;
		case 'o' :
			/*		restart		*/
			fputs("started\n", stdout);
			break;
		case 'X' :
			/*	buttons are shorting for no reason, disable button control for claw */
			info_ptr->wormButtonOff = 1;
			break;
		case 'H' :
			/*	software restart	*/
			PORTA = 0x00;
			PORTB = 0x00;
			PORTC = 0x00;
			PORTD = 0x00;
			DDRA = 0x00;
			DDRB = 0x00;
			DDRC = 0x00;
			DDRD = 0x00;
			initialise(info_ptr);
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
}

void initialise(info* info_ptr){
	
	/* Port initialisation */
	DDRA = (1<<PORTA0)|(1<<PORTA1);						/* Led and switch */
	DDRB = (1<<PORTB3)|(1<<PORTB0)|(1<<PINB1);			/*	PWM camera, buttons	*/
	PORTB |= (1<<PORTB1)|(1<<PORTB0);								/* set button to be high */
	DDRB &= ~((1<<PORTB1)|(1<<PORTB0));								/*	make input	*/
	DDRC = (1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4)|(1<<PORTC5)|(1<<PORTC6)|(1<<PORTC7);
	DDRD = (1<<PORTD1)|(1<<PORTD4)|(1<<PORTD3)|(1<<PORTD2)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7);			/* Servo */
	DDRD &= ~(1<<PIND0);
	init_serial_stdio(9600,0);											
	init_tcnt0();				/*	Timer		*/
	init_tcnt2();				/*	Motor PWM	*/
	init_servo();				/*	tcnt1 for servo PWM	*/
	srand(get_tcnt0_ticks());
	sei();
	/****************************************************************************************************************************************************************/
	/* insert test code here */
	/*
	move_up();
	custom_delay(500);
	move_up();
	custom_delay(500);
	move_up();
	*/
	/****************************************************************************************************************************************************************/
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
	/* Structure information initialisation */
	info_ptr->start = 1;
	info_ptr->stepAct = '\0';
	info_ptr->maxStep = 150;
	//info_ptr->camera = 0;
	info_ptr->pwmSpeed = 0;
	info_ptr->wormStop = '\0';
	info_ptr->wormButtonOff = 0;
	PORTC &= ~((1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4)|(1<<PORTC5));
	PORTD |= (1<<PORTD4)|(1<<PORTD5);	//enable claw motors
}

void custom_delay(uint32_t ticks){
	/*	Custom delay function, waits for timer to change by 'ticks'
	*	note that tcnt1 updates every 0.002 seconds */
	uint32_t current_time;
	current_time = get_tcnt0_ticks();
	while((current_time + ticks) > get_tcnt0_ticks()){
		;
	}
}

void init_ADC(void){
	/* The ADC clock must be somewhere
	* between 50kHz and 200kHz. We will divide our 1MHz clock by 64
	* to give us 62.5kHz.)
	*/
	ADMUX = (1<<REFS0); 							//set's compare to AVcc with external capacitor at AREF pin to ground.
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);		//enable ADC, ADATE (not used) sets auto triggering (using ADCSRB), ADPS sets sample every clk/64
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

