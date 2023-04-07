#include <avr/io.h> //для работы с регистрами ввода/вывода
#include <stdio.h> //для работы со стандартными потоками ввода/вывода
#include <util/delay.h> //для задержек
#include <avr/interrupt.h> //для работы с прерываниями
#include <avr/eeprom.h> //для работы с EEPROM
#include "save.h"
#define IR_LED_PORT     PORTD
#define IR_LED_PIN      PD1


int main(void)
{
	//инициализация UART
	uart_init();
	//инициализация датчика температуры
	temp_sensor_init();
	// Установка PD1 как пин для выхода ИК диода
	DDRD |= _BV(IR_LED_PIN);
	
	// Очищаем порт IR_LED_PIN ик диода
	IR_LED_PORT &= ~(1 << IR_LED_PIN);


	while (1)
	{
		// Если входной буфер UART готов для чтения
		if(uart_is_rx_ready())
		{
			
			// Читаем данные из входного буфера UART
			uint8_t data[MAX_BUFFER_SIZE];
			uint8_t len = uart_read(data, MAX_BUFFER_SIZE);
			// Если успешно считали данные
			if (len > 0)
			{
				// Обрабатываем команды
				commands_process(data, len);
			}
		}
		// Запрашиваем значение температуры
		uint16_t temperature = temp_sensor_read();
		
		//задержка в 500 мс перед повторением цикла
		_delay_ms(500);
		// Возможно нужен ещё блок инициализации ик передатчика/ отправки данных с помощью него
	}
}





