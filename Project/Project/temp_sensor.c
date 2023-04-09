#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "save.h"

//#define LM75A_ADDR 0x48
#define ADDR_W 0b10010001
#define ADDR_R 0b10010000

int16_t current_temp = 0;

void temp_sensor_init(void) {
	TWSR = 0;
	TWBR = ((F_CPU / F_SCL) - 16) / 2;

	
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
	OCR1A = 15624;
	TIMSK1 |= (1 << OCIE1A);
}

void TWI_WAIT(void)
{
	while(!(TWCR & (1<<TWINT)));
}

void TWI_START(void)
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
}

void TWI_SEND(unsigned char Data)
{
	TWDR = Data;
	TWCR = (1<<TWINT) | (1<<TWEN);
}

void TWI_STOP(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

uint8_t TWI_READ_ACK(void)
{
	uint8_t data;
	TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
	TWI_WAIT();
	data = TWDR;
	return data;
}

uint8_t TWI_READ_NACK(void)
{
	uint8_t data;
	TWCR &= ~(1<<TWEA);
	TWCR =(1<<TWINT) | (1<<TWEN);
	TWI_WAIT();
	data = TWDR;
	return data;
	
}

ISR(TIMER1_COMPA_vect) {
	temp_sensor_read();
}

void Write_to_USART(float celsius_temperature)
{
	celsius_temperature = celsius_temperature/1;
	uint8_t temp = (uint8_t)celsius_temperature;
	send(temp);
	
}

	void temp_sensor_read(void) {
		
		TWI_START();
		TWI_WAIT();
		TWI_SEND(ADDR_W);
		TWI_WAIT();
		TWI_SEND(0x00);
		TWI_WAIT();

		TWI_START();
		TWI_WAIT();
		TWI_SEND(ADDR_R);
		TWI_WAIT();
		
		TWI_WAIT();
		uint8_t msb = TWI_READ_ACK();
		TWI_WAIT();
		uint8_t lsb = TWI_READ_NACK();
		TWI_WAIT();
		TWI_STOP();

		int16_t temperature = (msb << 8) | lsb;
		temperature = (int16_t)(temperature << 7) >> 7;
		float celsius_temperature = (float)temperature * 0.5;
		
		current_temp = celsius_temperature;
		
		Write_to_USART(celsius_temperature);
		
	}
