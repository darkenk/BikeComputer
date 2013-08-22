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

void init_keypad()
{
    CLEAR_BIT(DDRD, DDD2);     // Clear the PD2 pin
    // PD2 (INT0 pin) is now an input
    SET_BIT(PORTD, PORTD2);    // turn On the Pull-up
    SET_BIT(GICR, INT0);
    SET_BIT(MCUCR, ISC01);
    SET_BIT(MCUCR, ISC00);

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
    if (IS_BIT_SET(PIND, DDD2)) {
        append_msg(MSG_INPUT, 0, 0);
    }
    SET_BIT(GICR, INT0);
}
