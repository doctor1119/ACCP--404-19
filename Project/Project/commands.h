#ifndef COMMANDS_H
#define COMMANDS_H

#include <avr/io.h>

#define MAXLEVEL 52
#define PWM_PIN PD5

extern uint16_t data18[];
extern uint16_t data19[];
extern uint16_t data20[];
extern uint16_t data21[];
extern uint16_t data22[];
extern uint16_t data23[];
extern uint16_t data24[];
extern uint16_t data25[];
extern uint16_t data26[];
extern uint16_t data27[];
extern uint16_t data28[];
extern uint16_t data29[];
extern uint16_t data30[];

extern volatile uint16_t interrupt_count;

void setupPWM(void);
void loop(int16_t data[], uint8_t array_size);
void startPWM(void);
void pausePWM(void);
void dropPWM(void);
void command(uint8_t temp);

#endif // COMMANDS_H