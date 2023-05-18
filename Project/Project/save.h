#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#define F_CPU 16000000UL
#define F_SCL 100000UL
#define BAUD_RATE 9600
#define UBRR_VALUE F_CPU/BAUD_RATE/16 - 1



void temp_sensor_init(void);
void TWI_WAIT(void);
void TWI_START(void);
void TWI_SEND(unsigned char);
void TWI_STOP(void);
uint8_t TWI_READ_ACK(void);
uint8_t TWI_READ_NACK(void);
void Write_to_USART(float);
void temp_sensor_read(void);

void USART_Init(unsigned int);
uint8_t uart_read(void);
uint8_t overwriting(uint8_t);
void send(uint8_t);
uint8_t start_stop_allow(void);

void command(uint8_t);



#endif /* TEMP_SENSOR_H */