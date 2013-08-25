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
#include <avr/interrupt.h>
#include "HD44780.h"
#include "Clock.h"
#include "msg_queue.h"
#include "utils.h"
#include "keypad.h"
#include "speedometer.h"

void milisec_handler();
void second_handler();

#define HEART_BEAT

int main(void)
{
    LCD_Initalize();
    LCD_GoTo(0, 0);                     // Set cursor to first char in first line
    LCD_WriteText("-= LCD READY =-");   // Display sample text

#ifdef HEART_BEAT
    SET_BIT(DDRB, PB0);
    SET_BIT(PORTB, PB0);
#endif

    g_clock.handle_milisec = milisec_handler;
    g_clock.handle_second = second_handler;
    init_clock();
    init_keypad();
    sei();

    char clock_value[9];
    clock_value[8] = 0;
    char speed_value[9];
    speed_value[8] = 0;

    msg_t* msg = 0;
    uint32_t time;

    while(1) {
        while((msg = get_msg())) {
            switch(msg->type) {
            case MSG_SECOND_CHANGE:
                LCD_GoTo(0,1);
                get_time_str(clock_value);
                LCD_WriteText(clock_value);

                LCD_GoTo(0, 0);
                LCD_WriteText("S:      \0");
                calculate_speed();
                get_current_speed(speed_value);
                LCD_GoTo(2, 0);
                LCD_WriteText(speed_value);
                break;

            case MSG_MILISEC_CHANGE:
                break;

            case MSG_INPUT:
                ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                    time = g_clock.current_time;
                }
                set_tick(time);
                break;

            default:
                break;
            }
            free_msg(&msg);
        }
        sleep_mode();
    }
    return 0;
}

void milisec_handler()
{
    append_msg(MSG_MILISEC_CHANGE, 0, 0);
}

void second_handler()
{
#ifdef HEART_BEAT
    PORTB ^= (1<<PB0);
#endif
    append_msg(MSG_SECOND_CHANGE, 0, 0);
}
