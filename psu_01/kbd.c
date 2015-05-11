/* vim: set sw=8 ts=8 si : */
/*********************************************
* Author: Guido Socher, Copyright: GPL 
* 
* read the keyboard
**********************************************/
#include <avr/io.h>
#define F_CPU 8000000UL  // 4 MHz
#include <util/delay.h>


// it looks like output port settings need time to propagate. Maybe
// caused by input capacitors on the lcd which connect to the same ports.
static void kbd_wait(void){
	_delay_ms(7.5);
}

// kbd connection:
// U+=PC3, U-=PC4, I+,store=PB2, I-=PC5
// common input wires: PB5, PB3 for store
// PC3, PC4, PB2, PC5 are already initialized as output lines
// by the LCD driver code
void init_kbd(void) 
{
	// init lcd sets those already so we comment them out here:
	/*
	DDRC|=(1<<DDC3); // output line
	DDRC|=(1<<DDC4); // output line
	DDRB|=(1<<DDB2); // output line
	DDRC|=(1<<DDC5); // output line
	*/

	DDRB&= ~(1<<DDB5); // input line
	DDRB&= ~(1<<DDB3);

	PORTB|= (1<<PINB5); // internal pullup resistor on
	PORTB|= (1<<PINB3); // internal pullup resistor on
}

uint8_t check_u_button(int16_t *u) 
{
	// check U+ button:
	PORTC&=~(1<<PORTC3); // to gnd
	PORTC|=(1<<PORTC4); // to vcc
	PORTB|=(1<<PORTB2); // to vcc
	PORTC|=(1<<PORTC5); // to vcc
	kbd_wait();
	if (bit_is_clear(PINB,PINB5)){
		(*u)++;
		return(1);
	}
	// check U- button:
	PORTC|=(1<<PORTC3); // to vcc
	PORTC&=~(1<<PORTC4); // to gnd
	PORTB|=(1<<PORTB2); // to vcc
	PORTC|=(1<<PORTC5); // to vcc
	kbd_wait();
	if (bit_is_clear(PINB,PINB5) && (*u)>0){
		(*u)--;
		if((*u)<0){
			(*u)=0;
		}
		return(1);
	}
	return(0);
}

uint8_t check_i_button(int16_t *i) 
{
	// check I+ button:
	PORTC|=(1<<PORTC3); // to vcc
	PORTC|=(1<<PORTC4); // to vcc
	PORTB&=~(1<<PORTB2); // to gnd
	PORTC|=(1<<PORTC5); // to vcc
	kbd_wait();
	if (bit_is_clear(PINB,PINB5)){
		(*i)++;
		return(1);
	}
	// check I- button:
	PORTC|=(1<<PORTC3); // to vcc
	PORTC|=(1<<PORTC4); // to vcc
	PORTB|=(1<<PORTB2); // to vcc
	PORTC&=~(1<<PORTC5); // to gnd
	kbd_wait();
	if (bit_is_clear(PINB,PINB5) && (*i)>0){
		(*i)--;
		if((*i)<0){
			(*i)=0;
		}
		return(1);
	}
	return(0);
}

uint8_t check_store_button(void) 
{
	// check store button:
	PORTB&=~(1<<PORTB2); // to gnd
	kbd_wait();
	if (bit_is_clear(PINB,PINB3)){
		return(1);
	}
	return(0);
}
