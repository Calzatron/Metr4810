/*
 * Working code v1.c
 *
 * Created: 10/05/2016 12:23:38 PM
 * Author : s4323183
 */ 

//// Definitions
// define cpu clock speed
// define some macros
// define baud
// set baud rate value for UBRR
#define F_CPU 8000000L
#define BAUD 9600                                   
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            

//// define included libraries 
#include <avr/io.h>
#include <util/delay.h>

//// function that creates a delay based on an input variable
// Is used to allow for finer control of the craft
// A lower power level setting results in larger delays between motor operation
void my_delay_ms(int ms)
{
	while(0 < ms)
	{
		_delay_ms(1);
		--ms;
	}
}

//// function that returns serial data read from the buffer
unsigned char UART_Receive(void)
{
	/* Wait for data to be received */
	/* Get and return received data from buffer */
	while ( !(UCSR0A & (1<<RXC0)) );
	return UDR0;
}

//// function that initializes serial communication
void serial_init(void)
{
	// shift the register right by 8 bits
	// set baud rate
	// enable receiver
	// 8bit data format
	// Remap reciever to PB2
	UBRR0H = (BAUDRATE>>8);
	UBRR0L = BAUDRATE;
	UCSR0B|= (1<<RXEN0);
	UCSR0C|= (1<<UCSZ01)|(1<<UCSZ00);
	REMAP |=  (1<<U0MAP);
}

//// function that initializes the PWM (servo) and Digital I/O (motor control) outputs
void servo_motor_init(void)
{
	// PWM init
	// TOCC2 = 0C2B (10)
	// Enable TOCC2
	// Enable PA3 OUTPUT for servo, other outputs for motors
	// Fast PWM, mode 14, non inverting, pre-scaler clk/8
	// Set Top for 20ms Period
	// Start servo at full open
	TOCPMSA0 |= (1<<TOCC2S1);
	TOCPMCOE |= (1<<TOCC2OE);
	DDRA = (1<<PORTA3)|(1<<PORTA2)|(1<<PORTA1)|(1<<PORTA0);
	DDRB = (1<<PORTB1);	
	TCCR2A |= (1<<COM2B1)|(1<<WGM21);
	TCCR2B |= (1<<CS21)|(1<<WGM23)|(1<<WGM22);
	ICR2 = 19999;
	OCR2B = 1600;
}

int main(void)
{
	servo_motor_init();
	serial_init();
	// Define the integer used for setting the PWM width
	int servo_input = 1600; //min 1050, max 1700
	// Define the integer used for setting the motor power level
	int power = 10;
		
	//// Initialize in stand-by mode
	// Motor 1 (Bottom Motor)
	PORTA &= ~(1<<PORTA1);
	PORTA &= ~(1<<PORTA2);
	
	// Motor 2 (Top Motor)
	PORTA &= ~(1<<PORTA0);
	PORTB &= ~(1<<PORTB1);
	
	//// Running while loop for vehicle control
	// Takes input from serial and controls motors and servo
	while (1)
	{
		//// Receives and assigns received serial data to RD
		_delay_ms(10);
		UART_Receive();
		char RD = UDR0;	
		
		//// Setting Power
		// Power ranges from 1 to 5, 5 = full power,  1 = maximum fine motor control
		// This controls a delay function later on that stutters the motors based on the delay amount
		// This was implemented to allow for greater fine motor control
		if (RD == '1')
		{
			power = 1;
		}
		else if (RD == '2')
		{
			power = 3;
		}
		else if (RD == '3')
		{
			power = 5;
		}
		else if (RD == '4')
		{
			power = 10;
		}
		else if (RD == '5')
		{
			power = -1;
		}
		
		//////// Motor control
		//// General standby mode (left motor B, right motor B, Servos hold angle)
		else if (RD == 'x')
		{
			// Motor 1 (Bottom Motor - SB)
			PORTA &= ~(1<<PORTA1);
			PORTA &= ~(1<<PORTA2);
			
			// Motor 2 (Top Motor - SB)
			PORTA &= ~(1<<PORTA0);
			PORTB &= ~(1<<PORTB1);
		}
		
		//// Forward drive (left motor F, right motor F)
		else if (RD == 'w')
		{	
			//// Motor 1 (Bottom Motor - F)
			PORTA |= (1<<PORTA1);
			PORTA &= ~(1<<PORTA2);
			
			//// Motor 2 (Top Motor - F)
			PORTA |= (1<<PORTA0);
			PORTB &= ~(1<<PORTB1);
		}
		
		//// Reverse drive (left motor R, right motor R)
		else if(RD == 's')
		{
			//// Motor 1 (Bottom Motor - R)
			PORTA &= ~(1<<PORTA1);
			PORTA |= (1<<PORTA2);
			
			//// Motor 2 (Top Motor - R)
			PORTA &= ~(1<<PORTA0);
			PORTB |= (1<<PORTB1);
		}
		
		//// Left turn (left motor R, right motor F)
		else if(RD == 'a')
		{
		//// Motor 1 (Bottom Motor - F)
		PORTA |= (1<<PORTA1);
		PORTA &= ~(1<<PORTA2);
		
		//// Motor 2 (Top Motor - R)
		PORTA &= ~(1<<PORTA0);
		PORTB |= (1<<PORTB1);	
		}
		
		//// Right turn (left motor F, right motor R)
		else if(RD == 'd')
		{
			// Motor 1 (Bottom Motor - R)
			PORTA &= ~(1<<PORTA1);
			PORTA |= (1<<PORTA2);
			
			// Motor 2 (Top Motor - F)
			PORTA |= (1<<PORTA0);
			PORTB &= ~(1<<PORTB1);
		}
		
		//////// Gripper control
		//// Tightens gripper
		// Limits tightening action to not over-work the servo
		else if (RD == 'q')
		{
			if (servo_input > 1050)
			{
				servo_input -= 50;
			}
				OCR2B = servo_input;
		}
		
		//// Loosens gripper
		// Limits loosening action to not let the gripper over extend
		else if (RD == 'e')
		{
			if (servo_input < 1700)
			{
				servo_input += 50;
			}
			OCR2B = servo_input;
		}
		
		//// If no input is received, left motor brake, right motor brake, servos hold angle
		else
		{
			// Motor 1 (Bottom Motor - SB)
			PORTA &= ~(1<<PORTA1);
			PORTA &= ~(1<<PORTA2);
			
			// Motor 2 (Top Motor - SB)
			PORTA &= ~(1<<PORTA0);
			PORTB &= ~(1<<PORTB1);
		}
		
		//// Based on the power level setting
		// Runs a delay that allows for greater fine motor control
		if(power != -1)
		{
			my_delay_ms(power);
			PORTA &= ~(1<<PORTA1);
			PORTA &= ~(1<<PORTA2);
			
			PORTA &= ~(1<<PORTA0);
			PORTB &= ~(1<<PORTB1);	
		}		
	}
}

