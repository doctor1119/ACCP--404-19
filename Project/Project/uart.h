#ifndef UART_H
#define UART_H

#include <avr/io.h>

#define BAUD_RATE 9600
#define UBRR_VALUE F_CPU/BAUD_RATE/16 - 1
#define BUFFER_SIZE 10

extern volatile char rx_buffer[BUFFER_SIZE];
extern volatile uint8_t rx_buffer_index;

void USART_Init(unsigned int UBRR);
void send(uint8_t data);

#endif //UART_H