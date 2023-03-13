
#include <xc.h>
#include <avr/interrupt.h>
#include <avr/io.h>


ISR(USART_RX_vect)
{
	
}


void uart_init(unsigned int baud)
{
	UBRR0L = (unsigned int)baud;
	UBRR0H = (unsigned int)(baud>>8);
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0); // Receiver/Transmitter
	UCSR0B |= (1<<RXCIE0); // ISR(USART_RX_vect)
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01); // 8 bit
	UCSR0C |= (1<<UMSEL00) | (1<<UMSEL01); // Asynchronous USART
	UCSR0C |= (1<<USBS0); // 1 stop-bit
	SREG |= (1<<7); // Global Interrupt
	
}


void uart_send()
{
	while((~UCSR0A&(1<<UDRE0))); // check UDR0
}