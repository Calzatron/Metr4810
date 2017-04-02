/*
 * USART_Receiver.h
 *
 * Created: 24/03/2017 3:42:27 PM
 *  Author: s4357594
 */ 

 #include <stdint.h>



#ifndef USART_RECEIVER_H_
#define USART_RECEIVER_H_

void USART_init(unsigned int ubrr);

uint8_t input(void);

void output(uint8_t serialOut);

#endif /* USART_RECEIVER_H_ */