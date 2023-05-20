#ifndef COMMANDS_H
#define COMMANDS_H

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Прототипы функций
void command(uint8_t temp);
void setupPWM(void);
void loop(int16_t data[], uint8_t array_size);
void startPWM(void);
void pausePWM(void);
void dropPWM(void);
void choose(uint8_t temp);

// Глобальные переменные
#define F_CPU 16000000UL
#define MAXLEVEL 52
#define PWM_PIN PD5

#endif  // COMMANDS_H
