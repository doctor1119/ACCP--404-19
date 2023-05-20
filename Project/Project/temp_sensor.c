#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h> // for delays
#include <util/twi.h>
#include "temp_sensor.h"
#include "uart.h"

#define F_CPU 16000000UL
#define F_SCL 100000UL
#define LM75A_ADDR 0x48

float current_temp = 0;

// Init temperature sensor
void temp_sensor_init(void) {
	TWSR = 0x00;
	TWBR = ((F_CPU / F_SCL) - 16) / 2;
	TWCR = (1 << TWEN);
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
	OCR1A = 15624;
	TIMSK1 |= (1 << OCIE1A);
}

// Wait for TWI
void TWI_WAIT(void) { while(!(TWCR & (1<<TWINT))); }

// Start TWI
void TWI_START(void) {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	TWI_WAIT();
}

// Send TWI
void TWI_SEND(uint8_t Data) {
	TWDR = Data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	TWI_WAIT();
}

// Stop TWI
void TWI_STOP(void) { TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); }

// Read TWI
uint8_t TWI_READ(uint8_t ack) {
	TWCR = (1 << TWINT) | (ack << TWEA) | (1 << TWEN);
	TWI_WAIT();
	return TWDR;
}

// Timer ISR
ISR(TIMER1_COMPA_vect) {
	temp_sensor_read();
}

// send to USART
void send_to_USART(float celsius_temperature) {
	uint8_t temp = (uint8_t)celsius_temperature;
	send(temp);
}

// Read temperature sensor
void temp_sensor_read(void) {
	int16_t tempr = 0;
	TWI_START();
	TWI_SEND((LM75A_ADDR << 1) | 0x01);
	uint8_t msb = TWI_READ(1);
	uint8_t lsb = TWI_READ(0);
	TWI_STOP();
	tempr = ((msb << 8) | lsb) >> 5;
	float celsius_temperature = tempr * 0.125;
	send_to_USART(celsius_temperature);
}