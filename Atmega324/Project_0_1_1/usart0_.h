/*
 * USART0_.h
 *
 * Created: 17/04/2017 6:50:36 PM
 *  Author: s4357594
 */ 


#ifndef USART0__H_
#define USART0__H_

unsigned char input(void);

void USART0_init(long ubrr);

void output_char(unsigned char serialOut);

void clear_serial_input_buffer(void);

void output_string(unsigned char* serialOutString);


#endif /* USART0__H_ */