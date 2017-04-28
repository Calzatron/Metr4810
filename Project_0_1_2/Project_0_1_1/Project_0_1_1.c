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
//#include "usart.h"
//#include <util/delay.h>

/*	custom libraries	*/
#include "tcnt0.h"
#include "Stepper.h"
//#include "usart0_.h"
#include "tcnt2.h"
#include "serialio.h"
#include "mpu6050.h"
//#include "USART_Transmitter.h"
#include "Project.h"

/*	set the UBRR0 register to BAUDREG (12 for 38.4k baudrate) */
#define F_CPU 8000000L		//Internal Calibrated RC Oscillator 8MHz
#define BAUDRATE 9600L			//from bluetooth datasheet38400
#define BAUDREG ((F_CPU)/(BAUDRATE*16UL)-1) 

//typedef struct info {
//
	//uint32_t sample;
	//char* mess;
	//uint16_t messLen;
	//char buffer[8];
	//int16_t ax;
	//int16_t ay;
	//int16_t az;
	//int16_t gx;
	//int16_t gy;
	//int16_t gz;
	//double axg;
	//double ayg;
	//double azg;
	//double gxds;
	//double gyds;
	//double gzds;
//} info;


/*	function declarations	*/
info* makeInfo(void);
void custom_delay(uint32_t ticks);
void Sensor_input(void);
void step_clockwise(void);
void step_clockwise(void);
void initialise(info* info_ptr);



/* functional code	*/
int main(void)
{
	//initialise();
	info* info_ptr = makeInfo();
	initialise(info_ptr);
	while(1)
	{
		
		
		//TODO:: Please write your application code
	}
}

info* makeInfo(void){
	/* initialize the game's information struct */
	info* info_ptr = malloc(sizeof(info));
	return info_ptr;
}

void mpu_get_data(info* info_ptr){
//
	int16_t ax = 0;
	int16_t ay = 0;
	int16_t az = 0;
	int16_t gx = 0;
	int16_t gy = 0;
	int16_t gz = 0;
	int16_t ax1 = 0;
	int16_t ay1 = 0;
	int16_t az1 = 0;
	int16_t gx1 = 0;
	int16_t gy1 = 0;
	int16_t gz1 = 0;
	//double axg = 0;
	//double ayg = 0;
	//double azg = 0;
	//double gxds = 0;
	//double gyds = 0;
	//double gzds = 0;
	//double axg1 = 0;
	//double ayg1 = 0;
	//double azg1 = 0;
	//double gxds1 = 0;
	//double gyds1 = 0;
	//double gzds1 = 0;
	mpu6050_getRawData(info_ptr, &ax, &ay, &az, &gx, &gy, &gz);
	custom_delay(10);
	mpu6050_getRawData(info_ptr, &ax1, &ay1, &az1, &gx1, &gy1, &gz1);
	char itmp[200];
	int16_t reading = (ax1-ax)*info_ptr->sample*info_ptr->sample/1000;

	ltoa(reading, itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	reading = (ay1-ay)*info_ptr->sample*info_ptr->sample/1000;

	ltoa(reading, itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	reading = (az1-az)*info_ptr->sample*info_ptr->sample/1000;

	ltoa(reading, itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	reading = (gx1-gx)*info_ptr->sample/100;

	ltoa(reading, itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	reading = (gy1-gy)*info_ptr->sample/100;

	ltoa(reading, itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	reading = (gz1-gz)*info_ptr->sample/100;

	ltoa(reading, itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	//ltoa((ay1-ay)*info_ptr->sample/1000, itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	//ltoa((az1-az)*info_ptr->sample/1000, itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	//ltoa((gx1-gx)*info_ptr->sample/1000, itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	//ltoa((gy1-gy)*info_ptr->sample/1000, itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	//ltoa((gz1-gz)*info_ptr->sample/1000, itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	fputs("\r\n", stdout);
	ltoa((ax), itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	ltoa((ay), itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	ltoa((az), itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	ltoa((gx), itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	ltoa((gy), itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	ltoa((gz), itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	fputs("\r\n", stdout);
	ltoa((ax1), itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	ltoa((ay1), itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	ltoa((az1), itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	ltoa((gx1), itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	ltoa((gy1), itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	ltoa((gz1), itmp, 10); fputc(' ', stdout); fputs(itmp,stdout); fputc(' ', stdout);
	ltoa(info_ptr->sample, itmp, 10);
	fputs(itmp, stdout);
	fputs("\r\n", stdout);
	fputs("\r\n", stdout);
//
	//mpu6050_getConvData(info_ptr, &axg, &ayg, &azg, &gxds, &gyds, &gzds);
	//custom_delay(50);
	//mpu6050_getConvData(info_ptr, &axg1, &ayg1, &azg1, &gxds1, &gyds1, &gzds1);
//
	//dtostrf((axg-axg1)*info_ptr->sample, 3, 5, itmp); fputs(itmp,stdout); fputc(' ', stdout);
	//dtostrf((ayg-ayg1)*info_ptr->sample, 3, 5, itmp); fputs(itmp,stdout); fputc(' ', stdout);
	//dtostrf((azg-azg1)*info_ptr->sample, 3, 5, itmp); fputs(itmp,stdout); fputc(' ', stdout);
	//dtostrf((gxds-gxds1)*info_ptr->sample, 3, 5, itmp); fputs(itmp,stdout); fputc(' ', stdout);
	//dtostrf((gyds-gyds1)*info_ptr->sample, 3, 5, itmp); fputs(itmp,stdout); fputc(' ', stdout);
	//dtostrf((gzds-gyds1)*info_ptr->sample, 3, 5, itmp); fputs(itmp,stdout); fputc(' ', stdout);
	
	fputs("\r\n", stdout);
	ltoa(info_ptr->sample, itmp, 10);
	fputs(itmp, stdout);
	fputs("\r\n", stdout);
	fputs("\r\n", stdout);

	custom_delay(200);


}

void initialise(info* info_ptr){

	DDRA = 0xFF;

	DDRD = (0<<PORTD7)|(1<<PORTD6);

	DDRD |= (0<<PIND0);
	DDRD |= (1<<PORTD1);

	//pwm_initialiser();
	init_tcnt0();
	init_serial_stdio(9600,0);
	//USART0_init(BAUDREG);
	//set Global Interrupt Enable flag
	
	srand(get_tcnt0_ticks());
	sei();
	

	info_ptr->ax = 0;
	info_ptr->ay = 0;
	info_ptr->az = 0;
	info_ptr->gx = 0;
	info_ptr->gy = 0;
	info_ptr->gz = 0;
	info_ptr->axg = 0;
	info_ptr->ayg = 0;
	info_ptr->azg = 0;
	info_ptr->gxds = 0;
	info_ptr->gyds = 0;
	info_ptr->gzds = 0;
	info_ptr->sample = get_tcnt0_ticks();
	info_ptr->prevSample = 0;
	custom_delay(250);
	PORTA = 0b00000001;
	//output_char('X');
	custom_delay(250);
	PORTA = 0b00000010;
	//output_char('Y');
	custom_delay(250);
	PORTA = 0b00000011;
	//output_char('Z');
	custom_delay(250);
	PORTA = 0b00000110;
	//output_char('E');
	custom_delay(250);
	PORTA = 0b00000111;
	//output_char('F');
	custom_delay(1000);
	PORTA = 0b00001010;
	//output_char('G');
	custom_delay(1000);
	PORTA = 0b00001000;
	//output_char('H');
	custom_delay(1000);
	PORTA = 0b00001110;
	//output_char('I');
	custom_delay(1000);
	char t = 'j';
	fputc(t, stdout);
	mpu6050_init();
	custom_delay(10);
	PORTA = 0b00001011;

	while(1){

		mpu_get_data(info_ptr);



	}
//
////	char t = input();
	//t = 'a';
	//int a = 'a';
	//if (t){
		//fputc(a, stdout);
		//fputc(t, stdout);
	//}
	//uint8_t g = 0;
	//PORTA = 0b00001000;
	//while(g < 7){
//
		//uint8_t b = 'b';
		//uint8_t c = 'c';
		//uint8_t d = 'd';
		//t = 'b';
		//char itmp[10];
		//
//
		//ltoa(info_ptr->ax, itmp, 10); fputc(' ', stdout); fputs(itmp, stdout); fputc(' ', stdout);
		//if (t){
			//fputc(t++, stdout);
			//fputc(t, stdout);
		//}
		//PORTA = 0xFF;
		//custom_delay(100);
		//PORTA = 0x00;
		//custom_delay(100);
		//g = g+1;
//
//
	//}
	//while(1){
		//char incomming;
				//if(serial_input_available()){
					//incomming = fgetc(stdin);
					//// echo back
					//fputc(incomming, stdout);
				//}
			//PORTA ^= 0xAA;
//
	//}

	//mpu6050_init();
	
}

void custom_delay(uint32_t ticks){
	/*	Custom delay function, waits for timer to change by 'ticks'
	*	note that tcnt0 updates every 0.002 seconds */
	
	uint32_t current_time;
	current_time = get_tcnt0_ticks();
	while((current_time + ticks) > get_tcnt0_ticks()){
		//wait
		//PORTA |= 0b00000001;
	}
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

