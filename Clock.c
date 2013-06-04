/*
 * Clock.c
 *
 * Created: 2013-06-03 20:44:19
 *  Author: darkenk
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Clock.h"

#ifndef F_CPU
# error "Please define F_CPU"
#endif

#if F_CPU > 16000000UL
# error "F_CPU freq is too high"
#endif

#define CLOCK_START_VALUE 156

// current time has precission 0.1s
// time is zeroed after 23:59
clock g_clock = {
    .current_time = 0,
    .current_hour = 0,
    .current_minute = 0,
    .current_second = 0,
    .handle_second = 0,
    .handle_minute = 0
};
// delta has precission 0.0001s
volatile uint16_t delta = 0;
volatile uint8_t ms = 0;

inline void init_clock()
{
    TIMSK |= (1 << TOIE0);
    TCCR0 |= (1 << CS02) | (1 << CS00);
    TCNT0 = CLOCK_START_VALUE;
    sei();
}

inline void set_clock(uint8_t _hour, uint8_t _minute)
{
    cli();
    g_clock.current_hour = _hour;
    g_clock.current_minute = _minute;
    g_clock.current_second = 0;
    g_clock.current_time = _hour * 36000 + _minute * 600;
    TCNT0 = CLOCK_START_VALUE;
    sei();
}

ISR(TIMER0_OVF_vect)
{
    TCNT0 = CLOCK_START_VALUE;
    delta += 102400 / (F_CPU/100000);
    while (delta > 10000) {
        g_clock.current_time++;
        delta -= 10000;
        ms++;
    }
    while (ms >= 10) {
        ms -= 10;
        g_clock.current_second++;
        if (g_clock.current_second == 60) {
            g_clock.current_second = 0;
            g_clock.current_minute++;
            if (g_clock.current_minute == 60) {
                g_clock.current_minute = 0;
                g_clock.current_hour++;
                if (g_clock.current_hour == 24) {
                    g_clock.current_hour = 0;
                    g_clock.current_time = 0;
                }
                if (g_clock.handle_minute) {
                    g_clock.handle_minute();
                }
            }
        }
        if (g_clock.handle_second) {
            g_clock.handle_second();
        }
    }
}