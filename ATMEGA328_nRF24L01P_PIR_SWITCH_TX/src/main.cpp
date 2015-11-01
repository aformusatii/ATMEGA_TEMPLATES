/********************************************************************************
Includes
********************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "mtimer.h"

#include "RF24.h"

#ifdef	__cplusplus
extern "C" {
#endif
#include "usart.h"

#ifdef	__cplusplus
}
#endif

/********************************************************************************
	Macros and Defines
********************************************************************************/
#define CONSOLE_PREFIX "\natmega328>"

#define _in(bit,port)	port &= ~(1 << bit)
#define _out(bit,port)	port |= (1 << bit)
#define _on(bit,port)	port |= (1 << bit)
#define _off(bit,port)	port &= ~(1 << bit)

#define _to_uint64(x,y) ((uint64_t) x << 16) | y

#define GET_REG1_FLAG(bit) (1 << bit) & reg1_flags
#define SET_REG1_FLAG(bit) reg1_flags |= (1 << bit)
#define CLR_REG1_FLAG(bit) reg1_flags &= ~(1 << bit)

#define UART_CMD_RECEIVED  0
#define UNSUPPORTED_CMD_RECEIVED  1
#define RF24_RECEIVER_ENABLED  2

/********************************************************************************
	Function Prototypes
********************************************************************************/

void processUsartCmd();
void testTimer(uint16_t value);

/********************************************************************************
	Global Variables
********************************************************************************/

volatile uint8_t usart_cmd_buffer[255];
volatile uint8_t usart_cmd_buffer_count = 0;

volatile uint8_t reg1_flags = 0;

volatile uint64_t endJob1Cicles = 0;
volatile uint16_t job1Seconds = 1;

RF24 radio;
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

/********************************************************************************
	Interrupt Service
********************************************************************************/

ISR(USART_RX_vect)
{
	uint8_t usart_data = UDR0;

	switch (usart_data) {
		case 13:
			SET_REG1_FLAG(UART_CMD_RECEIVED);
			break;

		case 127:
			SET_REG1_FLAG(UNSUPPORTED_CMD_RECEIVED);
			break;

		default:
			usart_putchar(usart_data);
			usart_cmd_buffer[usart_cmd_buffer_count++] = usart_data;
	}
}

ISR(TIMER1_OVF_vect)
{
	incrementOvf();
}

/********************************************************************************
	Main
********************************************************************************/
int main(void) {
    // initialize code
	usart_init();

    // enable interrupts
    sei();

    // Init timer1
    initTimer();

	// Output initialization log
    printf("Start ATMEGA328.");
    printf(CONSOLE_PREFIX);

    printf("Start NRF24L01P test...");

    radio.begin();
    radio.setRetries(15,15);
    radio.setPayloadSize(8);
    radio.setPALevel(RF24_PA_MAX);
    radio.setChannel(120);

    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);

    radio.startListening();

    radio.printDetails();

    radio.stopListening();

    // init job1
    endJob1Cicles = 1;

	// main loop
    while (1) {

    	/**
    	 * --------> USART Unsupported command received.
    	 */
    	if (GET_REG1_FLAG(UNSUPPORTED_CMD_RECEIVED)) {
			printf("<BACKSPACE NOT SUPPORTED>");
			printf(CONSOLE_PREFIX);
			usart_cmd_buffer_count = 0;
    		CLR_REG1_FLAG(UNSUPPORTED_CMD_RECEIVED);
    	}

    	/**
    	 * --------> USART Command received.
    	 */
    	if (GET_REG1_FLAG(UART_CMD_RECEIVED)) {
			processUsartCmd();
			printf(CONSOLE_PREFIX);
			usart_cmd_buffer_count = 0;
    		CLR_REG1_FLAG(UART_CMD_RECEIVED);
    	}

    	uint64_t currentTimeCicles = getCurrentTimeCicles();

    	/**
    	 * --------> Job 1 (Send data to server)
    	 */
    	if ((endJob1Cicles != 0) && (currentTimeCicles >= endJob1Cicles)) {
    		endJob1Cicles = convertSecondsToCicles(job1Seconds);

    		printf("\n job 1 execute");

			uint8_t data[] = {111};
			bool ok = radio.write(data, 1);

		    if (ok)
		      printf("ok...\n\r");
		    else
		      printf("failed.\n\r");
    	}

    }
}

/**
 * Processes the Command received via USART interface.
 */
void processUsartCmd() {

	if (usart_cmd_buffer_count > 0) {

		/**
		 * test command
		 */
		const char *testCmd = "test ";
		size_t cmd_size = strlen(testCmd);

		if (strncmp((char*) usart_cmd_buffer, testCmd, cmd_size) == 0) {
			char args[usart_cmd_buffer_count - cmd_size];
			for (uint8_t i = cmd_size; i < usart_cmd_buffer_count; i++) {
				args[i - cmd_size] = usart_cmd_buffer[i];
			}

			uint16_t value = atoi(args);
			testTimer(value);
			return;
		}

		/**
		 * setJob1 command
		 */
		const char *countCmd = "setJob1 ";
		cmd_size = strlen(countCmd);

		if (strncmp((char*) usart_cmd_buffer, countCmd, cmd_size) == 0) {
			char args[usart_cmd_buffer_count - cmd_size];
			for (uint8_t i = cmd_size; i < usart_cmd_buffer_count; i++) {
				args[i - cmd_size] = usart_cmd_buffer[i];
			}

			uint16_t value = atoi(args);
			endJob1Cicles = convertSecondsToCicles(value);
			job1Seconds = value;
			return;
		}

		/**
		 * sendData command
		 */
		const char *sendCmd = "send ";
		cmd_size = strlen(sendCmd);

		if (strncmp((char*) usart_cmd_buffer, sendCmd, cmd_size) == 0) {
			char args[usart_cmd_buffer_count - cmd_size];
			for (uint8_t i = cmd_size; i < usart_cmd_buffer_count; i++) {
				args[i - cmd_size] = usart_cmd_buffer[i];
			}

			uint16_t value = atoi(args);

			radio.stopListening();

			uint8_t data[] = {111};
			bool ok = radio.write(data, 1);

		    if (ok)
		      printf("ok...\n\r");
		    else
		      printf("failed.\n\r");

		    radio.startListening();

			return;
		}

		printf("\n Unknown command.");
	}
}

/**
 * Usart test function.
 */
void testTimer(uint16_t value) {
	printf("\n testTimer(%d)", value);

	uint64_t startTime = getCurrentTimeCicles();
	_delay_ms(value);
	uint32_t elapsedMillis = getElapsedMilliseconds(startTime);

	printf("\n diffMillis = %ld", elapsedMillis);
}
