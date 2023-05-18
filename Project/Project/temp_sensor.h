#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H
#define F_SCL 100000UL

#include <stdint.h>

// Прототипы функций
void temp_sensor_init(void);
void TWI_WAIT(void);
void TWI_START(void);
void TWI_SEND(uint8_t Data);
void TWI_STOP(void);
uint8_t TWI_READ(uint8_t ack);
void Write_to_USART(float celsius_temperature);
void temp_sensor_read(void);

// Глобальные переменные
extern float current_temp;

// Макросы
#define LM75A_ADDR 0x48
#define ADDR_W 0b1001000
#define ADDR_R 0b1001000

#endif // TEMP_SENSOR_H
