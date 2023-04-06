//#include "temp_sensor.h"
//#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/twi.h>
#include <stdio.h>

#define TW_WAIT while(!(TWCR & (1<<TWINT)));
#define F_CPU 16000000UL
#define F_SCL 100000UL
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

ISR(TIMER1_COMPA_vect) {
	temp_sensor_read();
}

void temp_sensor_read(void) {
	TWI_START();
	TW_WAIT;
	TWI_SEND(ADDR_W);
	TW_WAIT;
	TWI_SEND(0x00);
	TW_WAIT;

	TWI_START();
	TW_WAIT;
	TWI_SEND(ADDR_R);
	TW_WAIT;
	TWCR |= (1<<TWEA);
	uint8_t msb = TWDR;//TW_READ_ACK;
	//TW_WAIT;
	//TWSR = 0x50; // ACK
	TW_WAIT;
	TWCR &= ~(1<<TWEA);
	uint8_t lsb = TWDR; //TW_READ_NACK;
	//TW_WAIT;
	//TWSR = 0x58;
	
	TW_WAIT;
	TWI_STOP();

	int16_t temperature = (msb << 8) | lsb;
	temperature >>= 7;
	current_temp = temperature;

	char temp_str[10];
	sprintf(temp_str, "TEMP:%d\n", temperature);
	for (uint8_t i = 0; temp_str[i] != '\0'; i++) {
		uart_send(temp_str[i]);
	}
}
