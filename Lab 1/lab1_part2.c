/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 1 Exercise 2
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 


#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tmpA = 0x00;
	unsigned char tempVal = 0x00;
	unsigned char count = 0x00;
    while(1)
    {
		count = 0;
        tmpA = PINA & 0x0F;
		tempVal = tmpA & 0x01;
		if (tempVal == 0)
		{
			count = count + 1;
		}
		tempVal = tmpA & 0x02;
		if (tempVal == 0)
		{
			count = count + 1;
		}
		tempVal = tmpA & 0x04;
		if (tempVal == 0)
		{
			count = count + 1;
		}
		tempVal = tmpA & 0x08;
		if (tempVal == 0)
		{
			count = count + 1;
		}
		
		PORTC = count;
    }
	return 0;
}