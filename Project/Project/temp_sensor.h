#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <avr/io.h>

#define F_CPU 16000000UL
#define F_SCL 100000UL
#define LM75A_ADDR 0x48

extern float current_temp;

void temp_sensor_init(void);
void TWI_WAIT(void);
void TWI_START(void);
void TWI_SEND(uint8_t Data);
void TWI_STOP(void);
uint8_t TWI_READ(uint8_t ack);
void send_to_USART(float celsius_temperature);
void temp_sensor_read(void);

#endif //TEMP_SENSOR_H
