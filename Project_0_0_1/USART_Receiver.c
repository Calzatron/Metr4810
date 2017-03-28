/*
 * USART_Receiver.c
 *
 * Created: 24/03/2017 3:06:43 PM
 *  Author: s4357594
 */ 


 ISR(USART0_RX_vect){
	// code to execute when the USART receives a byte here
	char recievedByte;
	recievedByte = UDR0;
 }


 void USART_init(unsigned int ubrr){
	 // UBRR0 is split into two; store the high 4 bits of the baud rate
	 // in UBRR0H and then rest in UBRR0L
	 UBRR0H = (unsigned char)(ubrr>>8);
	 UBRR0L = (unsigned char)(ubrr);

	 // set up receiver on interrupt
	 UCSR0B |= (1<<RXEN0);
	 UCSR0B |= (1<<RXCIE0);
	 //UCSR0A |= (1<<TXC);

	 // control status register, keep asynchronous, parity disabled, 8-bit
	 UCSR0C = (1<<UCSZ11)|(1<<UCSZ10);

 }