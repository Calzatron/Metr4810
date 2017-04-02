/*
 * USART_Receiver.c
 *
 * Created: 24/03/2017 3:06:43 PM
 *  Author: s4357594
 */ 
 #include "USART_Receiver.h"

 #include <avr/io.h>
 #include <avr/interrupt.h>			//enables interrupts
 #include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
 #include <stdio.h>					//input / output
 #include <stdlib.h>					//standard function library; malloc / sort / rand etc

 void USART_init(unsigned int ubrr);

 static volatile uint8_t transmitByte;
 static volatile uint8_t receivedByte;


 void USART_init(unsigned int ubrr){
	 // UBRR0 is split into two; store the high 4 bits of the baud rate
	 // in UBRR0H and then rest in UBRR0L
	 receivedByte = 0x00;
	 
	 UBRR0H = (unsigned char)(ubrr>>8);
	 UBRR0L = (unsigned char)(ubrr);

	 // set up receiver and transmitter
	 UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
	 // enable interrupt reciever
	 UCSR0B |= (1<<RXCIE0);
	 //UCSR0A |= (1<<TXC);

	 // control status register, keep asynchronous, parity disabled, 8-bit
	 UCSR0C = (1<<UCSZ11)|(1<<UCSZ10);
 }


 uint8_t input(void){
	uint8_t returnValue = receivedByte;
	return returnValue;
 }

 void output(uint8_t serialOut){
	/*	adding bits to the buffer to transmit */
	// disable interrupts so half modified buffer isn't used 
	uint8_t interrupts_on = bit_is_set(SREG, SREG_I);
	cli();

	// copy the byte to transmit

	transmitByte = serialOut;


	// re-enable UART
	UCSR0B |= (1<<UDRIE0);
	// re-enable interrupts
	if(interrupts_on){
		sei();
	}
 }


 ISR(USART0_UDRE_vect){

	// send the transmitByte
	if (transmitByte != 0x00){
		UDR0 = transmitByte;
	} else {
		// disable UART as transmission complete
		UCSR0B &= ~(1<<UDRIE0);	
	}
 }



 ISR(USART0_RX_vect){
	// code to execute when the USART receives a byte here
	//uint8_t recievedByte;
	receivedByte = UDR0;


	// turn all pins in A on -> displays received byte to LEDs
	PORTA = ~receivedByte;
	// if statements here
 }

