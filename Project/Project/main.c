#include <avr/io.h> //для работы с регистрами ввода/вывода
#include <util/delay.h> //для задержек
#include <avr/interrupt.h> //для работы с прерываниями
#include "temp_sensor.h"
#include "uart.h"
#include "commands.h"




int main(void)
{
	sei(); // Разрешаем глобальные прерывания
	USART_Init(UBRR_VALUE); //инициализация UART
	temp_sensor_init();//инициализация датчика температуры
	uint8_t last_temperature = 0;
	while (1)
	{
		command(12);
		/*_delay_ms(1000); // костыль-задержка, чтобы функция uart_read успела правильно сформировать данные
		uint8_t temp_to_command = uart_read(); // Записываем значение температуры, полученное с компьютера, в переменную temp_to_command
		if(last_temperature != temp_to_command) // Если температура, получаемая с компьютера, изменилась
		{
		command(temp_to_command); // Отправляем значение полученной с компьютера температуры в блок commands
		last_temperature = temp_to_command; // Записываем в last_temperature последнее отправленное значение температуры для следующей проверки
		}*/
		
	}
}





