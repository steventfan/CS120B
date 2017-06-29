/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 2 Exercise 1
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 


#include <avr/io.h>

	unsigned char GetBit(unsigned char x, unsigned char k) {
		return ( (x & (0x01 << k)) != 0);
	}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tmpA = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char i = 0;
	unsigned char count = 0;
	
    while(1)
    {
		tmpA = PINA;
		tmpB = PINB;
		i = 0;
		count = 0;
		
		while (i < 8)
		{
			if(GetBit(tmpA, i) == 1)
			{
				count = count + 1;
			}
			if(GetBit(tmpB, i) == 1)
			{
				count = count + 1;
			}
			
			i = i + 1;
		}
		
		PORTC = count;
    }
	
	return 0;
}