/*
 * USART0_.c
 *
 * Created: 17/04/2017 8:09:17 PM
 *  Author: s4357594
 */ 

#include "USART0_.h"

 #include <avr/io.h>
 #include <avr/interrupt.h>			//enables interrupts
 #include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
 #include <stdio.h>					//input / output
 #include <stdlib.h>					//standard function library; malloc / sort / rand etc


#define OUTPUT_BUFFER_SIZE 255
volatile char output_buffer[OUTPUT_BUFFER_SIZE];
volatile uint8_t output_insert_pos;
volatile uint8_t bytes_in_output_buffer;



#define INPUT_BUFFER_SIZE 16
volatile char input_buffer[INPUT_BUFFER_SIZE];
volatile uint8_t input_insert_pos;
volatile uint8_t bytes_in_input_buffer;
volatile uint8_t input_overrun;

static volatile unsigned char transmitByte;
static volatile unsigned char receivedByte;



 void USART0_init(long ubrr){
	 // UBRR0 is split into two; store the high 4 bits of the baud rate
	 // in UBRR0H and then rest in UBRR0L
	 receivedByte = 0x00;
	 transmitByte = 0x00;

	 output_insert_pos = 0;
	 bytes_in_output_buffer = 0;
	 input_insert_pos = 0;
	 bytes_in_input_buffer = 0;
	 input_overrun = 0;

	 UBRR0H = (ubrr>>8);
	 UBRR0L = (ubrr);

	 // set up receiver and transmitter
	 UCSR0B |= (1<<TXEN0)|(1<<RXEN0);
	 // enable interrupt reciever and transmitter
	 UCSR0B |= (1<<TXCIE0)|(1<<RXCIE0);
	 //UCSR0A |= (1<<TXC);

	 // control status register, keep asynchronous, parity disabled, 8-bit
	 UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
 }
 

int8_t serial_input_available(void) {
	return (bytes_in_input_buffer != 0);
}


void clear_serial_input_buffer(void) {
	/* Just adjust our buffer data so it looks empty */
	input_insert_pos = 0;
	bytes_in_input_buffer = 0;
}


unsigned char input(void){
	/* Returns the latest value received in buffer	*/

	if (bytes_in_input_buffer == 0){
		// returns 0x00;
		return receivedByte;
	}

	uint8_t interrupts_on = bit_is_set(SREG, SREG_I);
	cli();

	if(input_insert_pos - bytes_in_input_buffer < 0) {
		/* Need to wrap around */
		receivedByte = input_buffer[input_insert_pos - bytes_in_input_buffer
		+ INPUT_BUFFER_SIZE];
		} else {
		receivedByte = input_buffer[input_insert_pos - bytes_in_input_buffer];
	}
	
	/* Decrement our count of bytes in the input buffer */
	bytes_in_input_buffer--;

	unsigned char returnValue = receivedByte;
	receivedByte = 0x00;
	if(interrupts_on){
		sei();
	}
	return returnValue;
}


void output_char(unsigned char serialOut){
	/*	adding bits to the buffer to transmit */
	// disable interrupts so half modified buffer isn't used
	uint8_t interrupts_on = bit_is_set(SREG, SREG_I);
	cli();

	// copy the byte to transmit

	output_buffer[output_insert_pos++] = serialOut;
	bytes_in_output_buffer++;
	
	if(output_insert_pos == OUTPUT_BUFFER_SIZE) {
		/* Wrap around buffer pointer if necessary */
		output_insert_pos = 0;
	}
	// re-enable UART
	UCSR0B |= (1<<UDRIE0);
	// re-enable interrupts
	if(interrupts_on){
		sei();
	}
}



void output_string(unsigned char* serialOutString){
	
	while (*serialOutString){
		output_char(*serialOutString++);
	}
}




 ISR (USART0_DRE_vect){

/* Check if we have data in our buffer */
	if(bytes_in_output_buffer > 0) {
		/* Yes we do - remove the pending byte and output it
		 * via the UART. The pending byte (character) is the
		 * one which is "bytes_in_buffer" characters before the 
		 * insert_pos (taking into account that we may 
		 * need to wrap around to the end of the buffer).
		 */
		//char c;
		if(output_insert_pos - bytes_in_output_buffer < 0) {
			/* Need to wrap around */
			transmitByte = output_buffer[output_insert_pos - bytes_in_output_buffer
				+ OUTPUT_BUFFER_SIZE];
		} else {
			transmitByte = output_buffer[output_insert_pos - bytes_in_output_buffer];
		}
		/* Decrement our count of the number of bytes in the 
		 * buffer 
		 */
		bytes_in_output_buffer--;
		
		/* Output the character via the UART */
		UDR0 = transmitByte;
	} else {
		// disable UART as transmission complete
		UCSR0B &= ~(1<<UDRIE0);
	}

 }
 
 
 
 ISR (USART0_RX_vect){
	// code to execute when the USART receives a byte here

	receivedByte = UDR0;
	input_buffer[input_insert_pos++] = receivedByte;

	if(input_insert_pos >= INPUT_BUFFER_SIZE - 1){
		input_insert_pos = 0;
	}

	bytes_in_input_buffer++;
	if(bytes_in_input_buffer >= INPUT_BUFFER_SIZE) {
		input_overrun = 1;
	}

 }