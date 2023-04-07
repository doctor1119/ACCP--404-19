#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "save.h"

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define UBRR_VALUE ((F_CPU / (BAUD_RATE * 16UL)) - 1)

volatile uint8_t rx_buffer[2];
volatile uint8_t rx_buffer_index = 0;
volatile uint8_t tx_buffer[3]; //BUFFER_SIZE пока равен 3
volatile uint8_t tx_buffer_index = 0;

void uart_init(void) {
	UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
	UBRR0L = (uint8_t)UBRR_VALUE;
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data, 1 stop bit, no parity
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // Enable RX, TX and RX interrupt
	
}

ISR(USART_RX_vect) {
	uint8_t data = UDR0; 
	rx_buffer[rx_buffer_index++] = data;
	if (rx_buffer_index == 2) {
		rx_buffer_index = 0;
	}
	//process_command(data);
}

void uart_send(uint8_t data) {
	char tx_b[3];
	for(int a = 0; a<2; a++)
	{
		tx_buffer[a] = 0;
	}
	
	int c;
	int i = 3;
	while(i != 0){
		c=data%10;
		tx_b[i]=c;
		tx_buffer[i] = c;
		i--;
		data/=10;
	}
	/*tx_buffer[tx_buffer_index++] = data;
	if (tx_buffer_index == 3) {
		tx_buffer_index = 0;
	}*/
	for(int a = 0; a <2; a++){
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = tx_b[a];
	}
}
uint8_t uart_read(void) {
	uint8_t data = rx_buffer[rx_buffer_index];
	rx_buffer_index++;
	if (rx_buffer_index == 2) {
		rx_buffer_index = 0;
	}
	return data;
}

