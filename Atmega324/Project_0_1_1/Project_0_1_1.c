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
//#include <util/delay.h>

/*	custom libraries	*/
#include "tcnt0.h"
#include "Stepper.h"
#include "usart0_.h"
#include "tcnt2.h"
#include "mpu6050.h"

/*	function declarations	*/
void initialise(void);
void custom_delay(uint32_t ticks);
void Sensor_input(void);
void step_clockwise(void);
void step_clockwise(void);



/*	set the UBRR0 register to BAUDREG (12 for 38.4k baudrate) */
#define F_CPU 8000000L		//Internal Calibrated RC Oscillator 8MHz
#define BAUDRATE 9600L			//from bluetooth datasheet38400
#define BAUDREG ((F_CPU)/(BAUDRATE*16UL)-1) 



/* functional code	*/
int main(void)
{
	int16_t ax = 0;
	int16_t ay = 0;
	int16_t az = 0;
	int16_t gx = 0;
	int16_t gy = 0;
	int16_t gz = 0;
	double axg = 0;
	double ayg = 0;
	double azg = 0;
	double gxds = 0;
	double gyds = 0;
	double gzds = 0;
	
	initialise();
    while(1)
    {
		mpu6050_getRawData(&ax, &ay, &az, &gx, &gy, &gz);
		mpu6050_getConvData(&axg, &ayg, &azg, &gxds, &gyds, &gzds);
		
		char itmp[10];
		ltoa(ax, itmp, 10); output_char(' '); output_string(itmp); output_char(' ');
		ltoa(ay, itmp, 10); output_char(' '); output_string(itmp); output_char(' ');
		ltoa(az, itmp, 10); output_char(' '); output_string(itmp); output_char(' ');
		ltoa(gx, itmp, 10); output_char(' '); output_string(itmp); output_char(' ');
		ltoa(gy, itmp, 10); output_char(' '); output_string(itmp); output_char(' ');
		ltoa(gz, itmp, 10); output_char(' '); output_string(itmp); output_char(' ');
		output_string("\r\n");

		dtostrf(axg, 3, 5, itmp); output_string(itmp); output_char(' ');
		dtostrf(ayg, 3, 5, itmp); output_string(itmp); output_char(' ');
		dtostrf(azg, 3, 5, itmp); output_string(itmp); output_char(' ');
		dtostrf(gxds, 3, 5, itmp); output_string(itmp); output_char(' ');
		dtostrf(gyds, 3, 5, itmp); output_string(itmp); output_char(' ');
		dtostrf(gzds, 3, 5, itmp); output_string(itmp); output_char(' ');
		output_string("\r\n");
		
		custom_delay(50);
        //TODO:: Please write your application code 
    }
}




void initialise(void){

	DDRA = 0xFF;

	DDRD |= (0<<PORTD7)|(1<<PORTD6);


	pwm_initialiser();
	init_tcnt0();
	
	
	//set Global Interrupt Enable flag
	sei();
	srand(get_tcnt0_ticks());

	USART0_init(BAUDREG);
	init_step();


	mpu6050_init();
	
}



void init_ADC(void){
	
	// The ADC clock must be somewhere
	// between 50kHz and 200kHz. We will divide our 1MHz clock by 64
	// to give us 62.5kHz.)
	ADMUX = (1<<REFS0); 	//set's compare to AVcc with external capacitor at AREF pin to ground.
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1); //enable ADC, ADATE (not used) sets autotriggering (using ADCSRB), ADPS sets sample every clk/64
	
}


uint16_t sense(uint8_t adcPin){
	/* takes in an ADC pin for conversion to occur on, and returns the converted value */
	
	adcPin &= 0b00000111; // only use lower three pins for conversion
	
	uint16_t value;
	//DDRC = 0x3C;		// 00111100
	//DDRD = 0x80;		// 10000000

	
	ADMUX = ((ADMUX & 0xF8)| adcPin);		//sets the ADMUX to itself, whilst clearing the last 3 bits, to be reset by adcPin
	
	ADCSRA |= (1<<ADSC);				// start conversion
	while(ADCSRA & (1<<ADSC)) {
		; /* Wait until conversion finished */
	}
	// ADC - a 10 bit register containing the 0-1024 value from the conversion
	value = ADC;
	
	return value;
		//if (value < 409.6){PORTC = (1<<PORTC4);}
		//if (value > 409.6 && value < 614.4){PORTC = (1<<PORTC5);}
		//else {PORTD = (1<<PORTD7);}
}

