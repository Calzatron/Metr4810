
 /*
 * USART_Transmitter.c
 *
 * Created: 9/04/2017 2:59:04 PM
 *  Author: s4357594
 */ 

  //#include "USART_Receiver.h"

  #include <avr/io.h>
  #include <avr/interrupt.h>			//enables interrupts
  #include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
  #include <stdio.h>					//input / output
  #include <stdlib.h>					//standard function library; malloc / sort / rand etc

  void USART_transmit_init(long ubrr);

  static volatile unsigned char transmitByte;
  static volatile unsigned char receivedByte;


  void USART_transmit_init(long ubrr){
	  // UBRR0 is split into two; store the high 4 bits of the baud rate
	  // in UBRR0H and then rest in UBRR0L
	  receivedByte = 0x00;
	  transmitByte = 0x00;

	  UBRR1H = (ubrr>>8);
	  UBRR1L = (ubrr);

	  // set up receiver and transmitter
	  UCSR1B |= (1<<TXEN1)|(1<<RXEN0);
	  // enable interrupt reciever and transmitter
	  UCSR1B |= (1<<TXCIE1)|(1<<RXCIE0);
	  //UCSR0A |= (1<<TXC);

	  // control status register, keep asynchronous, parity disabled, 8-bit
	  UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);
  }



  void output(unsigned char serialOut){
	  /*	adding bits to the buffer to transmit */
	  // disable interrupts so half modified buffer isn't used
	  uint8_t interrupts_on = bit_is_set(SREG, SREG_I);
	  cli();

	  // copy the byte to transmit

	  transmitByte = serialOut;


	  // re-enable UART
	  UCSR1B |= (1<<UDRIE1);
	  // re-enable interrupts
	  if(interrupts_on){
		  sei();
	  }
  }


ISR(USART1_UDRE_vect){

	  // send the transmitByte
	if (transmitByte != 0x00){
		  UDR1 = transmitByte;
		  transmitByte = 0x00;
	} else {
		  // disable UART as transmission complete
		  UCSR1B &= ~(1<<UDRIE1);
	}
}