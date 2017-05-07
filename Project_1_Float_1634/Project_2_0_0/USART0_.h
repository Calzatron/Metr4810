/*
 * USART0_.h
 *
 * Created: 17/04/2017 8:09:28 PM
 *  Author: s4357594
 */ 


#ifndef USART0__H_
#define USART0__H_

#include <stdint.h>

/* Initialise serial IO using the UART. baudrate specifies the desired
 * baudrate (e.g. 19200) and echo determines whether incoming characters
 * are echoed back to the UART output as they are received (zero means no
 * echo, non-zero means echo)
 */
void init_serial_stdio(long baudrate, int8_t echo);

/* Test if input is available from the serial port. Return 0 if not,
 * non-zero otherwise.
 */
int8_t serial_input_available(void);

/* Discard any input waiting to be read from the serial port. (Characters may
 * have been typed when we didn't want them - clear them.
 */
void clear_serial_input_buffer(void);



#endif /* USART0__H_ */