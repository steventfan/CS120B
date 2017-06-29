/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 2 Exercise 5
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 

#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b)
{
	return (b ? x | (0x01 << k) : x & ~(0x01 << k) );
}

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ( (x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRB = 0xFE; PORTB = 0x01;
	DDRD = 0x00; PORTD = 0xFF;
	unsigned char tmpB = 0x00;
	unsigned char tmpD = 0x00;
	
    while(1)
    {
		tmpB = PINB;
		tmpD = PIND;
		
		if ( (GetBit(tmpD, 0) == 1 && GetBit(tmpD, 1) == 1 && GetBit(tmpD, 5) == 1) || (GetBit(tmpD, 2) == 1 && GetBit(tmpD, 5) == 1) || (GetBit(tmpD, 3) == 1 && GetBit(tmpD, 5) == 1) || (GetBit(tmpD, 4) == 1 && GetBit(tmpD, 5) == 1) || GetBit(tmpD, 6) == 1 || GetBit(tmpD, 7) == 1)
		{
			tmpB = SetBit(tmpB, 1, 1);
			tmpB = SetBit(tmpB, 2, 0);
		}
		else if ( (GetBit(tmpD, 0) == 1 && GetBit(tmpD, 1) == 1) || GetBit(tmpD, 2) == 1 || GetBit(tmpD, 3) == 1 || GetBit(tmpD, 4) == 1 || GetBit(tmpD, 5) == 1)
		{
			tmpB = SetBit(tmpB, 1, 0);
			tmpB = SetBit(tmpB, 2, 1);
		}
		else
		{
			tmpB = SetBit(tmpB, 1, 0);
			tmpB = SetBit(tmpB, 2, 0);
		}
         
		PORTB = tmpB;
    }
	
	return 0;
}