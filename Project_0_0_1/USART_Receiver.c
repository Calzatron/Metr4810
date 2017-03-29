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
 //#include <util/delay.h>
 //#include <string.h>


 static volatile uint8_t receivedByte;




 ISR(USART0_RX_vect){
	// code to execute when the USART receives a byte here
	//uint8_t recievedByte;
	receivedByte = UDR0;

	// if statements here


 }


 void USART_init(unsigned int ubrr){
	 // UBRR0 is split into two; store the high 4 bits of the baud rate
	 // in UBRR0H and then rest in UBRR0L
	 receivedByte = 0x00;
	 
	 UBRR0H = (unsigned char)(ubrr>>8);
	 UBRR0L = (unsigned char)(ubrr);

	 // set up receiver on interrupt
	 UCSR0B |= (1<<RXEN0);
	 UCSR0B |= (1<<RXCIE0);
	 //UCSR0A |= (1<<TXC);

	 // control status register, keep asynchronous, parity disabled, 8-bit
	 UCSR0C = (1<<UCSZ11)|(1<<UCSZ10);

 }