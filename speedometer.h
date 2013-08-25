#ifndef SPEEDOMETER_H
#define SPEEDOMETER_H

#include <avr/io.h>

void set_circumference(uint32_t circumference);

void calculate_speed();

void get_current_speed(char* buffer);

void set_tick(uint32_t tick);

#endif // SPEEDOMETER_H
