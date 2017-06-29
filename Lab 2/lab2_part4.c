/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 2 Exercise 4
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
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tmpA = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;
	
    while(1)
    {
		tmpA = PINA;
		
		tmpC = SetBit(tmpC, 4, GetBit(tmpA, 0) );
		tmpC = SetBit(tmpC, 5, GetBit(tmpA, 1) );
		tmpC = SetBit(tmpC, 6, GetBit(tmpA, 2) );
		tmpC = SetBit(tmpC, 7, GetBit(tmpA, 3) );
		tmpB = SetBit(tmpB, 0, GetBit(tmpA, 4) );
		tmpB = SetBit(tmpB, 1, GetBit(tmpA, 5) );
		tmpB = SetBit(tmpB, 2, GetBit(tmpA, 6) );
		tmpB = SetBit(tmpB, 3, GetBit(tmpA, 7) );
		
		PORTB = tmpB;
		PORTC = tmpC;
    }
	
	return 0;
}