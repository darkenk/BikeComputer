#include <avr/interrupt.h>
#include "keypad.h"
#include "utils.h"
#include "msg_queue.h"

#ifndef F_CPU
# error "Please define F_CPU"
#endif

#if F_CPU > 1000000UL
# warning "Not tested with such high F_CPU"
#endif

#define DEBOUNCE_TIME 232;

#define KEY_MASK 0x07

void init_keypad()
{
    CLEAR_BIT(DDRD, DDD2);     // Clear the PD2 pin
    // PD2 (INT0 pin) is now an input
    SET_BIT(PORTD, PORTD2);    // turn On the Pull-up
    SET_BIT(GICR, INT0);
    // INT0 is triggered by falling edge
    CLEAR_BIT(MCUCR, ISC01);
    SET_BIT(MCUCR, ISC00);

    // disable jtag interface
    // yes, it is needed to be executed twice
    SET_BIT(MCUCSR, JTD);
    SET_BIT(MCUCSR, JTD);

    // set pins (PC0, PC1, PC2) as input
    SET_BITS(DDRC, KEY_MASK);
    // turn on the pull-up resistor
    SET_BITS(PORTC, KEY_MASK);

    // Set prescaler to 1024
    SET_BIT(TCCR2, CS22);
    SET_BIT(TCCR2, CS21);
    SET_BIT(TCCR2, CS20);;
}

ISR(INT0_vect)
{
    CLEAR_BIT(GICR, INT0);
    TCNT2 = DEBOUNCE_TIME;
    SET_BIT(TIMSK,TOIE2);
}

ISR(TIMER2_OVF_vect)
{
    CLEAR_BIT(TIMSK,TOIE2);
    if (!IS_BIT_SET(PIND, DDD2)) {
        int btn = (~PINC & KEY_MASK) >> 1;
        append_msg(MSG_INPUT, btn, 0);
    }
    SET_BIT(GICR, INT0);
}
