/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 1 Exercise 4
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 


#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0x00; PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00; 
	unsigned char tmpA = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;
	unsigned char tmpD = 0x00;
	unsigned char tmpD1 = 0x00;
	unsigned char tempVal = 0x00;
	
	while(1)
	{
		tmpA = PINA;
		tmpB = PINB;
		tmpC = PINC;
		tmpD = 0;
		
		if ( (tmpA + tmpB + tmpC) > 140)
		{
			tmpD = tmpD | 0x01;
		}
		if ( ( (tmpA - tmpC) > 80) || ( (tmpA - tmpC) < -80 ) )
		{
			tmpD = tmpD | 0x02;
		}
		
		tempVal = tmpA + tmpB + tmpC;
		tempVal = tempVal / 3; //average weight per seat
		tempVal = tempVal * 4; //shift value to bits[7:2]
		tmpD1 = (tmpD1 & 0x00) | tempVal;
		tmpD1 = tmpD1 | tmpD;
		
		PORTD = tmpD1;
	}

	return 0;
}