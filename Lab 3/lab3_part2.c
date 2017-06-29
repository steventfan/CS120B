/* Partner(s) Name & E-mail: Henry Hua hhua003@ucr.edu, Steven Fan sfan008@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 3 Exercise 2
 * 
 * I acknowledge all content contained herein, excluding template or example code, is my own original work.
 */ 


#include <avr/io.h>

enum States {START, INCREMENT, INCREMENTOFF, DECREMENT, DECREMENTOFF, RESET} state;

int main(void)
{	
	state = START;
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tmpA = 0x00;
	unsigned char tmpC = 0x07;
	
    while(1)
    {
		tmpA = PINA;
		
        switch(state)
        {
	   		case START:
				if( (tmpA & 2) == 0 && (tmpA & 1) == 1)
				{
					state = INCREMENT;
				}
				else if( (tmpA & 2) == 2 && (tmpA & 1) == 0)
				{
					state = DECREMENT;
				}
				else if( (tmpA & 2) == 2 && (tmpA & 1) == 1)
				{
					state = RESET;
				}
				break;
			case INCREMENT:
				if(tmpC < 9)
				{
					tmpC = tmpC + 1;
				}
				if( (tmpA & 2) == 0 && (tmpA & 1) == 0)
				{
					state = START;
				}
				else if( (tmpA & 2) == 0 && (tmpA & 1) == 1)
				{
					state = INCREMENTOFF;
				}
				else if( (tmpA & 2) == 2 && (tmpA & 1) == 0)
				{
					state = DECREMENT;
				}
				else if( (tmpA & 2) == 2 && (tmpA & 1) == 1)
				{
					state = RESET;
				}
				break;
			case INCREMENTOFF:
				if( (tmpA & 2) == 0 && (tmpA & 1) == 0)
				{
					state = START;
				}
				else if( (tmpA & 2) == 2 && (tmpA & 1) == 0)
				{
					state = DECREMENT;
				}
				else if( (tmpA & 2) == 2 && (tmpA & 1) == 1)
				{
					state = RESET;
				}
				break;
			case DECREMENT:
				if(tmpC > 0)
				{
					tmpC = tmpC - 1;
				}
				if( (tmpA & 2) == 0 && (tmpA & 1) == 0)
				{
					state = START;
				}
				else if( (tmpA & 2) == 0 && (tmpA & 1) == 1)
				{
					state = INCREMENT;
				}
				else if( (tmpA & 2) == 2 && (tmpA & 1) == 0)
				{
					state = DECREMENTOFF;
				}
				else if( (tmpA & 2) == 2 && (tmpA & 1) == 1)
				{
					state = RESET;
				}
				break;
			case DECREMENTOFF:
				if( (tmpA & 2) == 0 && (tmpA & 1) == 0)
				{
					state = START;
				}
				else if( (tmpA & 2) == 0 && (tmpA & 1) == 1)
				{
					state = INCREMENT;
				}
				else if( (tmpA & 2) == 2 && (tmpA & 1) == 1)
				{
					state = RESET;
				}
				break;
			case RESET:
				tmpC = 0;
				if( (tmpA & 2) == 0 && (tmpA & 1) == 0)
				{
					state = START;
				}
				else if( (tmpA & 2) == 0 && (tmpA & 1) == 1)
				{
					state = INCREMENT;
				}
				else if( (tmpA & 2) == 2 && (tmpA & 1) == 0)
				{
					state = DECREMENT;
				}
				break;
        }
        	
        PORTC = tmpC;
    }
	
	return 0;
}