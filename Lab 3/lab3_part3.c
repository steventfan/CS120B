/* Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 3 Exercise 3
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 


#include <avr/io.h>

enum States {START, STARTHOLD, KEY1, KEY2, UNLOCK, UNLOCKHOLD, LOCK} state;

int main(void)
{
	state = START;
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tmpA = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;
	
	while(1)
	{
		tmpA = PINA;
		
		switch(state)
		{
		case START:
			tmpC = 0;
			if( (tmpA & 4) == 4)
			{
				state = KEY1;
			}
			break;
		case STARTHOLD:
			tmpC = 1;
			if( (tmpA & 4) == 0)
			{
				state = START;
			}
			break;
		case KEY1:
			tmpC = 2;
			if( (tmpA & 4) == 0 && (tmpA & 3) == 0)
			{
				state = KEY2;
			}
			else if( (tmpA & 4) == 4 && (tmpA & 3) == 0)
			{
				state = KEY1;
			}
			else
			{
				if( (tmpA & 4) == 4)
				{
					state = STARTHOLD;
				}
				else
				{
					state = START;
				}
			}
			break;
		case KEY2:
			tmpC = 3;
			if( (tmpA & 2) == 2)
			{
				if( (tmpA & 128) == 128)
				{
					state = UNLOCKHOLD;
				}
				else
				{
					state = UNLOCK;
				}
			}
			else if( (tmpA & 4) == 0 && (tmpA & 2) == 0 && (tmpA & 1) == 0)
			{
				state = KEY2;
			}
			else
			{
				state = START;
			}
			break;
		case UNLOCK:
			tmpB = 1;
			tmpC = 4;
			if( (tmpA & 128) == 128)
			{
				state = LOCK;
			}
			break;
		case UNLOCKHOLD:
			tmpB = 1;
			tmpC = 5;
			if( (tmpA & 128) != 128)
			{
				state = UNLOCK;
			}
			break;
		case LOCK:
			tmpB = 0;
			tmpC = 6;
			if( (tmpA & 2) == 2)
			{
				state = KEY1;
			}
			else
			{
				state = START;
			}
			break;
		}
		
		PORTB = tmpB;
		PORTC = tmpC;
	}
	
	return 0;
}