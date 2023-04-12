﻿#include <xc.h>
#include <avr/delay.h>
#include "save.h" // Созданная нами библиотека
#include <avr/interrupt.h>

volatile uint8_t rx_buffer[2]; //Массив для записи температуры, полученной с компьютера
volatile uint8_t rx_buffer_index = 0; //Индекс массива для записи температуры, полученной с компьютера

//Оставить до испытания
volatile uint8_t tx_buffer[3];
volatile uint8_t tx_buffer_index = 0;
//Оставить до испытания

uint8_t Data_to_share = 0; //Температура, отправляемая в блок commands
uint8_t start_stop = 0; //Переменная для включения-выключения работы программы датчика температуры с компьютера (0 - выключена, 1 - включена)

ISR(USART_RX_vect)
{	
	uint8_t data = UDR0 - '0'; // Записываем данные полученные с компьютера и переводим их в численное значение
	int Data_s_t = UDR0;
	if (Data_s_t == 's') // Если с компьютера приходит символ s увеличиваем/уменьшаем значение start_stop (включаем/выключаем программу работы с датчиком)
	{
		start_stop++;
		if (start_stop > 1)
		{
			start_stop = 0;
		}
		
	}
	else // Если с компьютера приходит любое другое значение, расцениваем его, как температуру
	{
		rx_buffer[rx_buffer_index] = data;
		rx_buffer_index++;
		if (rx_buffer_index == 2)
		{
			rx_buffer_index = 0;
		}
	}
	
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
	
	if(data < 0) //Если значение температуры отрицательное, добавляем перед ним минус
	{
		while (!(UCSR0A & (1<<UDRE0))); // Проверяем буфер перед отправкой
		UDR0 = '-'; //Записываем минус в буфер для отправки
	}
	data = fabs(data); // Берем модуль от значения температуры
	data = overwriting(data); //Переписываем значение температуры 1234 -> 4321. Так надо.
	uint8_t c; // В c будем записывать по очереди цифры из data для отправки через ЮСАРТ
	while(data > 0)
	{
		c = data%10;
		data = data/10;
		switch(c) //Выбираем
		{
			case 0:
				while (!(UCSR0A & (1<<UDRE0))); // Проверяем буфер перед отправкой
				UDR0 = '0'; //Записываем значение с в буфер для отправки
				break;
			case 1:
			case -1:
				while ( !( UCSR0A & (1<<UDRE0)) ); // Проверяем буфер перед отправкой
				UDR0 = '1'; //Записываем значение с в буфер для отправки
				break;
			case 2:
			case -2:
				while ( !( UCSR0A & (1<<UDRE0)) ); // Проверяем буфер перед отправкой
				UDR0 = '2'; //Записываем значение с в буфер для отправки
				break;
			case 3:
			case -3:
				while ( !( UCSR0A & (1<<UDRE0)) ); // Проверяем буфер перед отправкой
				UDR0 = '3'; //Записываем значение с в буфер для отправки
				break;
			case 4:
			case -4:
				while ( !( UCSR0A & (1<<UDRE0)) ); // Проверяем буфер перед отправкой
				UDR0 = '4';
				break;
			case 5:
			case -5:
				while ( !( UCSR0A & (1<<UDRE0)) ); // Проверяем буфер перед отправкой
				UDR0 = '5'; //Записываем значение с в буфер для отправки
				break;
			case 6:
			case -6:
				while ( !( UCSR0A & (1<<UDRE0)) ); // Проверяем буфер перед отправкой
				UDR0 = '6'; //Записываем значение с в буфер для отправки
				break;
			case 7:
			case -7:
				while ( !( UCSR0A & (1<<UDRE0)) ); // Проверяем буфер перед отправкой
				UDR0 = '7'; //Записываем значение с в буфер для отправки
				break;
			case 8:
			case -8:
				while ( !( UCSR0A & (1<<UDRE0)) ); // Проверяем буфер перед отправкой
				UDR0 = '8'; //Записываем значение с в буфер для отправки
				break;
			case 9:
			case -9:
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
	Data_to_share = rx_buffer[0]*10 + rx_buffer[1]; // записываем значение полученной с компьютера температуры в целочисленную переменную для отправки в блок commands
	return Data_to_share;
}


