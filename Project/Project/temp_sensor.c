#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "save.h" // Созданная нами библиотека

//#define LM75A_ADDR 0x48
#define ADDR_W 0b10010001 // Адрес датчика температуры для записи
#define ADDR_R 0b10010000 // Адрес датчика температуры для считывания

float current_temp = 0; // Значение температуры

void temp_sensor_init(void) // Функция инициализации датчика температуры и установки таймера
{
	// Инициализация датчика температуры
	TWSR = 0;
	TWBR = ((F_CPU / F_SCL) - 16) / 2;

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
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
}

void TWI_SEND(unsigned char Data) // Отправка информации на датчик 
{
	TWDR = Data;
	TWCR = (1<<TWINT) | (1<<TWEN);
}

void TWI_STOP(void) // Окончание работы с датчиком
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

uint8_t TWI_READ_ACK(void) // Считываем часть данных, отправленных датчиком, и ждем вторую часть данных
{
	uint8_t data;
	TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
	TWI_WAIT();
	data = TWDR;
	return data;
}

uint8_t TWI_READ_NACK(void) // Считываем данные, отправленные датчиком, и заканчиваем общение
{
	uint8_t data;
	TWCR &= ~(1<<TWEA);
	TWCR =(1<<TWINT) | (1<<TWEN);
	TWI_WAIT();
	data = TWDR;
	return data;
	
}

ISR(TIMER1_COMPA_vect) // Прерывание по таймеру
{
	uint8_t start_stop = start_stop_allow(); // Считываем значение start_stop
	if(start_stop == 1) // Если значение start_stop рано единице, то разрешаем работу с датчиком, если - 0, то запрещаем работу с датчиком
	{
		temp_sensor_read(); // Функция общения с датчиком температуры 
	}
}

void Write_to_USART(float celsius_temperature) // Функция для перевода в целочисленное значение и отправки температуры, полученной с датчика, через ЮСАРТ на компьютер 
{
	celsius_temperature = celsius_temperature/1;
	uint8_t temp = (uint8_t)celsius_temperature;
	send(temp);
}

	void temp_sensor_read(void) // Функция общения с датчиком температуры
	{
		
		TWI_START();
		TWI_WAIT();
		TWI_SEND(ADDR_W);
		TWI_WAIT();
		TWI_SEND(0x00);
		//TWI_WAIT(); // stop here // пока оставить
		

		TWI_START();
		TWI_WAIT();
		TWI_SEND(ADDR_R);
		TWI_WAIT();
		
		TWI_WAIT();
		uint8_t msb = TWI_READ_ACK(); // Переменная для записи первого пакета данных с датчика
		TWI_WAIT();
		uint8_t lsb = TWI_READ_NACK(); // Переменная для записи второго пакета данных с датчика
		TWI_WAIT();
		TWI_STOP();

		// Объединяем два полученных пакета в одной переменной
		int16_t temperature = (msb << 8) | lsb;
		temperature = (int16_t)(temperature << 7) >> 7;
		float celsius_temperature = (float)temperature * 0.125;
		
		current_temp = celsius_temperature;	 // Записываем полученную с датчика температуру в глобальную переменную для возможной дальнейшей работы
		
		Write_to_USART(celsius_temperature); //Отправляем полученную температуру в ЮСАРТ
		
	}
