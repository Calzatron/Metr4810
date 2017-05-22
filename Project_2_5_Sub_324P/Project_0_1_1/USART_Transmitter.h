/*
 * USART_Transmitter.h
 *
 * Created: 24/04/2017 11:49:23 PM
 *  Author: s4357594
 */ 



#ifndef USART_TRANSMITTER_H_
#define USART_TRANSMITTER_H_


void USART_transmit_init(long ubrr);

//unsigned char input(void);

void output(unsigned char serialOut);


#endif /* USART_TRANSMITTER_H_ */