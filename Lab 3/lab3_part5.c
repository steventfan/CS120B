/* Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 3 Exercise 5
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 


#include <avr/io.h>

enum States {START, STARTHOLD, STARTHOLDLOCK, KEY1, KEY1LOCK, KEY2, KEY2LOCK, KEY3, KEY3LOCK, KEY4, KEY4LOCK, KEY5, KEY5LOCK, KEY6, KEY6LOCK, UNLOCK, UNLOCKHOLD, LOCK} state;

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
		case STARTHOLDLOCK:
			tmpC = 11;
			if( (tmpA & 128) == 128)
			{
				state = LOCK;
			}
			else if( (tmpA & 4) == 0)
			{
				state = UNLOCK;
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
		case KEY1LOCK:
			tmpC = 12;
			if( (tmpA& 128) == 128)
			{
				state = LOCK;
			}
			else if( (tmpA & 4) == 0 && (tmpA & 3) == 0)
			{
				state = KEY2LOCK;
			}
			else if( (tmpA & 4) == 4 && (tmpA & 3) == 0)
			{
				state = KEY1LOCK;
			}
			else
			{
				if( (tmpA & 4) == 4)
				{
					state = STARTHOLDLOCK;
				}
				else
				{
					state = UNLOCK;
				}
			}
			break;
		case KEY2:
			tmpC = 3;
			if( (tmpA & 4) == 0 && (tmpA & 2) == 0 && (tmpA & 1) == 1)
			{
				state = KEY3;
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
		case KEY2LOCK:
			tmpC = 13;
			if( (tmpA & 128) == 128)
			{
				state = LOCK;
			}
			else if( (tmpA & 4) == 0 && (tmpA & 2) == 0 && (tmpA & 1) == 1)
			{
				state = KEY3LOCK;
			}
			else if( (tmpA & 4) == 0 && (tmpA & 2) == 0 && (tmpA & 1) == 0)
			{
				state = KEY2LOCK;
			}
			else
			{
				state = UNLOCK;
			}
			break;
		case KEY3:
			tmpC = 4;
			if( (tmpA & 4) == 0 && (tmpA & 2) == 0 && (tmpA & 1) == 0)
			{
				state = KEY4;
			}
			else if( (tmpA & 4) == 0 && (tmpA & 2) == 0 && (tmpA & 1) == 1)
			{
				state = KEY3;
			}
			else
			{
				state = START;
			}
			break;
		case KEY3LOCK:
			tmpC = 14;
			if( (tmpA & 128) == 128)
			{
				state = LOCK;
			}
			else if( (tmpA & 4) == 0 && (tmpA & 2) == 0 && (tmpA & 1) == 0)
			{
				state = KEY4LOCK;
			}
			else if( (tmpA & 4) == 0 && (tmpA & 2) == 0 && (tmpA & 1) == 1)
			{
				state = KEY3LOCK;
			}
			else
			{
				state = UNLOCK;
			}
			break;
		case KEY4:
			tmpC = 5;
			if( (tmpA & 4) == 0 && (tmpA & 2) == 2 && (tmpA & 1) == 0)
			{
				state = KEY5;
			}
			else if( (tmpA & 4) == 0 && (tmpA & 2) == 0 && (tmpA & 1) == 0)
			{
				state = KEY4;
			}
			else
			{
				state = START;
			}
			break;
		case KEY4LOCK:
			tmpC = 15;
			if( (tmpA & 128) == 128)
			{
				state = LOCK;
			}
			else if( (tmpA & 4) == 0 && (tmpA & 2) == 2 && (tmpA & 1) == 0)
			{
				state = KEY5LOCK;
			}
			else if( (tmpA & 4) == 0 && (tmpA & 2) == 0 && (tmpA & 1) == 0)
			{
				state = KEY4LOCK;
			}
			else
			{
				state = UNLOCK;
			}
			break;
		case KEY5:
			tmpC = 6;
			if( (tmpA & 4) == 0 && (tmpA & 2) == 0 && (tmpA & 1) == 0)
			{
				state = KEY6;
			}
			else if( (tmpA & 4) == 0 && (tmpA & 2) == 2 && (tmpA & 1) == 0)
			{
				state = KEY5;
			}
			else
			{
				state = START;
			}
			break;
		case KEY5LOCK:
			tmpC = 16;
			if( (tmpA & 128) == 128)
			{
				state = LOCK;
			}
			else if( (tmpA & 4) == 0 && (tmpA & 2) == 0 && (tmpA & 1) == 0)
			{
				state = KEY6LOCK;
			}
			else if( (tmpA & 4) == 0 && (tmpA & 2) == 2 && (tmpA & 1) == 0)
			{
				state = KEY5LOCK;
			}
			else
			{
				state = UNLOCK;
			}
			break;
		case KEY6:
			tmpC = 7;
			if( (tmpA & 4) == 0 && (tmpA & 2) == 0 && (tmpA & 1) == 1)
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
				state = KEY6;
			}
			else
			{
				state = START;
			}
			break;
		case KEY6LOCK:
			tmpC = 17;
			if( ( (tmpA & 4) == 0 && (tmpA & 2) == 0 && (tmpA & 1) == 1) || ( (tmpA & 128) == 128) )
			{
				state = LOCK;
			}
			else if( (tmpA & 4) == 0 && (tmpA & 2) == 0 && (tmpA & 1) == 0)
			{
				state = KEY6LOCK;
			}
			else
			{
				state = UNLOCK;
			}
			break;
		case UNLOCK:
			tmpB = 1;
			tmpC = 8;
			if( (tmpA & 128) == 128)
			{
				state = LOCK;
			}
			else if( (tmpA & 4) == 4 && (tmpA & 2) == 0 && (tmpA & 1) == 0)
			{
				state = KEY1LOCK;
			}
			break;
		case UNLOCKHOLD:
			tmpB = 1;
			tmpC = 9;
			if( (tmpA & 128) != 128)
			{
				state = UNLOCK;
			}
			break;
		case LOCK:
			tmpB = 0;
			tmpC = 10;
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