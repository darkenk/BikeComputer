/*
 * Clock.h
 *
 * Created: 2013-06-03 20:47:18
 *  Author: darkenk
 */

#ifndef CLOCK_H_
#define CLOCK_H_

struct _clock {
    uint32_t current_time;
    uint8_t current_hour;
    uint8_t current_minute;
    uint8_t current_second;
    void (*handle_second)();
    void (*handle_minute)();
};

typedef struct _clock clock;

extern clock g_clock;

extern void init_clock();

extern void set_clock(uint8_t _hour, uint8_t _minute);

/**
 * Returns clock value in string. String format: HH:MM:SS
 * @brief get_time_str
 * @param out_buf
 */
extern void get_time_str(char* out_buf);

#endif /* CLOCK_H_ */