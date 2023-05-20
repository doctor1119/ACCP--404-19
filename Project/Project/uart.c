#include <xc.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include "uart.h"

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define UBRR_VALUE F_CPU/BAUD_RATE/16 - 1

volatile uint8_t rx_buffer[1]; //Массив для записи температуры, полученной с компьютера
volatile uint8_t rx_buffer_index = 0; //Индекс массива для записи температуры, полученной с компьютера

//Оставить до испытания
volatile uint8_t tx_buffer[3] = {0, 0, 0};
volatile uint8_t tx_buffer_index = 0;
//Оставить до испытания

uint8_t Data_to_share = 0; //Температура, отправляемая в блок commands
uint8_t start_stop = 0; //Переменная для включения-выключения работы программы датчика температуры с компьютера (0 - выключена, 1 - включена)
uint8_t overwriting(uint8_t data);

ISR(USART_RX_vect)
{	
	char Data_s_t = UDR0; // Записываем данные, полученные с компьютера
	
	/*if (Data_s_t == 's')// Если с компьютера приходит символ s увеличиваем/уменьшаем значение start_stop (включаем/выключаем программу работы с датчиком)
	{
		start_stop++;
		if (start_stop > 1)
		{
			start_stop = 0;
		}
	}
	else // Если с компьютера приходит любое другое значение, расцениваем его, как температуру
	{*/
		uint8_t data = atoi(Data_s_t); // Переводим данные, полученные с компьютера, в численное значение
		
		rx_buffer[rx_buffer_index] = (uint8_t)Data_s_t  + 1;
		while (!(UCSR0A & (1<<UDRE0))); // Проверяем буфер перед отправкой
		UDR0 = rx_buffer[rx_buffer_index];
		
		if (rx_buffer_index == 1)
		{			
			rx_buffer_index = 0;
			Data_to_share = rx_buffer[0]*10 + rx_buffer[1];
		}
		rx_buffer_index++;
	//}
	
}

uint8_t start_stop_allow() // Функция для передачи значения start_stop в другие модули
{
	return start_stop;
}

void USART_Init(unsigned int UBRR) // Функция инициализации ЮСАРТа
{
	// Устанавливаем скорость передачи
	UBRR0H = (unsigned char)(UBRR>>8);
	UBRR0L = (unsigned char)UBRR;
	
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0); // Разрешаем чтение, запись и прирывания для ЮСАРТа
	UCSR0C = (1<<USBS0)|(3<<UCSZ00); // 8 bit, 1 stopbit
}

void send(uint8_t data) // Функция отправки данных по ЮСАРТ
{	
	data = fabs(data); // Берем модуль от значения температуры
	data = overwriting(data); //Переписываем значение температуры 1234 -> 4321. Так надо.
	uint8_t c; // В c будем записывать по очереди цифры из data для отправки через ЮСАРТ
	while(data > 0)
	{
		c = 0;
		c += data%10; // Отделяем крайнюю правую цифру от переменной data и записываем ее в переменную с (не выводит 0. Сделать так, чтобы выводило)
		data = data/10; // Стираем крайнюю правую цифру из переменной дата
		switch(c) // Рассматриваем значение с
		{
			case 0: // Если в с записан 0 
				while (!(UCSR0A & (1<<UDRE0))); // Проверяем буфер перед отправкой
				UDR0 = '0'; //Записываем значение с в буфер для отправки
				break;
			case 1:
				while ( !( UCSR0A & (1<<UDRE0)) ); // Проверяем буфер перед отправкой
				UDR0 = '1'; //Записываем значение с в буфер для отправки
				break;
			case 2:
				while ( !( UCSR0A & (1<<UDRE0)) ); // Проверяем буфер перед отправкой
				UDR0 = '2'; //Записываем значение с в буфер для отправки
				break;
			case 3:
				while ( !( UCSR0A & (1<<UDRE0)) ); // Проверяем буфер перед отправкой
				UDR0 = '3'; //Записываем значение с в буфер для отправки
				break;
			case 4:
				while ( !( UCSR0A & (1<<UDRE0)) ); // Проверяем буфер перед отправкой
				UDR0 = '4';
				break;
			case 5:
				while ( !( UCSR0A & (1<<UDRE0)) ); // Проверяем буфер перед отправкой
				UDR0 = '5'; //Записываем значение с в буфер для отправки
				break;
			case 6:
				while ( !( UCSR0A & (1<<UDRE0)) ); // Проверяем буфер перед отправкой
				UDR0 = '6'; //Записываем значение с в буфер для отправки
				break;
			case 7:
				while ( !( UCSR0A & (1<<UDRE0)) ); // Проверяем буфер перед отправкой
				UDR0 = '7'; //Записываем значение с в буфер для отправки
				break;
			case 8:
				while ( !( UCSR0A & (1<<UDRE0)) ); // Проверяем буфер перед отправкой
				UDR0 = '8'; //Записываем значение с в буфер для отправки
				break;
			case 9:
				while ( !( UCSR0A & (1<<UDRE0)) ); // Проверяем буфер перед отправкой
				UDR0 = '9'; //Записываем значение с в буфер для отправки
				break;
		}
	}
}

uint8_t overwriting(uint8_t data) // Функция, которая переписывает значение температуры 1234 -> 4321 
{
	uint8_t newData = 0;
	while(data > 0)
	{
		newData = newData*10 + (data % 10);
		data = data/10;
	}
	return newData;
}


uint8_t uart_read(void)
{
	//Data_to_share = rx_buffer[0]*10 + rx_buffer[1]; // записываем значение полученной с компьютера температуры в целочисленную переменную для отправки в блок commands

	return Data_to_share;
}


