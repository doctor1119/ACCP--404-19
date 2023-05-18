#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

// Прототипы функций
void command(uint8_t temp);

// Глобальные переменные
#define PRESCALER 8 // Предделитель
#define TIME_MULTIPLIER 1.2 // Множитель времени (1.2 мс)

#endif // COMMANDS_H
