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
#define SCREEN_COUNT 2
#define DEFAULT_SCREEN 0
#define SETUP_TIME_SCREEN 1
#define SETUP_DIAMETER_SCREEN -1

struct default_screen {

} g_default_screen;

struct time_screen {
    int hour;
    int minute;
    char blink_pos;
    unsigned char blink;
} g_time_screen;

char string_buff[9];

void show_setup_time_screen(unsigned char screen_changed, msg_t * msg);
void show_default_screen(msg_t* msg);
void clear_screen();
void show_setup_diameter_screen();
void LCD_WriteTwoDigit(char posx, char posy, int number);

int main(void)
{
    LCD_Initalize();
    LCD_GoTo(0, 0);                     // Set cursor to first char in first line
    LCD_WriteText("-= LCD READY =-");   // Display sample text

#ifdef HEART_BEAT
    SET_BIT(DDRB, PB0);
    SET_BIT(PORTB, PB0);
#endif

//    g_clock.handle_milisec = milisec_handler;
    g_clock.handle_second = second_handler;
    init_clock();
    init_keypad();
    sei();

    msg_t* msg = 0;
    int screen = DEFAULT_SCREEN;
    char screen_changed = 1;

    while(1) {
        while((msg = get_msg())) {
            switch(msg->type) {
            case MSG_INPUT:
                if (msg->size == BTN_0) {
                    if (screen == SETUP_TIME_SCREEN) {
                        set_clock(g_time_screen.hour, g_time_screen.minute);
                    }
                    ++screen;
                    screen %= SCREEN_COUNT;
                    screen_changed = 1;
                }
                break;

            default:
                break;
            }
            if (screen_changed) {
                clear_screen();
            }
            switch (screen) {
            case 0:
                show_default_screen(msg);
                break;

            case 1:
                show_setup_time_screen(screen_changed, msg);
                break;

            case -1:
                show_setup_diameter_screen();
                break;

            default:
            break;
            }
            screen_changed = 0;
            free_msg(&msg);
        }
        sleep_mode();
    }
    return 0;
}

void milisec_handler()
{
//    append_msg(MSG_MILISEC_CHANGE, 0, 0);
}

void second_handler()
{
#ifdef HEART_BEAT
    PORTB ^= (1<<PB0);
#endif
    append_msg(MSG_SECOND_CHANGE, 0, 0);
}

void show_default_screen(msg_t *msg)
{
    LCD_GoTo(8,0);
    get_time_str(string_buff);
    LCD_WriteText(string_buff);
    LCD_GoTo(4, 0);
    LCD_WriteText("k/h \0");
    int offset = 1;
    switch(msg->type) {
    case MSG_SECOND_CHANGE:
        offset = calculate_speed() > 99 ? 0 : 1;
        break;
    }
    if (offset > 0) {
        LCD_GoTo(0,0);
        LCD_WriteText(" \0");
    }
    get_current_speed(string_buff);
    LCD_GoTo(offset, 0);
    LCD_WriteText(string_buff);
}

void show_setup_time_screen(unsigned char screen_changed, msg_t * msg)
{
    if (screen_changed) {
        g_time_screen.hour = g_clock.current_hour;
        g_time_screen.minute = g_clock.current_minute;
        LCD_GoTo(0,0);
        LCD_WriteText("Set time: ");
        LCD_GoTo(0,1);
        get_time_str(string_buff);
        string_buff[5] = 0;
        LCD_WriteText(string_buff);
        g_time_screen.blink = 0;
        g_time_screen.blink_pos = 4;
    }

    uint32_t inc = 0;
    switch(msg->type) {
    case MSG_SECOND_CHANGE:
        g_time_screen.blink = ~g_time_screen.blink;
        break;

    case MSG_INPUT:
        if (msg->size == BTN_1) {
            inc = 1;
        } else if (msg->size == BTN_2) {
            if (g_time_screen.blink_pos == 4) {
                LCD_WriteTwoDigit(g_time_screen.blink_pos - 1, 1, g_time_screen.minute);
            } else {
                LCD_WriteTwoDigit(g_time_screen.blink_pos - 1, 1, g_time_screen.hour);
            }
            g_time_screen.blink_pos -= 3;
            if (g_time_screen.blink_pos == -2) {
                g_time_screen.blink_pos = 4;
            }
        }
        break;

    default:
        break;
    }

    int time = 99;
    switch(g_time_screen.blink_pos) {
    case 4:
        if (inc) {
            g_time_screen.minute++;
            g_time_screen.minute %= 60;
        }
        time = g_time_screen.minute;
        break;

    case 1:
        if (inc) {
            g_time_screen.hour++;
            g_time_screen.hour %= 24;
        }
        time = g_time_screen.hour;
        break;
    }

    if (g_time_screen.blink && !inc) {
        LCD_GoTo(g_time_screen.blink_pos, 1);
        LCD_WriteText("_\0");
    } else {
        LCD_WriteTwoDigit(g_time_screen.blink_pos-1, 1, time);
    }
}

void LCD_WriteTwoDigit(char posx, char posy, int number)
{
    int offset = 0;
    if (number < 10) {
        string_buff[offset++] = '0';
    }
    itoa(number, &string_buff[offset], 10);
    LCD_GoTo(posx, posy);
    LCD_WriteText(string_buff);
}

void clear_screen()
{
    LCD_GoTo(0,0);
    LCD_WriteText("                \0");
    LCD_GoTo(0,1);
    LCD_WriteText("                \0");
}

void show_setup_diameter_screen()
{

}
