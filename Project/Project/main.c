#include <avr/io.h> //для работы с регистрами ввода/вывода
#include <util/delay.h> //для задержек
#include <avr/interrupt.h> //для работы с прерываниями
#include "save.h"




int main(void)
{
	SREG |=(1<<7);
	USART_Init(UBRR_VALUE); //инициализация UART
	temp_sensor_init();//инициализация датчика температуры
	
	DDRD |= (1<<1);// Установка PD1 как пин для выхода ИК диода	
	PORTD &= ~(1 << 1); // Убираем напряжение с PD1
	uint8_t last_temperature = 0;
	while (1)
	{
		
		uint8_t temp_to_command = uart_read(); // Записываем значение температуры, полученное с компьютера, в переменную temp_to_command
		if(last_temperature != temp_to_command) // Если температура, получаемая с компьютера, изменилась
		{
		command(temp_to_command); // Отправляем значение полученной с компьютера температуры в блок commands
		last_temperature = temp_to_command;
		}
		
	}
}





