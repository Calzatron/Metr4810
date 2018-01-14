
#include "usart0_.h"

#include <avr/io.h>
#include <avr/interrupt.h>			//enables interrupts
#include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
#include <stdio.h>					//input / output
#include <stdlib.h>					//standard function library; malloc / sort / rand etc


static volatile char transmitByte;
static volatile char receivedByte;
 

void USART0_init(long ubrr){
	// UBRR0 is split into two; store the high 4 bits of the baud rate
	// in UBRR0H and then rest in UBRR0L
	receivedByte = 0x00;
	transmitByte = 0x00;

	UBRR1H = (ubrr>>8);
	UBRR1L = (ubrr);

	// set up receiver and transmitter
	UCSR1B |= (1<<TXEN1);//|(1<<RXEN1);
	// enable interrupt reciever and transmitter
	UCSR1B |= (1<<TXCIE1);//|(1<<RXCIE1);
	//UCSR0A |= (1<<TXC);

	// control status register, keep asynchronous, parity disabled, 8-bit
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);
}



void output_char(char serialOut){
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



///*
 //* USART0_.c
 //*
 //* Created: 17/04/2017 6:46:32 PM
 //*  Author: s4357594
 //*/ 
//
 //
//
 //#include <avr/io.h>
 //#include <avr/interrupt.h>			//enables interrupts
 //#include <avr/pgmspace.h>			//for storing / accessing data in flash program instead of SRAM -> variable manipulation
 //#include <stdio.h>					//input / output
 //#include <stdlib.h>					//standard function library; malloc / sort / rand etc
 //#include "usart0_.h"
 //#include "Project.h"
//
 //#define OUTPUT_BUFFER_SIZE 255
 //volatile char output_buffer[OUTPUT_BUFFER_SIZE];
 //volatile uint8_t output_insert_pos;
 //volatile uint8_t bytes_in_output_buffer;
 ////volatile uint8_t string_flag;
//
//
 //#define INPUT_BUFFER_SIZE 16
 //volatile char input_buffer[INPUT_BUFFER_SIZE];
 //volatile uint8_t input_insert_pos;
 //volatile uint8_t bytes_in_input_buffer;
 //volatile uint8_t input_overrun;
//
 //static volatile char transmitByte;
 //static volatile char receivedByte;
//
//
 //void USART0_init(long ubrr){
	 //// UBRR0 is split into two; store the high 4 bits of the baud rate
	 //// in UBRR0H and then rest in UBRR0L
	 //receivedByte = 0x00;
	 //transmitByte = 0x00;
	 ////string_flag = 0;
	 //output_insert_pos = 0;
	 //bytes_in_output_buffer = 0;
	 //input_insert_pos = 0;
	 //bytes_in_input_buffer = 0;
	 //input_overrun = 0;
//
	 //UBRR1H = (ubrr>>8);
	 //UBRR1L = (ubrr);
//
	 //// set up receiver and transmitter
	 //UCSR1B |= (1<<TXEN1)|(1<<RXEN1);
	 //// enable interrupt reciever and transmitter
	 //UCSR1B |= (1<<TXCIE1)|(1<<RXCIE1);
	 ////UCSR0A |= (1<<TXC);
//
	 //// control status register, keep asynchronous, parity disabled, 8-bit
	 //UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);
 //}
//
//
////void set_string_flag(void){
////
	////if (string_flag){
		////string_flag = 0;
	////} else {
		////string_flag = 1;
	////}
////
////}
//
//
////int8_t serial_input_available(void) {
	 ////return (bytes_in_input_buffer != 0);
 ////}
////
////
 ////void clear_serial_input_buffer(void) {
	 /////* Just adjust our buffer data so it looks empty */
	 ////input_insert_pos = 0;
	 ////bytes_in_input_buffer = 0;
 ////}
//
//
////char input(void){
	/////* Returns the latest value received in buffer	*/
		////uint8_t interrupts_on = bit_is_set(SREG, SREG_I);
		////cli();
////
////
		////char returnValue = receivedByte;
		////receivedByte = 0x00;
		////if(interrupts_on){
			////sei();
		////}
////
		////return returnValue;
////
/////////////////////////////////////////////////////////////////////
	//////if (bytes_in_input_buffer == 0){
		//////return 0x00;
	//////return receivedByte;
	//////}
//////
	//////uint8_t interrupts_on = bit_is_set(SREG, SREG_I);
	//////cli();
//////
	//////if(input_insert_pos - bytes_in_input_buffer < 0) {
		///////* Need to wrap around */
		//////receivedByte = input_buffer[input_insert_pos - bytes_in_input_buffer
		//////+ INPUT_BUFFER_SIZE];
		//////} else {
		//////receivedByte = input_buffer[input_insert_pos - bytes_in_input_buffer];
	//////}
	 //////
	///////* Decrement our count of bytes in the input buffer */
	//////bytes_in_input_buffer--;
//////
	//////char returnValue = receivedByte;
	//////receivedByte = 0x00;
	//////if(interrupts_on){
		//////sei();
	//////}
	//////return returnValue;
////}
//
////
////void output_char(unsigned char serialOut){
	/////*	adding bits to the buffer to transmit */
	////// disable interrupts so half modified buffer isn't used
	////uint8_t interrupts_on = bit_is_set(SREG, SREG_I);
	////cli();
////
	////// copy the byte to transmit
////
	////transmitByte = serialOut;
////
////
	////// re-enable UART
	////UCSR0B |= (1<<UDRIE0);
	////// re-enable interrupts
	////if(interrupts_on){
		////sei();
	////}
////}
//
//void output_char(char serialOut){//, int string){
	///*	adding bits to the buffer to transmit */
	////disable interrupts so half modified buffer isn't used
	////uint8_t clear = 0;
	////if ((string = 1) && (string_flag = 0)){
		////set_string_flag();
	////} else if ((string = 0) && (string_flag = 1)){
		////clear = 1;
	////}
////
//////////////////////////////////////////////////////////////////////
	  //uint8_t interrupts_on = bit_is_set(SREG, SREG_I);
	  //cli();
//
	  //// copy the byte to transmit
//
	  //transmitByte = serialOut;
//
//
	  //// re-enable UART
	  //UCSR1B |= (1<<UDRIE1);
	  //// re-enable interrupts
	  //if(interrupts_on){
		  //sei();
	  //}
//////////////////////////////////////////////////////////////////
	////uint8_t interrupts_enabled = bit_is_set(SREG, SREG_I);
	////while(bytes_in_output_buffer >= OUTPUT_BUFFER_SIZE) {
	////
		/////* else do nothing */
	////}
	////
	/////* Add the character to the buffer for transmission if there
	 ////* is space to do so. We advance the insert_pos to the next
	 ////* character position. If this is beyond the end of the buffer
	 ////* we wrap around back to the beginning of the buffer 
	 ////* NOTE: we disable interrupts before modifying the buffer. This
	 ////* prevents the ISR from modifying the buffer at the same time.
	 ////* We reenable them if they were enabled when we entered the
	 ////* function.
	////*/	
////
	////cli();
	////output_buffer[output_insert_pos++] = serialOut;
	////bytes_in_output_buffer++;
	////if(output_insert_pos == OUTPUT_BUFFER_SIZE) {
		/////* Wrap around buffer pointer if necessary */
		////output_insert_pos = 0;
	////}
	/////* Reenable interrupts (UDR Empty interrupt may have been
	 ////* disabled) */
	////UCSR0B |= (1 << UDRIE0);
	////if(interrupts_enabled) {
		////sei();
	////}
//
//}
//
////unsigned char input(void){
	/////* Returns the latest value received in UDR1	*/
	   ////
	////uint8_t interrupts_on = bit_is_set(SREG, SREG_I);
	////cli();
////
////
	////unsigned char returnValue = receivedByte;
	////receivedByte = 0x00;
	////if(interrupts_on){
		////sei();
	////}
////
	////return returnValue;
////}
////void clear_output(void){
////
	////for (int x = 0; x < OUTPUT_BUFFER_SIZE; x++){
		////output_buffer[x] = 0;
	////}
	////output_insert_pos = 0;
////
////}
//
//
////void output_string(char serialOutString[]){
	////
	////for (int c = 0; c < strlen(serialOutString); c++){
		//////custom_delay(10);
		////output_char(serialOutString[c], 1);
	////}
////
	////for (int x = 0; x < OUTPUT_BUFFER_SIZE; x++){
		////output_buffer[x] = 0;
	////}
	////output_insert_pos = 0;
	//////while (*serialOutString){
		//////output_char(*serialOutString++);
	//////}
//////PORTA = 0xFF;
////}
//
//
//
 //ISR(USART1_UDRE_vect){
//////////////////////////////////////////////
	//if (transmitByte != 0x00){
		//UDR1 = transmitByte;
		//transmitByte = 0x00;
	//} else {
		//// disable UART as transmission complete
		//UCSR1B &= ~(1<<UDRIE1);
	//}
//////////////////////////////////////////////
	///* Check if we have data in our buffer */
	////if(bytes_in_output_buffer > 0) {
	//////if(output_insert_pos - bytes_in_output_buffer >= 0){
		/////* Yes we do - remove the pending byte and output it
			////* via the UART. The pending byte (character) is the
			////* one which is "bytes_in_buffer" characters before the 
			////* insert_pos (taking into account that we may 
			////* need to wrap around to the end of the buffer).
			////*/
		////char c;
		//////if(output_insert_pos - bytes_in_output_buffer < 0) {
			///////* Need to wrap around */
			//////c = output_buffer[output_insert_pos - bytes_in_output_buffer
				//////+ OUTPUT_BUFFER_SIZE];
			//////
			////////output_buffer[output_insert_pos - bytes_in_output_buffer
				////////+ OUTPUT_BUFFER_SIZE] = '\0';
				//////
		//////} else {
			////c = output_buffer[output_insert_pos - bytes_in_output_buffer];
			//////output_buffer[output_insert_pos - bytes_in_output_buffer] = '\0';
		//////}
		/////* Decrement our count of the number of bytes in the 
			////* buffer 
			////*/
		////bytes_in_output_buffer--;
		//////bytes_in_output_buffer = 0x00;
		////
		/////* Output the character via the UART */
		////UDR0 = c;
	////} else {
		/////* No data in the buffer. We disable the UART Data
			////* Register Empty interrupt because otherwise it 
			////* will trigger again immediately this ISR exits. 
			////* The interrupt is re-enabled when a character is
			////* placed in the buffer.
			////*/
		////UDR0 = 0x00;
		////UCSR0B &= ~(1<<UDRIE0);
	////}
 //}
//
//
//
////
////ISR(USART0_RX_vect){
	//////code to execute when the USART receives a byte here
////
	////receivedByte = UDR0;
////
///////////////////////////////////////////////////////////////
////
	//////char c;
	//////c = UDR0;
//////
	//////if(bytes_in_input_buffer >= INPUT_BUFFER_SIZE) {
		//////input_overrun = 1;
	//////} else {
		///////* If the character is a carriage return, turn it into a
		 //////* linefeed 
		//////*/
		//////if (c == '\r') {
			//////c = '\n';
		//////}
		//////
		///////* 
		 //////* There is room in the input buffer 
		 //////*/
		//////input_buffer[input_insert_pos++] = c;
		//////bytes_in_input_buffer++;
		//////if(input_insert_pos == INPUT_BUFFER_SIZE) {
			///////* Wrap around buffer pointer if necessary */
			//////input_insert_pos = 0;
		//////}
	//////}
////}