#include <stdlib.h>
#include "speedometer.h"

typedef struct _speedometer {
    // speed in 0.1km/h
    uint16_t speed;
    // cannot be cleared
    uint32_t total_distance;
    // distance for measure speed
    uint32_t current_distance;
    // in mm
    uint32_t circumference;
    // last timestamp
    uint32_t last_tick;
    // previous last timestamp
    uint32_t prev_last_tick;
    // how long speed should be shown
    uint8_t show_time;
} speedometer;

speedometer g_speedometer = {
    .speed = 0,
    .total_distance = 0,
    .current_distance = 0,
    .circumference = 2146,
    .last_tick = 0,
    .prev_last_tick = 0,
    .show_time = 0
};

void calculate_speed()
{
    uint32_t diff = g_speedometer.last_tick - g_speedometer.prev_last_tick;
    if (diff) {
        g_speedometer.speed = 36 * g_speedometer.current_distance / (diff);//multiply by 36 to convert into km/h
        g_speedometer.prev_last_tick = g_speedometer.last_tick;
        g_speedometer.current_distance = 0;
    } else {
        if (!(g_speedometer.show_time++%3)) {
            g_speedometer.speed = 0;
            g_speedometer.show_time = 0;
        }
    }
}

void set_circumference(uint32_t circumference)
{
    g_speedometer.circumference = circumference;
}

void set_tick(uint32_t tick)
{
    g_speedometer.last_tick = tick;
    g_speedometer.total_distance += g_speedometer.circumference;
    g_speedometer.current_distance += g_speedometer.circumference;
}

void get_current_speed(char *buffer)
{
    uint32_t speed = g_speedometer.speed/10;
    utoa(speed, buffer, 10);
    int i = 0;
    int j = 1;
    do {
        i++;
        j *= 10;
    } while(speed/j);
    buffer[i++] = '.';
    utoa(g_speedometer.speed%10, buffer+i, 10);
    buffer[++i] = '\0';
}
