/*
 * BikeComputer.c
 *
 * Created: 2013-06-02 23:38:45
 * Author: darkenk
 *
 * LCD Connection:
 *   VSS, VEE, R/W, LED-:   GND
 *   VCC, LED+ (R 330 ohm): VCC
 *   RS:                    PB2
 *   E:                     PB3
 *   DB4-7:                 PB4-7
 */

#include <avr/io.h>
#include <avr/sleep.h>
#include "HD44780.h"
#include "Clock.h"

void minute_handler();
void second_handler();

#define MSG_SECOND_CHANGE 0x1
#define MSG_MINUTE_CHANGE 0x2
#define SET_MESSAGE(msg) g_msg |= msg
#define CLEAR_MESSAGE(msg) g_msg &= ~msg

volatile char g_msg = 0x1;

#define HEART_BEAT

int main(void)
{
    LCD_Initalize();
    LCD_GoTo(0, 0);                     // Set cursor to first char in first line
    LCD_WriteText("-= LCD READY =-");   // Display sample text

#ifdef HEART_BEAT
    DDRB  |= (1<<PB0);
    PORTB |= (1<<PB0);
#endif

    g_clock.handle_minute = minute_handler;
    g_clock.handle_second = second_handler;
    init_clock();

    char clock_value[9];
    clock_value[8] = 0;

    LCD_GoTo(0,1);

    while(1) {
        while(g_msg != 0) {
            if (g_msg & MSG_SECOND_CHANGE) {
                LCD_GoTo(0,1);
                get_time_str(clock_value);
                LCD_WriteText(clock_value);
                CLEAR_MESSAGE(MSG_SECOND_CHANGE);
            } else if (g_msg & MSG_MINUTE_CHANGE) {
                CLEAR_MESSAGE(MSG_MINUTE_CHANGE);
            }
        }
        sleep_mode();
    }
    return 0;
}

void minute_handler()
{
    SET_MESSAGE(MSG_MINUTE_CHANGE);
}

void second_handler()
{
#ifdef HEART_BEAT
    PORTB ^= (1<<PB0);
#endif
    SET_MESSAGE(MSG_SECOND_CHANGE);
}
