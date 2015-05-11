/*********************************************
* vim: set sw=8 ts=8 si :
* Author: Guido Socher, Copyright: GPL
* This is a test program which will write "LCD works"
* on the LCD display.
* This program is also used to test the keypad. It
* displays the button last pressed.
*
* See http://www.tuxgraphics.org/electronics/
*
* Chip type           : ATMEGA8
* Clock frequency     : Internal clock 8 Mhz
*********************************************/
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#define F_CPU 8000000UL  // 8 MHz
#include <util/delay.h>
#include "dac.h"
#include "lcd.h"
#include "kbd.h"
#include <stdlib.h>
#include <string.h>

//debug LED:
// set output to VCC, red LED off
#define LEDOFF PORTD|=(1<<PORTD0)
// set output to GND, red LED on
#define LEDON PORTD&=~(1<<PORTD0)
// to test the state of the LED
#define LEDISOFF PORTD&(1<<PORTD0)

void delay_ms(uint16_t ms)
/* delay for a minimum of <ms> */
{
	// we use a calibrated macro. This is more
	// accurate and not so much compiler dependent
	// as self made code.
	while(ms){
		_delay_ms(0.96);
		ms--;
	}
}


int main(void)
{
	int16_t dac_val=511;
	int16_t cnt;
	int8_t dac_dir=0;
	char out_buf[21];
	DDRD|= (1<<DDD0); // LED, enable PD0, LED as output
	LEDOFF;
	init_dac();
	lcd_init();
	lcd_clrscr();
	lcd_puts("use U+/U-");
	lcd_gotoxy(0,1);
	lcd_puts("and store");
	init_kbd();
	delay_ms(500);
	delay_ms(500);
	lcd_clrscr();
	lcd_puts_p(PSTR("pause"));
	while (1) {
		if (dac_dir==1){
			dac_val++;
		}
		if (dac_dir==-1){
			dac_val--;
		}
		if (dac_val>0xFFF){
			dac_val=0xFFF; //max, 13bit
		}
		if (dac_val<0){
			dac_val=0;
		}
		lcd_gotoxy(0,1);
		itoa(dac_val,out_buf,10);
		lcd_puts(out_buf);
		lcd_puts("    ");
		dac(dac_val);
		cnt=1;
		check_u_button(&cnt);
		if (cnt>1){
			lcd_clrscr();
			// u+ pressed
			lcd_puts_p(PSTR("up"));
			dac_dir=1;
			LEDOFF;
		}
		if (cnt<1){
			lcd_clrscr();
			// u- pressed
			lcd_puts_p(PSTR("down"));
			dac_dir=-1;
			LEDON;
		}
		if (check_store_button()){
			lcd_clrscr();
			lcd_puts_p(PSTR("pause"));
			dac_dir=0;
		}
		delay_ms(100);
		cnt=1;
		check_u_button(&cnt);
		if (cnt>1){
			lcd_clrscr();
			// u+ pressed
			lcd_puts_p(PSTR("up"));
			dac_dir=1;
			LEDOFF;
		}
		if (cnt<1){
			lcd_clrscr();
			// u- pressed
			lcd_puts_p(PSTR("down"));
			dac_dir=-1;
			LEDON;
		}
		if (check_store_button()){
			lcd_clrscr();
			lcd_puts_p(PSTR("pause"));
			dac_dir=0;
		}
		delay_ms(100);
	}
	return(0);
}
