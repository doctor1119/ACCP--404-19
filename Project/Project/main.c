#include <avr/io.h> //для работы с регистрами ввода/вывода
#include <util/delay.h> //для задержек
#include <avr/interrupt.h> //для работы с прерываниями
#include <stdio.h>
#include <util/twi.h>
#include "temp_sensor.h"
#include "uart.h"
#include "commands.h"

#define F_CPU 16000000UL




int main(void)
{
	sei(); // Разрешаем глобальные прерывания
	USART_Init(UBRR_VALUE); //инициализация UART
	temp_sensor_init();//инициализация датчика температуры
	uint8_t last_temperature = 0;
	setupPWM();
	command(18); //Для отладки модуля commands
	while (1)
	{
			
	}
}





