/* vim: set sw=8 ts=8 si : */
/*********************************************
* Author: Guido Socher, Copyright: GPL 
* 
* Digital to analog converter using a R-2R leadder (7bit)
* and PWM (5bit)
**********************************************/
#include <avr/io.h>

// this dac can do 12 bit resolution: bit 0-4=pwm, bit 5-11=R-2R leadder
void dac(uint16_t value){
        //OCR1AH=0;
        OCR1AL=value&0x1F; // lower 5 bits
	value=value>>(5-2);
	// r2r ladder is pd2 to pd7 and pb0
	PORTD=(PORTD&0x3)|(value&0xfc);
	value=value>>8;
	if (value){ // the MSB in the dac
		PORTB|= (1<<PINB0);
	}else{
		PORTB &= ~(1<<PINB0);
	}
}

void init_dac(void) 
{
	// enable PD2 PD3 PD4 PD5 PD6 PD7 PB0 as output (PD2=LSB of R2R-leadder)
	DDRD|= 0xfc; // output
	PORTD &= PORTD&0x3; //  zero volt on PD2..PD7
	DDRB|= (1<<DDB0);
	PORTB &= ~(1<<PINB0);
	//
	DDRB|= (1<<DDB1); // PB1 output
	// set up of Pulse Width Modulation (PWM)
	TCNT1H=0; // counter to zero, high byte first
	TCNT1L=0;
        // COM1A1  COM1A0
        //  1       0     Clear OC1A/OC1B on Compare Match (Set output to low level)
        //  1       1     Set OC1A/OC1B on Compare Match (Set output to high level)
        //
        // Fast PWM, ICR1 is top
        // See datasheet page 99 (settings) and 88 (description).
        TCCR1A=(0<<COM1A0)|(1<<COM1A1)|(0<<WGM10)|(1<<WGM11);
        TCCR1B=(1<<CS10)|(1<<WGM12)|(1<<WGM13); // full clock speed
	// 5 bit resolution:
	ICR1H=0;
	ICR1L=0x1F;
        // At what value to switch on the port (port OC1A=0 -> 0 Volt output)
        OCR1AH=0;
        OCR1AL=0;
}


