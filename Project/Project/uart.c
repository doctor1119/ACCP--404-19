#include "uart.h"
#include <avr/io.h>
#define BAUD_RATE 9600
#define UBRR_VALUE F_CPU/BAUD_RATE/16 - 1
#define BUFFER_SIZE 10

volatile char rx_buffer[BUFFER_SIZE];
volatile uint8_t rx_buffer_index = 0;

ISR(USART_RX_vect)
{
	char data_s_t = UDR0;
	if (rx_buffer_index < BUFFER_SIZE) {
		rx_buffer[rx_buffer_index] = data_s_t;
		rx_buffer_index++;
		} else {
		// Обработка переполнения буфера
	}
}

void USART_Init(unsigned int UBRR) {
	UBRR0H = (unsigned char)(UBRR>>8);
	UBRR0L = (unsigned char)UBRR;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void send(uint8_t data) {
	char str[4];
	sprintf(str, "%d", data);  // преобразует число в строку
	for (uint8_t i = 0; str[i] != '\0'; i++) {
		while (!(UCSR0A & (1<<UDRE0)));  // ждет, пока UDR0 освободится
		UDR0 = str[i];  // отправляет символ
	}
}