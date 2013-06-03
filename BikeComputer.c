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
#define F_CPU 1000000UL

#include <avr/io.h>
#include "HD44780.h"

/*
 *  Component's initialization
 */
void init(void) {
	LCD_Initalize();			// LCD Display	
}

int main(void)
{
	init();
	
	LCD_GoTo(0, 0);						// Set cursor to first char in first line
	LCD_WriteText("-= LCD READY =-");	// Display sample text
	unsigned i;
	
    while(1)
    {
		LCD_GoTo(0, 1);			// Set cursor to first char in second line
	    for(i = 0; i < 16; ++i)
		{
			LCD_WriteText(">");
			_delay_ms(500);
		}
		
		LCD_GoTo(0, 1);		
		for(i = 0; i < 16; ++i)
		{
		    LCD_WriteText(" ");
			_delay_ms(500);	
		}
    }
}