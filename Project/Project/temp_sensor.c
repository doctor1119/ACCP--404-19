#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h> //для задержек
#include <util/twi.h>
#include "temp_sensor.h"

#define F_SCL 100000UL
#define LM75A_ADDR 0x48
#define ADDR_W 0b1001000 //0x90  // Адрес датчика температуры для записи
#define ADDR_R 0b1001000 // Адрес датчика температуры для считывания

float current_temp = 0; // Значение температуры

void temp_sensor_init(void) // Функция инициализации датчика температуры и установки таймера
{
	// Инициализация датчика температуры
	TWSR = 0x00;
	TWBR = ((F_CPU / F_SCL) - 16) / 2;
	TWCR = (1 << TWEN);

	// Установка прерывания по таймеру раз в секунду
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
	OCR1A = 15624;
	TIMSK1 |= (1 << OCIE1A);
 
}

void TWI_WAIT(void) // Ожидание окончания предыдущей операции
{
	while(!(TWCR & (1<<TWINT)));
}

void TWI_START(void) // Начало общение с датчиком
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

void TWI_SEND(uint8_t Data) // Отправка информации на датчик 
{
	TWDR = Data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

void TWI_STOP(void) // Окончание работы с датчиком
{
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); 
}

uint8_t TWI_READ(uint8_t ack) // Считываем часть данных, отправленных датчиком, и ждем вторую часть данных
{
	
	TWCR = (1 << TWINT) | (ack << TWEA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}



ISR(TIMER1_COMPA_vect) // Прерывание по таймеру
{
	uint8_t start_stop = start_stop_allow(); // Считываем значение start_stop
	//if(start_stop == 1) // Если значение start_stop рано единице, то разрешаем работу с датчиком, если - 0, то запрещаем работу с датчиком
	//{
		temp_sensor_read(); // Функция общения с датчиком температуры 
	//}
}

void Write_to_USART(float celsius_temperature) // Функция для перевода в целочисленное значение и отправки температуры, полученной с датчика, через ЮСАРТ на компьютер 
{
	
	celsius_temperature = celsius_temperature/1;
	uint8_t temp = 0;
	temp = (uint8_t)celsius_temperature;
	send(temp);
}

	void temp_sensor_read(void) // Функция общения с датчиком температуры
	{
		
		
		 int16_t tempr = 0;
		 
		 //int temperature;

		 /*TWI_START();
		 TWI_SEND(LM75A_ADDR<< 1);
		 TWI_SEND(0x00);
		 TWI_STOP();

		 _delay_ms(100); // Даем датчику время на измерение температуры
*/
		 TWI_START();
		 TWI_SEND((LM75A_ADDR << 1) | 0x01);
		 uint8_t msb = TWI_READ(1);
		 uint8_t lsb = TWI_READ(0);
		 TWI_STOP();

		
		tempr = ((msb << 8) | lsb) >> 5;
		float celsius_temperature = (uint8_t)tempr* 0.125;
		//celsius_temperature = (int16_t)(celsius_temperature << 7) >> 7;

		Write_to_USART(celsius_temperature);
	}
	
	
	
	