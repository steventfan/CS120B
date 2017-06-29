/*
 * Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 023
 * Assignment: Lab 1 Exercise 1
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 


#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char tmpA = 0x00;
    while(1)
    {
		tmpA = PINA & 0x03;
		if (tmpA == 0x01){
			tmpB = (tmpB & 0xFC) | 0x01;
		}
		else {
			tmpB = (tmpB & 0xFC) | 0x00;
		}
				
        PORTB = tmpB;
    }
	return 0;
}