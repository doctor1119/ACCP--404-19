#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#define F_CPU 16000000UL
#define F_SCL 100000UL
#define ADDR_W 0b10010001
#define ADDR_R 0b10010000
int16_t current_temp = 0;

void temp_sensor_init(void)
void TWI_WAIT(void)
void TWI_START(void)
void TWI_SEND(unsigned char)
void TWI_STOP(void)
uint8_t TWI_READ_ACK(void)
uint8_t TWI_READ_NACK(void)
void Write_to_USART(float)
void temp_sensor_read(void)

#endif /* TEMP_SENSOR_H */