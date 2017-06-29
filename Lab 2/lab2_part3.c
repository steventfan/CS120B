/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 2 Exercise 3
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
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tmpA = 0x00;
	unsigned char tmpC = 0x00;
	
    while(1)
    {
        tmpA = PINA;
		
		if (GetBit(tmpA, 0) == 1 || GetBit(tmpA, 1) == 1 || GetBit(tmpA, 2) == 1 || GetBit(tmpA, 3) == 1)
		{
			tmpC = SetBit(tmpC, 5, 1);
		}
		else
		{
			tmpC = SetBit(tmpC, 5, 0);
		}
		if ( (GetBit(tmpA, 0) == 1 && GetBit(tmpA, 1) == 1) || GetBit(tmpA, 2) == 1 || GetBit(tmpA, 3) == 1)
		{
			tmpC = SetBit(tmpC, 4, 1);
		}
		else
		{
			tmpC = SetBit(tmpC, 4, 0);
		}
		if ( (GetBit(tmpA, 0) == 1 && GetBit(tmpA, 2) == 1) || (GetBit(tmpA, 1) == 1 && GetBit(tmpA, 2) == 1) || GetBit(tmpA, 3) == 1)
		{
			tmpC = SetBit(tmpC, 3, 1);
		}
		else
		{
			tmpC = SetBit(tmpC, 3, 0);
		}
		if ( (GetBit(tmpA, 0) == 1 && GetBit(tmpA, 1) == 1 && GetBit(tmpA, 2) == 1) || GetBit(tmpA, 3) == 1)
		{
			tmpC = SetBit(tmpC, 2, 1);
		}
		else
		{
			tmpC = SetBit(tmpC, 2, 0);
		}
		if ( (GetBit(tmpA, 1) == 1 && GetBit(tmpA, 3) == 1) || (GetBit(tmpA, 2) == 1 && GetBit(tmpA, 3) == 1) )
		{
			tmpC = SetBit(tmpC, 1, 1);
		}
		else
		{
			tmpC = SetBit(tmpC, 1, 0);
		}
		if ( (GetBit(tmpA, 0) == 1 && GetBit(tmpA, 2) == 1 && GetBit(tmpA, 3) == 1) || (GetBit(tmpA, 1) == 1 && GetBit(tmpA, 2) == 1 && GetBit(tmpA, 3) == 1) )
		{
			tmpC = SetBit(tmpC, 0, 1);
		}
		else
		{
			tmpC = SetBit(tmpC, 0, 0);
		}
		if ( (GetBit(tmpA, 0) == 0 && GetBit(tmpA, 1) == 0 && GetBit(tmpA, 2) == 0 && GetBit(tmpA, 3) == 0) || (GetBit(tmpA, 0) == 1 && GetBit(tmpA, 1) == 0 && GetBit(tmpA, 2) == 0 && GetBit(tmpA, 3) == 0) || (GetBit(tmpA, 0) == 0 && GetBit(tmpA, 1) == 1 && GetBit(tmpA, 2) == 0 && GetBit(tmpA, 3) == 0) || (GetBit(tmpA, 0) == 1 && GetBit(tmpA, 1) == 1 && GetBit(tmpA, 2) == 0 && GetBit(tmpA, 3) == 0) || (GetBit(tmpA, 0) == 0 && GetBit(tmpA, 1) == 0 && GetBit(tmpA, 2) == 1 && GetBit(tmpA, 3) == 0) )
		{
			tmpC = SetBit(tmpC, 6, 1);
		}
		else
		{
			tmpC = SetBit(tmpC, 6, 0);
		}
		
		if (GetBit(tmpA, 4) == 1 && GetBit(tmpA, 5) == 1 && GetBit(tmpA, 6) == 0)
		{
			tmpC = SetBit(tmpC, 7, 1);
		}
		else
		{
			tmpC = SetBit(tmpC, 7, 0);
		}
		
		PORTC = tmpC;
    }
	
	return 0;
}