/********************************************************************************
	Includes
********************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

/********************************************************************************
Macros and Defines
********************************************************************************/
#define _in(bit,port)	port &= ~(1 << bit)
#define _out(bit,port)	port |= (1 << bit)
#define _on(bit,port)	port |= (1 << bit)
#define _off(bit,port)	port &= ~(1 << bit)

/********************************************************************************
	Main
********************************************************************************/
int main(void) {
	// init GPIO
	_out(DDB3, DDRB); // CSN
	while (1) {
		_on(PB3, PORTB);
		_delay_ms(1000);
		_off(PB3, PORTB);
		_delay_ms(1000);
	}
}
