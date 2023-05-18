#ifndef UART_H
#define UART_H
#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define UBRR_VALUE F_CPU/BAUD_RATE/16 - 1

#include <stdint.h>

// Прототипы функций
void USART_Init(unsigned int UBRR);
void send(uint8_t data);
uint8_t overwriting(uint8_t data);
uint8_t uart_read(void);
uint8_t start_stop_allow(void);

// Глобальные переменные
extern volatile uint8_t rx_buffer[1];
extern volatile uint8_t rx_buffer_index;
extern volatile uint8_t tx_buffer[3];
extern volatile uint8_t tx_buffer_index;
extern uint8_t Data_to_share;
extern uint8_t start_stop;

#endif // UART_H
