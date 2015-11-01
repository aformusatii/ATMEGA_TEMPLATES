/********************************************************************************
	Includes
********************************************************************************/
#include "ATtiny85.h"

/********************************************************************************
	Global Variables
********************************************************************************/
volatile uint64_t startTime = 0;

/* ======================================================= */
// Set up a memory regions to access GPIO
void setup_io()
{
	_out(DDB3, DDRB); // CSN
	_out(DDB4, DDRB); // CE
	_out(DDB2, DDRB); // SCK
	_out(DDB1, DDRB); // MOSI (DO)
	_in(DDB0, DDRB); // MISO (DI)
} // setup_io

/* ======================================================= */
// Set up SPI interface
void setup_spi()
{
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
} // setup_spi

/* ======================================================= */
void setCSN(uint8_t value)
{
	if (value) {
		_on(PB3, PORTB);
	} else {
		_off(PB3, PORTB);
	}
}

/* ======================================================= */
void setCE(uint8_t value)
{
	if (value) {
		_on(PB4, PORTB);
	} else {
		_off(PB4, PORTB);
	}
}

/* ======================================================= */
// SPI transfer
uint8_t transfer_spi(uint8_t tx_)
{
	/* Start transmission */
	SPDR = tx_;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	/* Return data register */
	return SPDR;
} // transfer_spi

/* ======================================================= */
void __start_timer()
{
	startTime = getCurrentTimeCicles();
}

/* ======================================================= */
uint16_t __millis()
{
	return (uint16_t) getElapsedMilliseconds(startTime);
}

